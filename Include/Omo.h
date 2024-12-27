#pragma once
#ifndef Omo
	#define Omo
	#include "..\Include\Matrix.h"
	#include "..\Include\Smith.h"


	int omologia(int**, int, int, int**, int, int);
	bool is_Complex(int**, int, int, int**, int, int);
	//int nn_zero_row(int**, int, int);

	bool is_Complex(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		if (col2 != row1) {
			return false;
		}
		int** temp = mul_matrix(matrix2, row2, col2, matrix1, row1, col1);
		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col1; j++) {
				if (temp[i][j] != 0) {
					return false;
				}
			}
		}
		return true;
	}

	int omologia(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int h = 0;
		if (!is_Complex(matrix1, row1, col1, matrix2, row2, col2)) {
			return -1;
		}
		int** S, ** D, ** T;
		D = (int**)calloc(col2, sizeof(int*));
		S = (int**)calloc(row2, sizeof(int*));
		T = (int**)calloc(col2, sizeof(int*));
		for (int i = 0; i < col2; i++) {
			D[i] = (int*)calloc(row2, sizeof(int));
			T[i] = (int*)calloc(col2, sizeof(int));
		}
		for (int i = 0; i < row2; i++) {
			S[i] = (int*)calloc(row2, sizeof(int));
		}
		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col2; j++) {
				D[i][j] = matrix2[i][j];
			}
		}
		
		SmithNormalForm(D, S, T, row2, col2);

		printf("Matrice D\n");
		print_matrix(D, row2, col2);
		printf("Matrice T\n");
		print_matrix(T, col2, col2);
		printf("Matrice S\n");
		print_matrix(S, row2, row2);

		//int** temp = mul_matrix(T, col2, col2, matrix1, row1, col1);
		

		printf("Matrice molt\n");
		//print_matrix(temp, col2, col1);
		return h;
	}

	
#endif