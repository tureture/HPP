#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/*
High Performance Programming
Project: Sudoku Solver
by Ture Hassler

Pthreads version of the code
*/

// function headers
unsigned int validateBoard(unsigned int coordinates, unsigned int num, unsigned int ** board, unsigned int n, unsigned int N);
unsigned int solveBoard(unsigned int ** board, unsigned int n, unsigned int N, unsigned int nr_remaining, unsigned int * unassigned_indicies);
// void *solveBoard(void *arg);
void print_board(unsigned int ** board, unsigned int n, unsigned int N);
void write_board(unsigned int ** board, unsigned int N, char * output);
void* pthread_worker(void* arg);
void *solveBoardGenerator(void *arg);
void *work();


// Input data for pthreads
struct Pthread_data
{
    unsigned int ** board;
    unsigned int nr_remaining;
};

// Global variables accessible inside threads
int solution_found = 0;
int NUM_THREADS;
int n;
int N;
unsigned int * unassigned_indicies;
unsigned int ** board;
int * looking_for_work;
int *** board_array;
int * nr_left;

char* filename_out;

struct Pthread_data * queue;
int nr_active_threads = 0;

// Pthreads stuff
pthread_mutex_t mutex;

int main(int argc, char *argv[]){

    // Parse command line arguments and initialize input variables
    if (argc != 5){
        printf("Usage: ./sudoku n input_filename output_filename num_threads\n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *filename_in= argv[2];
    // char *filename_out = argv[3];    
    int NUM_THREADS = atoi(argv[4]);
    int N = n * n; // Size of board

    // Initialise looking for work array
    looking_for_work = (int *)malloc(NUM_THREADS * sizeof(int));
    for (int i = 0; i < NUM_THREADS; i++){
        looking_for_work[i] = 1;
    }

    // Initialize board array and allocate memory for all threads
    nr_left = (int *)malloc(NUM_THREADS * sizeof(int);
    board_array = (int ***)malloc(NUM_THREADS * sizeof(int **));
    for (int i = 0; i < NUM_THREADS; i++){
        board_array[i] = (int **)malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++){
            board_array[i][j] = (int *)malloc(N * sizeof(int));
        }
    }   



    // Read data from file and initialize board from input file
    FILE *file = fopen(filename_in, "r");
    if (file == NULL){
        printf("Error opening file\n");
        return 1;
    }

    int ** starting_board = board_array[0];
    // Loops through data twice, first to count number of unassigned values and fill upp the board
    // Then to assign them to an array
    int unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            fscanf(file, "%d", &starting_board[i][j]);
            if (board[i][j] == 0){
                unnasigned_n++;
            }
        }
    } 

    unassigned_indicies = (unsigned int *)malloc((unnasigned_n + 1) * sizeof(unsigned int));
    unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (board[i][j] == 0){
                unassigned_indicies[unnasigned_n] = i * N + j;
                unnasigned_n++;
            }
        }
    }

    // Create thread input
    struct Pthread_data * input = (struct Pthread_data *)malloc(sizeof(struct Pthread_data));
    input->board = board;
    input->nr_remaining = unnasigned_n;

    printf("Unassigned: %d \n", unnasigned_n); 
    printf("Input unnasigned number: %d \n", input->nr_remaining);



    // Allocate memory for thread IDs
    pthread_t* threads = (pthread_t*) malloc(NUM_THREADS * sizeof(pthread_t));
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // pthread_create(&threads[0], NULL, solveBoardGenerator, (void*)input);
    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, work, (void*)i);
    }

    // Wait for worker threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
    }

    // Free memory
    free(threads);

    // Free memory
    for (int i = 0; i < N; i++){
        free(board[i]);
    }
    free(board);
    free(queue);
    pthread_mutex_destroy(&mutex);

     
    return 0;
}



unsigned int validateBoard(unsigned int coordinates, unsigned int num, unsigned int ** board, unsigned int n, unsigned int N){
    // Get row and column from single coordinate number (row * N + col)
    unsigned int row = coordinates / N;
    unsigned int col = coordinates % N;
    unsigned int box_x = (row / n) * n;
    unsigned int box_y = (col / n) * n;

    // Check row
    for (int i = 0; i < N; i++){
        if (board[row][i] == num){
            return 0;
        }
    }

    // Check column
    for (int i = 0; i < n; i++){
        if (board[i][col] == num){
            return 0;
        }
    }

    // Check box
    for (int i = box_x; i < box_x + n; i++){
        for (int j = box_y; j < box_y + n; j++){
            if (board[i][j] == num){
                return 0;
            }
        }
    }

    return 1;
}

void *work(void* arg){
    int thread_id = (int)arg[0];

    while (!solution_found) {

        while(looking_for_work[thread_id] == 1){
            // Wait for work
            pthread_sleep(100);
        }

        if(solveBoard(board_array[thread_id], nr_left[thread_id], n, N, unassigned_indicies)){
            // Solution found
            return NULL;
        }

        // No solution found, free memory and look for new work
        free(board_array[thread_id]);
        
        // Lock mutex
        pthread_mutex_lock(&mutex);
        looking_for_work[thread_id] = 1;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
    
void *solveBoardGenerator(void *arg){
    // Solves board using backtracking
    // While generating new pthreads to solve the board in parallel

    struct Pthread_data *input = (struct Pthread_data *)arg;
    unsigned int nr_remaining = input->nr_remaining;
    unsigned int ** board = input->board;


    printf("INside solveBoardGenerator \n");
    printf("Nr remaining: %d \n", nr_remaining);
    printf("Solution found: %d \n", solution_found);

    if (nr_remaining == 0){
        write_board(board, N, "output.txt");
        print_board(board, n, N);
        return NULL;
    }
    else {
        printf("Unnasigned_n: %d \n", nr_remaining);
        unsigned int coordinates = unassigned_indicies[nr_remaining -1];
        int row = coordinates / N;
        int col = coordinates % N;

        printf("Before for loop \n");
        
        for (int i = 1; i <= N; i++){
            if (validateBoard(coordinates, i, board, n, N)){
                board[row][col] = i; 

                printf("INside solveBoardGenerator, inside for \n");

                if (queue == NULL){ // Give board for different pthread to solve
                    printf("Inside queue == NULL \n");

                    // Make copy of board and give it to new pthread
                    struct Pthread_data *out = (struct Pthread_data *)malloc(sizeof(struct Pthread_data));
                    unsigned int ** board_copy = (unsigned int **)malloc(N * sizeof(unsigned int *));
                    for (int i = 0; i < N; i++){
                        board_copy[i] = (unsigned int *)malloc(N * sizeof(unsigned int));
                    }
                    for (int i = 0; i < N; i++){
                        for (int j = 0; j < N; j++){
                            board_copy[i][j] = board[i][j];
                        }
                    }

                    // Add new board to queue
                    out->board = board_copy;
                    out->nr_remaining = nr_remaining - 1;
                    printf("Adding new board to queue \n");
                    pthread_mutex_lock(&mutex);
                    queue = out;
                    pthread_mutex_unlock(&mutex);
                    printf("Values in queue: %d %d \n", queue->nr_remaining, queue->board[0][0]);

                    
                } else {
                    // Keep solving same board
                    input->board = board;
                    input->nr_remaining = nr_remaining - 1;
                    solveBoardGenerator((void *)input);
                }

                
                if (solution_found == 1){
                    return NULL;
                }
                board[row][col] = 0;
            }
        }     
    }  

    
}



/*
void *solveBoard(void *arg)
{
    // Solves entire version of board using backtracking

    struct Pthread_data *input = (struct Pthread_data *)arg;
    unsigned int nr_remaining = input->nr_remaining;
    unsigned int ** board = input->board;

    

    unsigned int row, col;

    if (nr_remaining == 0){
        write_board(board, N, "output.txt");
        print_board(board, n, N);
        return NULL;
    }
    else {
        unsigned int coordinates = unassigned_indicies[nr_remaining -1];
        row = coordinates / N;
        col = coordinates % N;
        
        for (int i = 1; i <= N; i++){
            if (validateBoard(coordinates, i, board, n, N)){
                input -> nr_remaining = nr_remaining - 1;
             
                board[row][col] = i;  
                solveBoard((void *)input);
                if (solution_found == 1){
                    return NULL;
                }
                board[row][col] = 0;
            }
        }     
    }   

    free(input);

    return NULL;
}
*/


unsigned int solveBoard(unsigned int ** board, unsigned int n, unsigned int N, unsigned int nr_remaining, unsigned int * unassigned_indicies){
    printf("Inside solveBoard \n");
    printf("Values inside solveboard: %d, %d, %d, %d \n", n, N, nr_remaining, unassigned_indicies[0]);
    unsigned int row, col;

    if (nr_remaining == 0){
        write_board(board, N, "output.txt");
        print_board(board, n, N);
        return 1;
    }
    else {
        unsigned int coordinates = unassigned_indicies[nr_remaining -1];
        row = coordinates / N;
        col = coordinates % N;
        
        for (int i = 1; i <= N; i++){
            if (validateBoard(coordinates, i, board, n, N)){
                board[row][col] = i;  
                if (solveBoard(board, n, N, nr_remaining - 1, unassigned_indicies)){
                    return 1;
                }
                board[row][col] = 0;
            }
        }     
    }   
    return 0;
}

void print_board(unsigned int ** board, unsigned int n, unsigned int N){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (j % n == 0 && j != 0){
                printf("| ");
            }
            printf("%d ", board[i][j]);
        }
        printf("\n");
        if (i % n == n - 1 && i != N - 1){
            for (int j = 0; j < N; j++){
                printf("--");
            }
            printf("\n");
        }
    }
    printf("\n");
}

void write_board(unsigned int ** board, unsigned int N, char * output){
    FILE *file = fopen(output, "w");
    if (file == NULL){
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }
}

/*
void queue_add(void *value)
{
    pthread_mutex_lock(&mutex);

    

    pthread_mutex_unlock(&mutex);

    
    pthread_cond_signal(&q->cond);
}

void queue_get(queue q, void **val_r)
{
    pthread_mutex_lock(&q->mtx);

    
    while (empty(q))
        rc = pthread_cond_wait(&q->cond, &q->mtx);

    

    pthread_mutex_unlock(&q->mtx);
}

*/
