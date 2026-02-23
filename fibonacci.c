#include <stdio.h>

/**
 * Program to calculate and display the Fibonacci sequence
 * The Fibonacci sequence is a series where each number is the sum of the two preceding ones,
 * usually starting with 0 and 1.
 */

int main() {
    int n, i;
    unsigned long long fib1 = 0, fib2 = 1, next;

    printf("Fibonacci Sequence Calculator\n");
    printf("Enter the number of terms: ");
    scanf("%d", &n);

    // Input validation
    if (n <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    printf("Fibonacci Sequence (first %d terms):\n", n);

    // For n = 1, only print the first term
    if (n >= 1) {
        printf("%llu", fib1);
    }
    
    // For n >= 2, print the second term and calculate the rest
    if (n >= 2) {
        printf(", %llu", fib2);
    }

    // Calculate and print the remaining terms
    for (i = 3; i <= n; i++) {
        next = fib1 + fib2;
        printf(", %llu", next);
        fib1 = fib2;
        fib2 = next;
    }

    printf("\n");
    return 0;
}

