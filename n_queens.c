#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * N-Queens Problem Solver
 * 
 * This program solves the classic N-Queens puzzle using backtracking.
 * The N-Queens problem asks how to place N queens on an NxN chessboard
 * so that no two queens threaten each other (no two queens share the
 * same row, column, or diagonal).
 */

// Function to print the chessboard
void printSolution(int *board, int n) {
    printf("\nSolution:\n");
    
    // Print the top border
    printf("+");
    for (int i = 0; i < n; i++) {
        printf("---+");
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("|");
        for (int j = 0; j < n; j++) {
            if (board[i] == j) {
                printf(" Q |"); // Queen position
            } else {
                printf("   |"); // Empty position
            }
        }
        
        // Print row separator
        printf("\n+");
        for (int j = 0; j < n; j++) {
            printf("---+");
        }
        printf("\n");
    }
}

// Function to check if a queen can be placed at board[row] = col
bool isSafe(int *board, int row, int col) {
    // Check for queens in the same column or diagonals
    for (int i = 0; i < row; i++) {
        // If there's a queen in the same column or diagonals
        if (board[i] == col || 
            board[i] - i == col - row || 
            board[i] + i == col + row) {
            return false;
        }
    }
    return true;
}

// Recursive function to solve N-Queens using backtracking
bool solveNQueensUtil(int *board, int row, int n) {
    // Base case: If all queens are placed
    if (row == n) {
        return true;
    }
    
    // Try placing queen in each column of the current row
    for (int col = 0; col < n; col++) {
        // Check if queen can be placed at board[row][col]
        if (isSafe(board, row, col)) {
            // Place the queen
            board[row] = col;
            
            // Recursively place rest of the queens
            if (solveNQueensUtil(board, row + 1, n)) {
                return true;
            }
            
            // If placing queen doesn't lead to a solution, backtrack
            board[row] = -1; // Backtrack
        }
    }
    
    // If queen cannot be placed in any column in this row
    return false;
}

// Function to solve the N-Queens problem
bool solveNQueens(int n) {
    // Create a 1D array for the board
    // board[i] = j means a queen is placed at position (i,j)
    int *board = (int *)malloc(n * sizeof(int));
    if (board == NULL) {
        printf("Memory allocation failed\n");
        return false;
    }
    
    // Initialize the board with -1 (no queens placed)
    for (int i = 0; i < n; i++) {
        board[i] = -1;
    }
    
    // Solve the problem
    if (solveNQueensUtil(board, 0, n)) {
        // Print the solution
        printSolution(board, n);
        free(board);
        return true;
    } else {
        printf("No solution exists for %d-Queens problem\n", n);
        free(board);
        return false;
    }
}

// Function to find all solutions to the N-Queens problem
int countNQueensSolutions(int n) {
    int *board = (int *)malloc(n * sizeof(int));
    int count = 0;
    
    if (board == NULL) {
        printf("Memory allocation failed\n");
        return 0;
    }
    
    // Initialize the board with -1 (no queens placed)
    for (int i = 0; i < n; i++) {
        board[i] = -1;
    }
    
    // Function to count all solutions
    void countSolutionsUtil(int row) {
        // Base case: If all queens are placed
        if (row == n) {
            count++;
            printf("\nSolution #%d:\n", count);
            printSolution(board, n);
            return;
        }
        
        // Try placing queen in each column of the current row
        for (int col = 0; col < n; col++) {
            if (isSafe(board, row, col)) {
                // Place the queen
                board[row] = col;
                
                // Recursively place rest of the queens
                countSolutionsUtil(row + 1);
                
                // Backtrack
                board[row] = -1;
            }
        }
    }
    
    countSolutionsUtil(0);
    
    free(board);
    return count;
}

int main() {
    int n, choice;
    
    printf("N-Queens Problem Solver\n");
    printf("=======================\n");
    printf("Enter the size of the chessboard (N): ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Invalid board size. Please enter a positive integer.\n");
        return 1;
    }
    
    if (n == 2 || n == 3) {
        printf("No solution exists for %d-Queens problem\n", n);
        return 0;
    }
    
    printf("\nChoose an option:\n");
    printf("1. Find one solution\n");
    printf("2. Find all solutions (warning: can be slow for N > 8)\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            if (!solveNQueens(n)) {
                printf("Failed to find a solution.\n");
            }
            break;
        case 2:
            if (n > 12) {
                printf("Warning: Finding all solutions for N > 12 may take a very long time.\n");
                printf("Do you want to continue? (1 for Yes, 0 for No): ");
                int confirm;
                scanf("%d", &confirm);
                if (!confirm) {
                    return 0;
                }
            }
            int solutions = countNQueensSolutions(n);
            printf("\nTotal number of solutions for %d-Queens: %d\n", n, solutions);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
    
    return 0;
}

