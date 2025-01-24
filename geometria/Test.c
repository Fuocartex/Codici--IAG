#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

#define SIZE 5

int main() {
    // Matrice unimodulare
    int** matrix1 = (int**)malloc(5 * sizeof(int*));
    int** matrix2 = (int**)malloc(5 * sizeof(int*));
    int** result = (int**)malloc(5 * sizeof(int*));
    int** result2 = (int**)malloc(5 * sizeof(int*));
    for (int i = 0; i < SIZE; i++) {
        matrix1[i] = (int*)malloc(5 * sizeof(int));
        matrix2[i] = (int*)malloc(5 * sizeof(int));
		result2[i] = (int*)malloc(5 * sizeof(int));
		result[i] = (int*)malloc(5 * sizeof(int));
    }

    int temp1[SIZE][SIZE] = {
        {1, 2, 0, -1, 0},
        {0, 1, 1, 0, -1},
        {-1, -1, 2, 1, 0},
        {0, 0, 1, 1, 2},
        {1, 0, -1, -2, 3}
    };

    int temp2[SIZE][SIZE] = {
      {4,  2,  0,  0,  1},
      {2,  5,  1,  0,  0},
      {0,  1,  3,  1,  0},
      {0,  0,  1,  6,  2},
      {1,  0,  0,  2,  7}
    };
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix1[i][j] = temp1[i][j];
            matrix2[i][j] = temp2[i][j];
        }
    }

    result2 = invert_matrix_integer(matrix1, SIZE);
	printf("\n");
	print_matrix(result2, SIZE, SIZE);

    result = mul_matrix(matrix1, SIZE, SIZE, result2, SIZE, SIZE);
    print_matrix(result, SIZE, SIZE);

    // Libera la memoria allocata
    /*for (int i = 0; i < SIZE; i++) {
        free(matrix1[i]);
        free(matrix2[i]);
    }
    free(matrix1);
	free(matrix2);*/

    return 0;
}