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
int validateBoard(int coordinates,  int num,  int ** board,  int n,  int N); 
int solveBoard( int ** board,  int n,  int N,  int nr_remaining,  int * unassigned_indicies, int depth);
void print_board( int ** board,  int n,  int N);
void write_board( int ** board,  int N, char * output);
int solveBoard_serial(int ** board,  int n,  int N,  int nr_remaining,  int * unassigned_indicies, int depth);
void shuffle(int *array, size_t n);

int solution_found = 0;


int main(int argc, char *argv[]){

    // Parse command line arguments and initialize input variables
    if (argc != 5){
        printf("Usage: ./sudoku n input_filename output_filename nr_threads \n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *filename_in= argv[2];
    // char *filename_out = argv[3];
    int NUM_THREADS = atoi(argv[4]);

    int N = n * n; // Size of board

    // Allocate memory for NUM_THREADS boards 
    int *** boards = malloc(NUM_THREADS * sizeof(int **));
    for (int i = 0; i < NUM_THREADS; i++){
        boards[i] = malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++){
            boards[i][j] = malloc(N * sizeof(int));
        }
    }


    // Read data from file and initialize board from input file
    FILE *file = fopen(filename_in, "r");
    if (file == NULL){
        printf("Error opening file\n");
        return 1;
    }   

    // After file reead
    printf("File read\n");


    // Loops through data twice, first to count number of unassigned values and fill upp the board
    // Then to assign them to an array
    int unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            int val;
            fscanf(file, "%d", &val);
            if (boards[0][i][j] == 0){
                for(int k = 0; k < NUM_THREADS; k++){
                    boards[k][i][j] = val;
                }
                unnasigned_n++;
            }
        }
    } 

    // allocate memory for unassigned indicies
    int ** unnasigned_indicies = malloc(NUM_THREADS * sizeof(int *));
    for (int i = 0; i < NUM_THREADS; i++){
        unnasigned_indicies[i] = malloc(unnasigned_n * sizeof(int));
    }

    unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (boards[0][i][j] == 0){
                for (int k = 0; k < NUM_THREADS; k++){
                    unnasigned_indicies[k][unnasigned_n] = i * N + j;
                }       
                unnasigned_n++;
            }
        }
    }

    // Shuffle unnasigned indicies list
    for (int i = 1; i < NUM_THREADS; i++){
        shuffle(unnasigned_indicies[i], unnasigned_n);
    }


    // Print openmp stuff
    printf("Number of threads: %d \n", NUM_THREADS);
    printf("max threads: %d \n", omp_get_max_threads());

    
    // Solve board
    #pragma omp parallel 
    {
        #pragma omp taskloop num_tasks(NUM_THREADS) firstprivate(boards, n, N, unnasigned_n, unnasigned_indicies)
        for (int i=0; i < NUM_THREADS; i++){
        solveBoard_serial(boards[i], n, N, unnasigned_n, unnasigned_indicies[i], 0);   
        }

    }
    
    printf("Done \n");

    return 0;
}

 int validateBoard( int coordinates,  int num, int ** board,  int n,  int N){
    // Get row and column from single coordinate number (row * N + col)
     int row = coordinates / N;
     int col = coordinates % N;
     int box_x = (row / n) * n;
     int box_y = (col / n) * n;

    // Check row
    for (int i = 0; i < N; i++){
        if (board[row][i] == num){
            return 0;
        }
    }

    // Check column
    for (int i = 0; i < N; i++){
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
    

 int solveBoard_serial(int ** board,  int n,  int N,  int nr_remaining,  int * unassigned_indicies, int depth){
    

    #pragma omp flush (solution_found)
    if (solution_found){
        printf("Solution found in another thread \n");
        return 1;
    }

    int row, col;

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
    else {
         int coordinates = unassigned_indicies[nr_remaining -1];
        row = coordinates / N;
        col = coordinates % N;
        
        for (int i = 1; i <= N; i++){
            if (validateBoard(coordinates, i, board, n, N)){
                board[row][col] = i;  
                if (solveBoard_serial(board, n, N, nr_remaining - 1, unassigned_indicies, depth + 1)){
                    return 1;
                }
                board[row][col] = 0;
            }
        }     
    }   
    return 0;
}

void print_board(int ** board, int n, int N){
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

void write_board(int ** board, int N, char * output){
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

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}