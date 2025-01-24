#pragma once
#ifndef Omo
	#define Omo
	#include "..\Include\Matrix.h"
	#include "..\Include\Smith.h"


	//int** omologia(int**, int, int, int**, int, int);
	int compare(const void*, const void*);
	int dim_omologia(int**, int, int, int**, int, int);
	bool is_Complex(int**, int, int, int**, int, int);
	int rank_matrix_diag(int**, int, int);
	//int nn_zero_row(int**, int, int);

	int compare(const void* a, const void* b) {
		return (*(int*)a - *(int*)b);
	}

	bool is_Complex(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		if (col2 != row1) {
			return false;
		}
		int** temp = mul_matrix(matrix2, row2, col2, matrix1, row1, col1);
		//print_matrix(temp, row2, col1);
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

	int dim_omologia(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int h = 0, ck = 0, rk = 0, rk1 = 0;
		
		if (matrix1 && matrix2) {
			if (!is_Complex(matrix1, row1, col1, matrix2, row2, col2)) {
				return -1;
			}
		}
		int** D = NULL;
		int** S = NULL;
		int** T = NULL;

		if (matrix2) {

			D = input_null(D, row2, col2);
			S = input_id(S, row2);
			T = input_id(T, col2);

			for (int i = 0; i < row2; i++) {
				for (int j = 0; j < col2; j++) {
					D[i][j] = matrix2[i][j];
				}
			}


			ck = col2;

			SmithNormalForm(D, S, T, row2, col2);
			rk = rank_matrix_diag(D, row2, col2);
			printf("rk = %d\n", rk);

			free(D);
			free(S);
			free(T);
		}
		else
			rk = 0;

		if (matrix1) {
			D = input_null(D, row1, col1);
			S = input_id(S, row1);
			T = input_id(T, col1);

			for (int i = 0; i < row1; i++) {
				for (int j = 0; j < col1; j++) {
					D[i][j] = matrix1[i][j];
				}
			}

			ck = row1;

			SmithNormalForm(D, S, T, row1, col1);
			rk1 = rank_matrix_diag(D, row1, col1);
			printf("rk1 = %d\n", rk1);

			free(D);
			free(S);
			free(T);
		}
		else
			rk1 = 0;

		
		printf("ck = %d\n", ck);
		h = ck - rk - rk1;

		return h;
	}

	//da completare
	/*int** omologia(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int h = 0;
		if (!is_Complex(matrix1, row1, col1, matrix2, row2, col2)) {
			return -1;
		}
		
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

		printf("Matrice D\n");
		print_matrix(D, row2, col2);
		printf("Matrice T\n");
		print_matrix(T, col2, col2);
		printf("Matrice T1\n");
		print_matrix(T1, row2, row2);

		int r = rank_matrix_diag(D, row2, col2);

		int** T2 = NULL;
		T2 = input_null(T2, row2, col2-r);
		int** T2_inv = NULL;
		T2_inv = input_null(T2_inv, row2, col2 - r);

		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col2 - r; j++) {
				T2[i][j] = T[i][j + r];
				T2_inv[i][j] = T1[i][j + r];
			}
		}

		
		int** B = NULL;
		int** B_inv = NULL;
		int** C = NULL;
		B = input_null(B, col2 - r, col2 - r);
		B_inv = input_null(B_inv, col2 - r, col2 - r);
		C = input_null(C, col2 - r, col1);
		int* index = (int*)calloc(col2 - r, sizeof(int));
		for (int i = 0; i < col2 - r; i++) {
			int k = 0;
			for (k = 0; k < row2; k++) {
				if (T2[k][i] != 0) {
					index[i] = k;
					
					for (int j = i; j < col2 - r; j++) T2[k][j] = 0;
					break;
				}
			}
		}
		qsort(index, col2 - r, sizeof(int), compare);

		for (int i = 0; i < col2 - r; i++) {
			printf("%d ", index[i]);
			int k = index[i];
			for (int j = 0; j < col2 - r; j++) {
				B[i][j] = T[k][j + r];
				B_inv[i][j] = T1[k][j + r];
			}

			for( int j = 0; j < col1; j++){
				C[i][j] = matrix1[k][j];
			}
		}

		printf("Matrice B\n");
		print_matrix(B, col2 - r, col2 - r);
		printf("Matrice B_inv\n");
		print_matrix(B_inv, col2 - r, col2 - r);

		int** temp = mul_matrix(B_inv, col2 - r, col2 - r, B, col2 - r, col2 - r);

		
		printf("Matrice molt\n");
		print_matrix(temp, col2 - r, col2 - r);

		printf("Matrice C\n");
		print_matrix(C, col2 - r, col1);



		free(D);
		free(S);
		free(T);
		free(S1);
		free(T1);
		
		
		return h;
	}
	*/
	
#endif