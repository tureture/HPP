#include <pthread.h>
#include <stdio.h>
# include <stdlib.h>

// The function that will be executed by each thread
void *print_index(void *thread_id) {
    // Convert the void pointer to an int pointer
    int id = *((int *) thread_id);

    // Print the thread index
    printf("Thread %d\n", id);

    // Free the memory allocated for the thread index
    free(thread_id);

    // Exit the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n;

    // Check if the number of arguments is correct
    if (argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return 1;
    }

    // Convert the argument to an integer
    n = atoi(argv[1]);

    // Array to store the thread IDs
    pthread_t threads[n];

    int i, status;
    for (i = 0; i < n; i++) {
        // Allocate memory for the thread index
        int *id = (int *) malloc(sizeof(int));
        *id = i;

        // Create the thread
        status = pthread_create(&threads[i], NULL, print_index, (void *) id);
        if (status != 0) {
            printf("Error creating thread %d\n", i);
        }
    }

    // Wait for all threads to exit
    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}