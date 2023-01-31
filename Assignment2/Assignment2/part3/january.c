#include <stdio.h>
#include <stdlib.h>

/*
Assignment 2 HPP, part3
Ture Hassler & Jacob Malmenstedt
*/

/// Define struct for linked list node containing data for a given day
typedef struct node{
    int day;
    int min;
    int max;
    struct node *next;
} node_t;


int main() {

    // initialize linked list pointer and allocate memory for first node
    node_t *jan_ll = NULL;
    jan_ll = (node_t*)malloc(sizeof(node_t)); 

    //
    char command;
    int day;
    double min, max;

    // give initial command
    printf("Enter command: ");
    scanf("%c", &command);

    while (command != 'Q'){

        // Switch case for all commands
        switch (command) {
            case 'A':
                // read in day, min and max
                scanf("%d %lf %lf", &day, &min, &max);
                printf("%d %lf %lf \n", day, min, max);

                break;
            
            case 'D':
                // do stuff, deletes day
                break;

            case 'P':
                // do stuff, print all days
                break;

            default:
                printf("Invalid command. Try again thx. \n");
        }


        printf("Enter command: ");
        scanf(" %c", &command);


    }

  return 0;
}

// combine these and stuff
// All borrowed from https://www.learn-c.org/en/Linked_lists

// Add to end of list 
void push(node_t * head, int day, double min, double max) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

// Add to beginning of list
void push(node_t ** head, int val) {
    node_t * new_node;
    new_node = (node_t *) malloc(sizeof(node_t));

    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

// remove specific index from list
int remove_by_index(node_t ** head, int n) {
    int i = 0;
    int retval = -1;
    node_t * current = *head;
    node_t * temp_node = NULL;

    if (n == 0) {
        return pop(head);
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL) {
        return -1;
    }

    temp_node = current->next;
    retval = temp_node->val;
    current->next = temp_node->next;
    free(temp_node);

    return retval;

}


