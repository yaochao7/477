#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/**
 * Hungarian Algorithm Implementation in C
 * 
 * This program implements the Hungarian algorithm (also known as the Kuhn-Munkres algorithm)
 * for solving the assignment problem. The assignment problem is about finding the optimal
 * assignment of n workers to n jobs to minimize the total cost.
 * 
 * Time Complexity: O(n³)
 * Space Complexity: O(n²)
 */

#define MAX_SIZE 100

// Function to find the minimum value in a row
int findMin(int* row, int n) {
    int min = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (row[i] < min) {
            min = row[i];
        }
    }
    return min;
}

// Function to subtract the minimum value from each element in a row
void subtractMin(int* row, int min, int n) {
    for (int i = 0; i < n; i++) {
        row[i] -= min;
    }
}

// Function to find the minimum value in a column
int findMinInColumn(int** matrix, int col, int n) {
    int min = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (matrix[i][col] < min) {
            min = matrix[i][col];
        }
    }
    return min;
}

// Function to subtract the minimum value from each element in a column
void subtractMinFromColumn(int** matrix, int col, int min, int n) {
    for (int i = 0; i < n; i++) {
        matrix[i][col] -= min;
    }
}

// Function to print the cost matrix
void printMatrix(int** matrix, int n) {
    printf("\nCost Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to find a zero in a row that has no starred zero
int findZero(int** matrix, bool* rowCovered, bool* colCovered, int n) {
    for (int i = 0; i < n; i++) {
        if (!rowCovered[i]) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0 && !colCovered[j]) {
                    return i * n + j; // Return the position of the zero
                }
            }
        }
    }
    return -1; // No zero found
}

// Function to find a starred zero in a column
int findStarInCol(int* colStar, int col, int n) {
    for (int i = 0; i < n; i++) {
        if (colStar[i] == col) {
            return i;
        }
    }
    return -1; // No starred zero found
}

// Function to find a primed zero in a row
int findPrimeInRow(int* rowPrime, int row, int n) {
    for (int j = 0; j < n; j++) {
        if (rowPrime[j] == row) {
            return j;
        }
    }
    return -1; // No primed zero found
}

// Function to find the minimum uncovered value
int findMinUncovered(int** matrix, bool* rowCovered, bool* colCovered, int n) {
    int min = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (!rowCovered[i]) {
            for (int j = 0; j < n; j++) {
                if (!colCovered[j] && matrix[i][j] < min) {
                    min = matrix[i][j];
                }
            }
        }
    }
    return min;
}

// Function to implement the Hungarian algorithm
void hungarianAlgorithm(int** costMatrix, int n, int* assignment) {
    // Step 1: Subtract the smallest entry in each row from all entries in that row
    for (int i = 0; i < n; i++) {
        int min = findMin(costMatrix[i], n);
        subtractMin(costMatrix[i], min, n);
    }

    // Step 2: Subtract the smallest entry in each column from all entries in that column
    for (int j = 0; j < n; j++) {
        int min = findMinInColumn(costMatrix, j, n);
        subtractMinFromColumn(costMatrix, j, min, n);
    }

    // Arrays to keep track of starred and primed zeros
    int* rowStar = (int*)calloc(n, sizeof(int));
    int* colStar = (int*)calloc(n, sizeof(int));
    int* rowPrime = (int*)calloc(n, sizeof(int));
    int* colPrime = (int*)calloc(n, sizeof(int));

    // Initialize arrays to -1 (no stars or primes)
    for (int i = 0; i < n; i++) {
        rowStar[i] = -1;
        colStar[i] = -1;
        rowPrime[i] = -1;
        colPrime[i] = -1;
    }

    // Step 3: Find a zero in the matrix. If there is no starred zero in its row or column, star it.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (costMatrix[i][j] == 0 && rowStar[i] == -1 && colStar[j] == -1) {
                rowStar[i] = j;
                colStar[j] = i;
            }
        }
    }

    // Arrays to keep track of covered rows and columns
    bool* rowCovered = (bool*)calloc(n, sizeof(bool));
    bool* colCovered = (bool*)calloc(n, sizeof(bool));

    // Main loop of the algorithm
    while (true) {
        // Check if we have found an optimal assignment
        bool done = true;
        for (int j = 0; j < n; j++) {
            if (colStar[j] == -1) {
                done = false;
                break;
            }
        }

        if (done) {
            break;
        }

        // Reset covered arrays
        for (int i = 0; i < n; i++) {
            rowCovered[i] = false;
            colCovered[i] = false;
        }

        // Cover columns with starred zeros
        for (int j = 0; j < n; j++) {
            if (colStar[j] != -1) {
                colCovered[j] = true;
            }
        }

        // Find a non-covered zero
        int zeroPos = findZero(costMatrix, rowCovered, colCovered, n);
        while (zeroPos != -1) {
            int row = zeroPos / n;
            int col = zeroPos % n;

            // Prime the zero
            rowPrime[row] = col;
            colPrime[col] = row;

            // Check if there is a starred zero in the same row
            int starCol = rowStar[row];
            if (starCol != -1) {
                // Cover the row and uncover the column with the starred zero
                rowCovered[row] = true;
                colCovered[starCol] = false;
            } else {
                // No starred zero in the row, we have an augmenting path
                int r = row;
                int c = col;

                // Augment the path
                while (true) {
                    // Star the primed zero
                    rowStar[r] = c;
                    colStar[c] = r;

                    // Find a starred zero in the same column
                    r = colPrime[c];
                    if (r == -1) {
                        break;
                    }

                    // Find a primed zero in the same row
                    c = rowPrime[r];
                }

                // Reset primed zeros
                for (int i = 0; i < n; i++) {
                    rowPrime[i] = -1;
                    colPrime[i] = -1;
                }

                // Reset covered arrays
                for (int i = 0; i < n; i++) {
                    rowCovered[i] = false;
                    colCovered[i] = false;
                }

                // Cover columns with starred zeros
                for (int j = 0; j < n; j++) {
                    if (colStar[j] != -1) {
                        colCovered[j] = true;
                    }
                }

                break;
            }

            // Find another non-covered zero
            zeroPos = findZero(costMatrix, rowCovered, colCovered, n);
        }

        // If no non-covered zero was found, find the minimum uncovered value
        if (zeroPos == -1) {
            int minVal = findMinUncovered(costMatrix, rowCovered, colCovered, n);

            // Add the minimum value to covered rows
            for (int i = 0; i < n; i++) {
                if (rowCovered[i]) {
                    for (int j = 0; j < n; j++) {
                        costMatrix[i][j] += minVal;
                    }
                }
            }

            // Subtract the minimum value from uncovered columns
            for (int j = 0; j < n; j++) {
                if (!colCovered[j]) {
                    for (int i = 0; i < n; i++) {
                        costMatrix[i][j] -= minVal;
                    }
                }
            }
        }
    }

    // Extract the optimal assignment
    for (int i = 0; i < n; i++) {
        assignment[i] = rowStar[i];
    }

    // Free allocated memory
    free(rowStar);
    free(colStar);
    free(rowPrime);
    free(colPrime);
    free(rowCovered);
    free(colCovered);
}

// Function to calculate the total cost of an assignment
int calculateTotalCost(int** costMatrix, int* assignment, int n) {
    int totalCost = 0;
    for (int i = 0; i < n; i++) {
        totalCost += costMatrix[i][assignment[i]];
    }
    return totalCost;
}

// Function to print the optimal assignment
void printAssignment(int* assignment, int** originalCostMatrix, int n) {
    printf("\nOptimal Assignment:\n");
    int totalCost = 0;
    for (int i = 0; i < n; i++) {
        printf("Worker %d assigned to Job %d with cost %d\n", 
               i + 1, assignment[i] + 1, originalCostMatrix[i][assignment[i]]);
        totalCost += originalCostMatrix[i][assignment[i]];
    }
    printf("\nTotal Cost: %d\n", totalCost);
}

// Function to create a copy of the cost matrix
int** createCopyMatrix(int** matrix, int n) {
    int** copy = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        copy[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
    return copy;
}

// Function to free a matrix
void freeMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int n;
    printf("Hungarian Algorithm - Assignment Problem Solver\n");
    printf("==============================================\n");
    printf("Enter the number of workers/jobs (n): ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_SIZE) {
        printf("Invalid input. Number of workers/jobs should be between 1 and %d.\n", MAX_SIZE);
        return 1;
    }

    // Allocate memory for the cost matrix
    int** costMatrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        costMatrix[i] = (int*)malloc(n * sizeof(int));
    }

    printf("\nEnter the cost matrix (%d x %d):\n", n, n);
    printf("(Cost of assigning worker i to job j)\n");
    for (int i = 0; i < n; i++) {
        printf("Enter costs for Worker %d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf("%d", &costMatrix[i][j]);
        }
    }

    // Print the original cost matrix
    printf("\nOriginal Cost Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", costMatrix[i][j]);
        }
        printf("\n");
    }

    // Create a copy of the cost matrix for the algorithm
    int** workMatrix = createCopyMatrix(costMatrix, n);

    // Allocate memory for the assignment array
    int* assignment = (int*)malloc(n * sizeof(int));

    // Run the Hungarian algorithm
    hungarianAlgorithm(workMatrix, n, assignment);

    // Print the optimal assignment
    printAssignment(assignment, costMatrix, n);

    // Free allocated memory
    freeMatrix(costMatrix, n);
    freeMatrix(workMatrix, n);
    free(assignment);

    return 0;
}

