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

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

// function headers
int validateBoard(int coordinates,  int num,  int ** board,  int n,  int N); 
void print_board( int ** board,  int n,  int N);
void write_board( int ** board,  int N, char * output);
int solveBoard_serial(int ** board,  int n,  int N,  int nr_remaining,  int * unassigned_indicies, int depth, int * values_to_try);
void shuffle(int *array, size_t n);
int validate_entire_board(int ** board, int n, int N);

int solution_found = 0;



int main(int argc, char *argv[]){

    double start = get_wall_seconds();

    // Parse command line arguments and initialize input variables
    if (argc != 5){
        printf("Usage: ./sudoku n input_filename output_filename nr_threads \n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *filename_in= argv[2];
    // char *filename_out = argv[3];
    int num_threads = atoi(argv[4]);

    int N = n * n; // Size of board

    // 
    // Allocate memory for num_threads boards 
    int *** boards = malloc(num_threads * sizeof(int **));
    for (int i = 0; i < num_threads; i++){
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


    // Loops through data twice, first to count number of unassigned values and fill upp the board
    // Then to assign them to an array
    int unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            int val;
            fscanf(file, "%d", &val);
            for(int k = 0; k < num_threads; k++){
                boards[k][i][j] = val;
            }
            if (val == 0){
                unnasigned_n++;
            }
        }
    } 


    // allocate memory for unassigned indicies
    int ** unnasigned_indicies = malloc((num_threads + 1) * sizeof(int *));
    for (int i = 0; i < num_threads; i++){
        unnasigned_indicies[i] = malloc(unnasigned_n * sizeof(int));
    }

    unnasigned_n = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (boards[0][i][j] == 0){
                for (int k = 0; k < num_threads; k++){
                    if (boards[k][i][j] == 0){
                        unnasigned_indicies[k][unnasigned_n] = i * N + j;
                    }
                }       
                unnasigned_n++;
            }
        }
    }


    // Shuffle unnasigned indicies list
    for (int i = 1; i < num_threads; i++){
        shuffle(unnasigned_indicies[i], unnasigned_n);
    }

    int ** values_to_try_array = malloc(num_threads * sizeof(int *));
    for(int i = 0; i < num_threads; i++){
        values_to_try_array[i] = malloc(N * sizeof(int));
    }

    // fill values to try
    for (int i = 0; i < num_threads; i++){
        for (int j = 0; j < N; j++){
            values_to_try_array[i][j] = j + 1;
        }
    }

    // shuffle values to try
    for (int i = 1; i < num_threads; i++){
        shuffle(values_to_try_array[i], N);
    }
    // double middle = get_wall_seconds();
    


    #pragma omp parallel num_threads(num_threads)
    #pragma omp single
    {
        for (int i=0; i < num_threads; i++){
            #pragma omp task firstprivate(i, boards, n, N, unnasigned_n, unnasigned_indicies, values_to_try_array) shared(solution_found)
            solveBoard_serial(boards[i], n, N, unnasigned_n, unnasigned_indicies[i], 0, values_to_try_array[i]);

        }
    }

    double end = get_wall_seconds();
    printf("Time spent solving: %f \n", end - start);

    /*
    printf("Time spent reading file and shuffling: %f \n", middle - start);
    printf("Time spent solving: %f \n", end - middle);
    printf("Total time: %f \n", end - start);

    */



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
    

 int solveBoard_serial(int ** board,  int n,  int N,  int nr_remaining,  int * unassigned_indicies, int depth, int * values_to_try){
    
    // Check if solution has been found in another thread
    // No flush needed, done automatically by critical section when setting solution_found
    if (solution_found){
    return 1;
    }
    

    int row, col;

    /*
     #pragma omp critical 
    {
        printf("THread number: %d \n", omp_get_thread_num());
        printf("values to try: ");
        for (int i = 0; i < 10; i++){
            printf("%d ", values_to_try[i]);
        }
    }
    
    */


    /*
        #pragma omp critical
    {
        // print first 10 unnasigned indicies
        printf("THread number: %d \n", omp_get_thread_num());
        printf("Unnasigned indicies: ");
        for (int i = 0; i < 10; i++){
            printf("%d ", unassigned_indicies[i]);
        }
        printf("\n");
    }
    
    */

    


    // Check if solution found
    if (nr_remaining == 0){
        #pragma omp critical 
        {
            printf("Solution found \n");
            printf("By thread nr: %d \n", omp_get_thread_num());
            write_board(board, N, "output.txt");
            print_board(board, n, N);
            if (validate_entire_board(board, n, N)){
                // printf("Board is valid \n");
            }
            else{
                printf("Board is not valid \n");
            }
            solution_found = 1;
        }

        return 1;
    }
    else { // solve normally
        int coordinates = unassigned_indicies[nr_remaining -1];
        row = coordinates / N;
        col = coordinates % N;

        
        
        for (int i = 0; i < N; i++){
            int num = values_to_try[i];
            if (validateBoard(coordinates, num, board, n, N)){
                board[row][col] = num;  
                if (solveBoard_serial(board, n, N, nr_remaining - 1, unassigned_indicies, depth + 1, values_to_try)){
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

int validate_entire_board(int ** board, int n, int N){
    int tmp;
    // Check for invalid values
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (board[i][j] != 0){
                tmp = board[i][j]; // weird looking tradeoff I made for a nicer validateBoard function
                board[i][j] = 0;
                if (!validateBoard(i * N + j, tmp, board, n, N)){
                    printf("Coordinate %d %d is not valid \n", i, j);
                    return 0;
                }
                board[i][j] = tmp;
            }
        }
    }

    // Check for zeros
    for(int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (board[i][j] == 0){
                printf("Board is not valid \n");
                return 0;
            }
        }
    }

    return 1;
}