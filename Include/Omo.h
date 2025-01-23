#pragma once
#ifndef Omo
	#define Omo
	#include "..\Include\Matrix.h"
	#include "..\Include\Smith.h"


	int** omologia(int**, int, int, int**, int, int);
	bool is_Complex(int**, int, int, int**, int, int);
	int rank_matrix_diag(int**, int, int);
	//int nn_zero_row(int**, int, int);

	bool is_Complex(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		if (col2 != row1) {
			return false;
		}
		int** temp = mul_matrix(matrix2, row2, col2, matrix1, row1, col1);
		print_matrix(temp, row2, col1);
		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col1; j++) {
				if (temp[i][j] != 0) {
					return false;
				}
			}
		}
		return true;
	}

	int rank_matrix_diag(int** matrix, int row, int col) {
		int r = 0;
		for (int i = 0; i < min(row, col); i++) {
			if (matrix[i][i] != 0)
				r++;
		}
		return r;
	}

	int** omologia(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int h = 0;
		if (!is_Complex(matrix1, row1, col1, matrix2, row2, col2)) {
			return -1;
		}
		/*int** S, ** D, ** T, ** S1, ** T1;
		D = (int**)calloc(row2, sizeof(int*));
		S = (int**)calloc(row2, sizeof(int*));
		T = (int**)calloc(col2, sizeof(int*));
		S1 = (int**)calloc(row2, sizeof(int*));
		T1 = (int**)calloc(col2, sizeof(int*));
		for (int i = 0; i < col2; i++) {
			
			T[i] = (int*)calloc(col2, sizeof(int));
			T1[i] = (int*)calloc(col2, sizeof(int));
		}
		for (int i = 0; i < row2; i++) {
			S[i] = (int*)calloc(row2, sizeof(int));
			S1[i] = (int*)calloc(row2, sizeof(int));
			D[i] = (int*)calloc(col2, sizeof(int));
		}*/
		int** D = NULL;
		D = input_null(D, row2, col2);
		int** S = NULL;
		S = input_id(S, row2);
		int** T = NULL;
		T = input_id(T, col2);
		int** S1 = NULL;
		S1 = input_id(S1, row2);
		int** T1 = NULL;
		T1 = input_id(T1, col2);
		
		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col2; j++) {
				D[i][j] = matrix2[i][j];
			}
		}
		
		SmithNormalForm5mat(D, S, T, S1, T1, row2, col2);

		/*printf("Matrice D\n");
		print_matrix(D, row2, col2);*/
		printf("Matrice T\n");
		print_matrix(T, col2, col2);
		/*printf("Matrice T1\n");
		print_matrix(T1, row2, row2);*/

		int r = rank_matrix_diag(D, row2, col2);

		int** T2 = NULL;
		T2 = input_null(D, row2, col2-r);

		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col2 - r; j++) {
				T2[i][j] = T1[i][j + r];
			}
		}

		int** temp = mul_matrix(T, row2, row2, matrix1, row1, col1);

		/*int** temp = mul_matrix(T, col2, col2, matrix1, row1, col1);
		*/

		printf("Matrice molt\n");
		print_matrix(temp, col2, col1);

		free(D);
		free(S);
		free(T);
		free(S1);
		free(T1);
		
		/*D = input_null(D, row1, col1);
		S = input_id(S, row1);
		T = input_id(T, col1);
		for (int i = 0; i < row1; i++) {
			for (int j = 0; j < col1; j++) {
				D[i][j] = temp[i][j];
			}
		}

		SmithNormalForm(D, S, T, row1, col1);
		printf("Matrice D\n");
		print_matrix(D, row1, col1);*/
		return h;
	}

	
#endif