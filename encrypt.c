#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000   // We have less than 60000 words
#define MAX_WORD_LENGTH 80     // Each word is less than 80 letters

// Using these two global variables can be justified :)
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];  // 2D array to hold all the words
int word_count = 0;  // Number of words, initialized to 0

// Note the words in the dictionary file are sorted
// This fact could be useful
void read_file_to_array(char *filename) {
    FILE *fp;

    // Open the file for reading
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }
    
    // Read words from the file, ensure no newline characters in the array
    while (!feof(fp) && word_count < MAX_WORD_COUNT) {  // Add a bounds check for word_count
        fscanf(fp, "%s\n", words[word_count++]);
    }
    fclose(fp);

    // Debug: Print the first few words read from the dictionary
    printf("Dictionary loaded, first 5 words:\n");
    for (int i = 0; i < 5 && i < word_count; i++) {
        printf("%s\n", words[i]);
    }
}

// Test whether a string word is in the dictionary
// Return 1 if word is in the dictionary
// Return 0 otherwise
// Be efficient in implementing this function
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

// Decrypt the encrypted message using only XOR
// len is the length of the encrypted message
// The result is stored in the decrypted array
void decryption(unsigned char key, const int *encrypted, int len, char *decrypted) {
    printf("Trying Key: %u\n", key);  // Debug: Print key

    for (int i = 0; i < len; i++) {
        char decrypted_char = (char)(encrypted[i] ^ key);  // XOR only
        if (isprint(decrypted_char)) {
            decrypted[i] = decrypted_char;
        } else {
            decrypted[i] = '?';  // Replace non-printable characters with '?'
        }
        // Debug: Print each step of decryption
        printf("Encrypted[%d]: %d, Decrypted Char: %c\n", i, encrypted[i], decrypted[i]);
    }
    decrypted[len] = '\0';  // Null-terminate the string
}

// Calculate a score for a message msg
// The score is used to determine whether msg is the original message
int message_score(const char *msg) {
    int score = 0;
    char word[MAX_WORD_LENGTH];
    int i = 0, j = 0;

    // Loop through the message and extract words
    while (msg[i] != '\0') {
        if (isalpha(msg[i])) {
            word[j++] = msg[i];  // Collect word characters
        } else if (j > 0) {
            word[j] = '\0';  // Null-terminate the word
            if (in_dict(word)) {
                score++;  // Increment score if word is found in dictionary
            }
            j = 0;
        }
        i++;
    }

    if (j > 0) {  // Check last word
        word[j] = '\0';
        if (in_dict(word)) {
            score++;
        }
    }
    return score;
}

// Read the encrypted message from the file to encrypted
// Return number of bytes read
int read_encrypted(char *filename, int *encrypted) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Cannot open file %s\n", filename);
        return -1;
    }
    int len = read(fd, encrypted, MAX * sizeof(int));
    close(fd);
    
    if (len / sizeof(int) > MAX) {  // Check for overflow
        printf("Error: Encrypted message is too long!\n");
        return -1;
    }

    len = len / sizeof(int);  // Convert to number of integers read
    printf("Read %d integers from %s\n", len, filename);

    // Debug: Print the first few integers from the encrypted message
    for (int i = 0; i < len && i < 10; i++) {
        printf("Encrypted[%d]: %d\n", i, encrypted[i]);
    }

    return len;
}

// Search for the best decryption by varying the key (no shift)
void search(const int *encrypted, int len, char *message) {
    char decrypted[MAX];
    int max_score = 0;
    strcpy(message, "");  // Clear the message buffer

    // Try all keys (without shift)
    for (unsigned char k = 0; k < 255; k++) {
        decryption(k, encrypted, len, decrypted);  // No shift
        int score = message_score(decrypted);

        // Store the best scoring message
        if (score > max_score) {
            max_score = score;
            strcpy(message, decrypted);
        }
    }
}

// Do not change the main() function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s encrypted-message\n", argv[0]);
        return 0;
    }

    // Read the dictionary file
    read_file_to_array("dict.txt");

    // Read the encrypted message
    int encrypted[MAX];
    int len = read_encrypted(argv[1], encrypted);

    // Search for the best decryption
    char message[MAX];
    strcpy(message, "");
    search(encrypted, len, message);

    // Print the best decrypted message
    printf("Decrypted message: %s\n", message);

    return 0;
}
