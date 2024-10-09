#include "linked_list.h"

// create a node with value v
node * create_node(int v) 
{
    node * p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);   // you can be nicer
    
    p->v = v;      
    p->next = NULL;  
    return p;     // return
}

//TODO:
void insert(node **head, node *newnode)
{
    if ((*head) == NULL) {
        (*head) = newnode;
        return;
    }

    node *p = (*head);
    node *q = p;

    while (p != NULL && newnode->v > p->v) {
        q = p;
        p = p->next;
    }

    if (p == (*head)) {
        newnode->next = (*head);
        (*head) = newnode;
    }
    else {
        newnode->next = p;
        q->next = newnode;
    }

    return;
}

// add newnode to the last of the linked list determined by *head and *tail
// note **head, **tail
void insert_last(node **head, node **tail, node *newnode)
{
    if ((*head) == NULL) {
        (*head) = (*tail) = newnode;
    }
    else {
        (*tail)->next = newnode;
        (*tail) = newnode;
    }
}

// remove the first node from the list
// note **head
// return a pointer to the removed content
node * remove_first(node **head) 
{
    node *p;

    p = (*head);
    if ((*head) != NULL) (*head) = (*head)->next;
    return p;
}

// print the list pointed by head
void print_list(node *head)
{
    node *p = head;

    while (p != NULL) {
        printf("%d ", p->v);
        p = p->next;
    }
    printf("\n");
}

//TODO
// merge two sorted linked list as one
node * merge(node **p1, node **p2)
{
    node *head = NULL, *tail = NULL;
    node *q1 = (*p1), *q2 = (*p2), *p;

    if (q1 == NULL) return q2; 
    if (q2 == NULL) return q1; 

    while (q1 != NULL && q2 != NULL) {
        if (q1->v <= q2->v) {
            p = q1;
            q1 = q1->next;
        } else {
            p = q2;
            q2 = q2->next;
        }

        if (head == NULL) {
            head = tail = p;  
        } else {
            tail->next = p;
            tail = p;
        }
    }

    while (q1 != NULL) {
        tail->next = q1;
        tail = q1;
        q1 = q1->next;
    }

    while (q2 != NULL) {
        tail->next = q2;
        tail = q2;
        q2 = q2->next;
    }

    (*p1) = (*p2) = NULL;

    return head;
}

int main(int argc, char *argv[])
{
    assert(argc == 2);
    int n = atoi(argv[1]);

    int a[n];
    int b[n];

    for (int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }

    node *p1 = NULL, *p2 = NULL;

    for (int i = 0; i < n; i++) {
        node *p = create_node(a[i]);
        insert(&p1, p);
    }
    print_list(p1);

    for (int i = 0; i < n; i++) {
        node *p = create_node(b[i]);
        insert(&p2, p);
    }
    print_list(p2);

    node *p = merge(&p1, &p2);
    print_list(p);
    printf("%p\n", p1);
    printf("%p\n", p2);

    while (p != NULL) {
        node *temp = p;
        p = p->next;
        free(temp);
    }

    return 0;
}

