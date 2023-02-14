#include <stdio.h>
#include <stdlib.h>

/*
Assignment 2 HPP, part3
Ture Hassler & Jacob Malmenstedt
*/

/// Define struct for linked list node containing data for a given day
typedef struct node{
    unsigned int day;
    double min;
    double max;
    struct node *next;
} node_t;

// Function headers
void add(node_t ** head, unsigned int day, double min, double max);
void print_list(node_t * head);
void delete(node_t ** head, int day);

int main() {

    // initialize linked list pointer to first node
    node_t *jan_ll = NULL;
 
    char command;
    int day;
    double min, max;

    // give initial command
    printf("Enter command: ");
    scanf("%c", &command);

    while (command != 'Q'){

        // Switch case for all commands
        switch (command) {
            case 'A': // read data and add to list
                scanf("%d %lf %lf", &day, &min, &max);
                add(&jan_ll, day, min, max);
                printf("%d %lf %lf \n", day, min, max);

                break;
            
            case 'D': // read day and delete that from list
                scanf("%d", &day);
                delete(&jan_ll, day);
                break;

            case 'P': // print list
                print_list(jan_ll);
                break;

            default:
                printf("Invalid command. Try again thx. \n");
        }

        printf("Enter command: ");
        scanf(" %c", &command);
    }

  return 0;
}


// All methods modified and built upon this link:
// https://www.learn-c.org/en/Linked_lists


// Add to list
void add(node_t ** head, unsigned int day, double min, double max) {
    node_t * current;

    // allocate memory
    node_t * new_node;
    new_node = (node_t *) malloc(sizeof(node_t));

    // set day, min, max in the new node
    new_node -> day = day;
    new_node -> min = min;
    new_node -> max = max;
    new_node -> next = NULL;

    // List is empty
    if (*head == NULL){

        // set head so it points to the new node
        *head = new_node;

    }
    // if list is not empty
    else {
        current = *head;

         // case when adding to the beginning of the list
        if (day < current->day) {
            new_node->next = current;
            *head = new_node;
        } 
        else { // Case when adding to the middle or end of the list

            // loop through list
            while (current -> next != NULL && day >= current -> next -> day){
            current = current -> next;
            }

            // if day already exists, edit instead
            if (day == current -> day){
                current -> min = min;
                current -> max = max;

                free(new_node); // free memory, required since i always allocate memory at the start, not beatiful but works for now
                return;
            }
            else {
                // set current next to the new node, if 
                new_node -> next = current -> next;
                current -> next = new_node;

            }        
        }
    }

    return;
}

void print_list(node_t * head) {
    node_t * current = head;

    printf("day min max \n");

    while (current != NULL) {
        printf("%d %f %f \n", current->day, current->min, current->max);
        current = current->next;
    }
}

// Delete node from list, doesn't work if node dont exist, don't try that lol
void delete(node_t ** head, int day) {
    node_t * current = *head;
    node_t * prev = NULL;

    current = *head;

    // case when its the first element of the list 
    if (current -> day == day){
        *head = current -> next; // Set head to second element
        return;
    }
    else {
        // loop through list
        while (current -> day != day){
            prev = current;
            current = current -> next;
        }

        // remove node from list
        prev -> next = current -> next;
    }

    free(current); // free memory of the node we want to delete
    return;
}


