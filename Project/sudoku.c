#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

/*
High Performance Programming
Project: Sudoku Solver
by Ture Hassler
*/

int main(int argc, char *argv[])
{

    // Parse command line arguments and initialize input variables
    if (argc != 3)
    {
        printf("Usage: ./sudoku n input_filename output_filename \n");
        return 1;
    }
    int n = atoi(argv[1]);


}

bool validateBoard(uint row, uint col, uint num, uint ** board, uint n){
    // Check row
    for (int i = 0; i < n; i++){
        if (board[row][i] == num){
            return false;
        }
    }

    // Check column
    for (int i = 0; i < n; i++){
        if (board[i][col] == num){
            return false;
        }
    }

    // Check box
    uint box_x = row / n;
    uint box_y = col / n;
    for (int i = box_x * n; i < box_x * n + n; i++){
        for (int j = box_y * n; j < box_y * n + n; j++){
            if (board[i][j] == num){
                return false;
            }
        }
    }

    return true;
}
    
uint ** solveBoard(uint ** board, uint n){
    uint row, col;
    if (!findEmpty(board, n, &row, &col)){
        return board;
    }

    for (int i = 1; i <= n; i++){
        if (validateBoard(row, col, i, board, n)){
            board[row][col] = i;
            if (solveBoard(board, n)){
                return board;
            }
            board[row][col] = 0;
        }
    }
    return NULL;
}