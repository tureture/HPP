#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <omp.h>

/*
High Performance Programming
Project: Sudoku Solver
by Ture Hassler
*/

// function headers
unsigned int validateBoard(unsigned int coordinates, unsigned int num, unsigned int ** board, unsigned int n, unsigned int N);
unsigned int solveBoard(unsigned int ** board, unsigned int n, unsigned int N, unsigned int nr_remaining, unsigned int * unassigned_indicies, int depth);
void print_board(unsigned int ** board, unsigned int n, unsigned int N);
void write_board(unsigned int ** board, unsigned int N, char * output);

int solution_found = 0;
unsigned int ** final_board;
int threads_created = 0;

int main(int argc, char *argv[]){

    // Parse command line arguments and initialize input variables
    if (argc != 5){
        printf("Usage: ./sudoku n input_filename output_filename \n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *filename_in= argv[2];
    // char *filename_out = argv[3];
    int NUM_THREADS = atoi(argv[3]);

    int N = n * n; // Size of board


    // Allocate memory for board of size n^2 x n^2
    unsigned int ** board = (unsigned int **)malloc(N * sizeof(unsigned int *));
    for (int i = 0; i < N; i++){
        board[i] = (unsigned int *)malloc(N * sizeof(unsigned int));
    }

    // Read data from file and initialize board from input file
    FILE *file = fopen(filename_in, "r");
    if (file == NULL){
        printf("Error opening file\n");
        return 1;
    }

    // Loops through data twice, first to count number of unassigned values and fill upp the board
    // Then to assign them to an array
    int unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            fscanf(file, "%d", &board[i][j]);
            if (board[i][j] == 0){
                unnasigned_n++;
            }
        }
    } 

    unsigned int * unassigned_indicies = (unsigned int *)malloc((unnasigned_n + 1) * sizeof(unsigned int));
    unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (board[i][j] == 0){
                unassigned_indicies[unnasigned_n] = i * N + j;
                unnasigned_n++;
            }
        }
    }

    // Print openmp stuff
    printf("Number of threads: %d \n", NUM_THREADS);
    printf("max threads: %d \n", omp_get_max_threads());
    printf("set nested: %d \n", omp_get_nested());
    // printf()

    #pragma omp parallel
    {
        #pragma omp single
        {
            solveBoard(board, n, N, unnasigned_n, unassigned_indicies, 0);
        }
    }
    // solveBoard(board, n, N, unnasigned_n, unassigned_indicies);
    
    printf("Threads created: %d \n", threads_created);

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
    
unsigned int solveBoard(unsigned int ** board, unsigned int n, unsigned int N, unsigned int nr_remaining, unsigned int * unassigned_indicies, int depth){
    
    unsigned int row, col;

    if (solution_found){
        printf("Solution found in another thread \n");
        return 1;
    }

    if (nr_remaining == 0){
            #pragma omp critical 
            {
                printf("Solution found \n");
                    write_board(board, N, "output.txt");
                    print_board(board, n, N);
                solution_found = 1;
            }
        return 1;
    }

    unsigned int coordinates = unassigned_indicies[nr_remaining -1];
    row = coordinates / N;
    col = coordinates % N;
    
    for (int i = 1; i <= N; i++){
        if (validateBoard(coordinates, i, board, n, N)){
            if (depth > 1){
                // printf("Inside depth part \n");
                // printf("Get thread number: %d \n", omp_get_thread_num());
                //printf("Board adress (should be unique): %d \n", &board);
                // printf("Depth: %d \n", depth);
                // Solve serially
                board[row][col] = i;  
                // printf("Depth: %d \n", depth);
                // print_board(board, n, N);
                if (solveBoard(board, n, N, nr_remaining - 1, unassigned_indicies, depth + 1)){
                    return 1;
                }
                board[row][col] = 0;
            }
            else {
                // Solve in parallel
                // printf("Parallel part \n");
                #pragma omp task firstprivate(row, col, i, board, n, N, nr_remaining, unassigned_indicies, depth) priority(depth)
                {       
                        threads_created++;
                        // Allocate memory for board of size n^2 x n^2
                        unsigned int ** board_copy = (unsigned int **)malloc(N * sizeof(unsigned int *));
                        for (int i = 0; i < N; i++){
                            board_copy[i] = (unsigned int *)malloc(N * sizeof(unsigned int));
                        }

                        // Copy board
                        for (int i = 0; i < N; i++){
                            for (int j = 0; j < N; j++){
                                board_copy[i][j] = board[i][j];
                            }
                        }

                        board_copy[row][col] = i;  
                        solveBoard(board_copy, n, N, nr_remaining - 1, unassigned_indicies, depth + 1);
                        // board[row][col] = 0;
                        
                        // Free memory before returning
                        for (int i = 0; i < N; i++){
                            free(board_copy[i]);
                        }
                        free(board_copy);


                } // end omp task
            } 
        }
    }    
    #pragma omp taskwait   
    


    return 0;
}

unsigned int solveBoard_serial(unsigned int ** board, unsigned int n, unsigned int N, unsigned int nr_remaining, unsigned int * unassigned_indicies, int depth){
    
    unsigned int row, col;

    if (solution_found){
        printf("Solution found in another thread \n");
        return 1;
    }

    if (nr_remaining == 0){
            #pragma omp critical 
            {
                printf("Solution found \n");
                    write_board(board, N, "output.txt");
                    print_board(board, n, N);
                solution_found = 1;
            }
        return 1;
    }

    unsigned int coordinates = unassigned_indicies[nr_remaining -1];
    row = coordinates / N;
    col = coordinates % N;
    
    for (int i = 1; i <= N; i++){
        if (validateBoard(coordinates, i, board, n, N)){
            if (depth > 1){
                // printf("Inside depth part \n");
                // printf("Get thread number: %d \n", omp_get_thread_num());
                //printf("Board adress (should be unique): %d \n", &board);
                // printf("Depth: %d \n", depth);
                // Solve serially
                board[row][col] = i;  
                // printf("Depth: %d \n", depth);
                // print_board(board, n, N);
                if (solveBoard(board, n, N, nr_remaining - 1, unassigned_indicies, depth + 1)){
                    return 1;
                }
                board[row][col] = 0;
            }
            else {
                // Solve in parallel
                // printf("Parallel part \n");
                #pragma omp task firstprivate(row, col, i, board, n, N, nr_remaining, unassigned_indicies, depth)
                {       
                        threads_created++;
                        // Allocate memory for board of size n^2 x n^2
                        unsigned int ** board_copy = (unsigned int **)malloc(N * sizeof(unsigned int *));
                        for (int i = 0; i < N; i++){
                            board_copy[i] = (unsigned int *)malloc(N * sizeof(unsigned int));
                        }

                        // Copy board
                        for (int i = 0; i < N; i++){
                            for (int j = 0; j < N; j++){
                                board_copy[i][j] = board[i][j];
                            }
                        }

                        board_copy[row][col] = i;  
                        solveBoard(board_copy, n, N, nr_remaining - 1, unassigned_indicies, depth + 1);
                        // board[row][col] = 0;
                        
                        // Free memory before returning
                        for (int i = 0; i < N; i++){
                            free(board_copy[i]);
                        }
                        free(board_copy);


                } // end omp task
            } 
        }
    }    
    #pragma omp taskwait   
    


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