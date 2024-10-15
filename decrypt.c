#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240  // Maximum buffer size for encrypted and decrypted messages
#define MAX_WORD_COUNT 60000   // Maximum number of words in the dictionary
#define MAX_WORD_LENGTH 80     // Maximum length of each word in the dictionary

// Global variables to store the dictionary words and their count
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];  // 2D array to hold all the words
int word_count = 0;  // Number of words in the dictionary

// Function to load the dictionary into a 2D array
void read_file_to_array(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }

    while (fscanf(fp, "%79s", words[word_count]) == 1 && word_count < MAX_WORD_COUNT) {
        word_count++;
    }
    fclose(fp);

    // Debug: Print the first few words read from the dictionary
    printf("Dictionary loaded, first 5 words:\n");
    for (int i = 0; i < 5 && i < word_count; i++) {
        printf("%s\n", words[i]);
    }
}

// Function to check if a word is in the dictionary (using binary search)
int in_dict(char *word) {
    int low = 0, high = word_count - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(word, words[mid]);
        if (cmp == 0) {
            return 1;  // Word found
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return 0;  // Word not found
}

// Function to decrypt the message using XOR with a given key
void decryption(unsigned char key, const int *encrypted, int len, char *decrypted) {
    if (len >= MAX) {
        printf("Error: Decrypted message too long!\n");
        return;
    }

    for (int i = 0; i < len; i++) {
        unsigned char decrypted_char = (unsigned char)(encrypted[i] ^ key);
        decrypted[i] = isprint(decrypted_char) ? decrypted_char : '.';
    }
    decrypted[len] = '\0';  // Null-terminate the string
}

// Function to calculate the score of the decrypted message
int message_score(const char *msg) {
    int score = 0;
    char word[MAX_WORD_LENGTH];
    int i = 0, j = 0;

    while (msg[i] != '\0') {
        if (isalpha(msg[i]) || msg[i] == '\'') {
            if (j < MAX_WORD_LENGTH - 1) {
                word[j++] = tolower(msg[i]);  // Convert to lowercase
            }
        } else if (j > 0) {
            word[j] = '\0';
            if (in_dict(word)) {
                score++;
            }
            j = 0;
        }
        i++;
    }

    if (j > 0) {
        word[j] = '\0';
        if (in_dict(word)) {
            score++;
        }
    }

    return score;
}

// Function to read the encrypted message from a file
int read_encrypted(char *filename, int *encrypted) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Cannot open file %s\n", filename);
        return -1;
    }

    int len = read(fd, encrypted, MAX * sizeof(int));
    close(fd);

    if (len < 0) {
        printf("Error reading file %s\n", filename);
        return -1;
    }

    len = len / sizeof(int);  // Convert bytes read into number of integers

    if (len > MAX) {  // Ensure len does not exceed MAX
        printf("Error: Encrypted message is too long!\n");
        return -1;
    }

    printf("Read %d integers from %s\n", len, filename);
    return len;
}

// Function to search for the best decryption using different key values
void search(const int *encrypted, int len, char *message) {
    char decrypted[MAX];
    int max_score = 0;
    strcpy(message, "");  // Initialize message buffer

    for (unsigned int k = 0; k < 256; k++) {
        decryption(k, encrypted, len, decrypted);
        int score = message_score(decrypted);

        // Print the first 200 characters of each decryption attempt
        printf("Key: %u, Score: %d, First 200 chars: %.200s\n", k, score, decrypted);

        if (score > max_score) {
            max_score = score;
            strncpy(message, decrypted, MAX - 1);  // Copy decrypted message, limit to MAX
            message[MAX - 1] = '\0';  // Null-terminate
        }
    }

    printf("Best decryption (score %d): %s\n", max_score, message);
}

// Main function to handle input arguments and run the decryption
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s encrypted-message1\n", argv[0]);
        return 0;
    }

    // Load the dictionary
    read_file_to_array("dict.txt");

    // Read the encrypted message
    int encrypted[MAX];
    int len = read_encrypted(argv[1], encrypted);

    if (len > 0 && len <= MAX) {
        printf("First few encrypted integers:\n");
        for (int i = 0; i < 5 && i < len; i++) {
            printf("%d ", encrypted[i]);
        }
        printf("\n");

        // Decrypt the message and find the best result
        char message[MAX];
        search(encrypted, len, message);
    } else {
        printf("Error: Could not read encrypted message or message too long.\n");
    }

    return 0;
}