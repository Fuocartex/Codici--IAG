#pragma once
#ifndef Omo
	#define Omo
	#include "..\Include\Matrix.h"
	
    // Questo file include funzioni per la manipolazione delle omologie

	//Definizioni funzioni
	int** omologia(int**, int, int, int**, int, int);
	int compare(const void*, const void*);
	
	int dim_omologia(int**, int, int, int**, int, int);
	bool is_Complex(int**, int, int, int**, int, int);

	//Implementazione

	//semplcie funzione per comparare due interi 
	int compare(const void* a, const void* b) {
		return (*(int*)a - *(int*)b);
	}
	
	//funzione per controllare se due matrici formano un complesso ovvero se la loro moltiplicazione è nulla
	//proprietà delle matrici di bordo
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

	
	//funzione per calcolare la dimensione dell'omologia n richiesta passo in input solo le due matrici di bordo
	int dim_omologia(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int h = 0, ck = 0, rk = 0, rk1 = 0;
		
		//controlliamo che le due matrici diano un complesso
		if (matrix1 && matrix2) {
			if (!is_Complex(matrix1, row1, col1, matrix2, row2, col2)) {
				return -1;
			}
		}

		int** D = NULL;
		int** S = NULL;
		int** T = NULL;

		//calcoliamo la dimensione dell'omologia come h = ck - rk - rk1 dove ck è il numero di n simplessi
		//rk è il rango della matrice di bordo n+1-esima e rk1 è il rango della matrice di bordo n-esima
		//controlliamo se esistono le matrici e quindi ne calcolo il rango utilizzando la matrice di smith 
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

	
	//Come rappresentare l'omologia in z-moduli
	int** omologia(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int** h = NULL;

		//controlliamo che le due matrici diano un compleso
		if (!is_Complex(matrix1, row1, col1, matrix2, row2, col2)) {
			printf("Errore: le matrici non formano un complesso\n");
			return NULL;
		}
		
		int** D = NULL;
		D = input_null(D, row2, col2);
		int** S = NULL;
		S = input_id(S, row2);
		int** T = NULL;
		T = input_id(T, col2);
		
		
		for (int i = 0; i < row2; i++) {
			for (int j = 0; j < col2; j++) {
				D[i][j] = matrix2[i][j];
			}
		}
		
		//calcoliamo la forma di smith della matrice An 
		SmithNormalForm(D, S, T, row2, col2);

		//troviamo il rango della matrice ed estraiamo una base del nucleo ovvero
		//le ultime r colonne della matrice T (SAT = D) che salvo in T2
		int r = rank_matrix_diag(D, row2, col2);

		int** T2 = NULL;
		T2 = input_null(T2, col2, col2-r);
		

		for (int i = 0; i < col2; i++) {
			for (int j = 0; j < col2 - r; j++) {
				T2[i][j] = T[i][j + r];
				
			}
		}
		
		//troviamo la forma di smith della base del nucleo appena trovata
		//per risolvere Bx = An+1 negli interi moltiplicando per l'inverso di ogni matrice che gia ho 
		//e fermandomi al rango della matrice B

		int** S1 = NULL;
		S1 = input_id(S1, col2);
		int** T1 = NULL;
		T1 = input_id(T1, col2 - r);
		int** D1 = NULL;
		D1 = input_null(D1, col2, col2 - r);

		for (int i = 0; i < col2; i++) {
			for (int j = 0; j < col2 - r; j++) {
				D1[i][j] = T2[i][j];
			}
		}

		SmithNormalForm(D1, S1, T1, col2, col2 - r);
		int** temp = NULL;

		//mi fermo a col2-r righe per s1*A
		temp = mul_matrix(S1, col2 - r, col2, matrix1, row1, col1);
		
		//D^-1 * S1 * A
		for (int i = 0; i < col2 - r; i++)
		{
			for (int j = 0; j < col1; j++)
			{
				temp[i][j] = temp[i][j] / D1[i][i];
			}
		}
		
		//T1 * D^-1 * S1 * A=x
		temp = mul_matrix(T1, col2 - r, col2 - r, temp, col2 - r, col1);
		

		free(S1);
		free(T1);
		free(D1);	

		//ora decompongo in z-moduli il risultato x=temp sempre attaverso smith e la matrice diagonale

		S1 = NULL;
		S1 = input_id(S1, col2 - r);
		T1 = NULL;
		T1 = input_id(T1, col2 - r);
		D1 = NULL;
		D1 = input_null(D1, col2 - r, col2 - r);

		for (int i = 0; i < col2 - r; i++) {
			for (int j = 0; j < col2 - r; j++) {
				D1[i][j] = temp[i][j];
			}
		}

		SmithNormalForm(D1, S1, T1, col2 - r, col2 - r);
		h = (int*)calloc(col2 - r - 1, sizeof(int));
		for (int i = 0; i < col2 - r ; i++) {
			h[i] = D1[i][i];
		}

		//printiamo il risultato ottenuto ovvero i valori di h
		printf("\nDecomposizione di h\n");
		printf("Z_%d", h[0]);
		for (int i = 1; i < col2 - r; i++) {
			printf(" + ");
			printf("Z_%d", h[i]);
		}

		/* non sempre trovo un minore invertibile negli itneri quindi questo codice non va bene
		int** B = NULL;
		int** B_inv = NULL;
		int** C = NULL;
		B = input_null(B, col2 - r, col2 - r);
		B_inv = input_null(B_inv, col2 - r, col2 - r);
		C = input_null(C, col2 - r, col1);

		//cerco un minore invertibile di T2
		//uso gauss per mettere la matrice in forma diagonale
		//poi prendo gli indici con elemnti non nulli e li salvo per estrarne un minore
		int* index = (int*)calloc(col2 - r, sizeof(int));
		gauss_rectangular(T2, col2, col2 - r);
		printf("Matrice T2 gauss\n");
		print_matrix(T2, col2, col2 - r);
		for (int i = 0; i < col2 - r; i++) {
			int k = 0;
			for (k = 0; k < col2; k++) {
				if (T2[k][i] != 0) {
					index[i] = k;
					
					for (int j = i; j < col2 - r; j++) T2[k][j] = 0;
					break;
				}
			}
		}
		//ordino gli indici per prendere le righe corrispondenti
		//sia su B che su A
		qsort(index, col2 - r, sizeof(int), compare);


		for (int i = 0; i < col2 - r; i++) {
			printf("i = %d \n", index[i]);
			int k = index[i];
			for (int j = 0; j < col2 - r; j++) {
				B[i][j] = T[k][j + r];
			}

			for( int j = 0; j < col1; j++){
				C[i][j] = matrix1[k][j];
			}
		}

		int iter = -1;

		//cerco il minore invertibile di B negli interi condizione sul determinante
		while (det_matrix_triangular(B, col2 - r) != 1 && det_matrix_triangular(B, col2 - r) != - 1) {
			iter++;
			if (iter == col2)
			{
				printf("Matrice non invertibile\n");
				return NULL;
			}
			else
			{
				
				while(T[iter][col2 - 1] == 0 && iter < col2)
					iter++;
			}

			for (int j = 0; j < col2 - r; j++) {
				B[col2 - r - 1][j] = T[iter][j + r];
			}

			for (int j = 0; j < col1; j++) {
				C[col2 - r - 1][j] = matrix1[iter][j];
			}	
		}

		

		//inverto la matrice
		B_inv = invert_matrix_integer(B, col2 - r);

		printf("Matrice B\n");
		print_matrix(B, col2 - r, col2 - r);
		printf("Matrice B_inv\n");
		print_matrix(B_inv, col2 - r, col2 - r);

		int** temp = mul_matrix(B_inv, col2 - r, col2 - r, C, col2 - r, col2 - r);

		
		printf("Matrice molt\n");
		print_matrix(temp, col2 - r, col2 - r);

		

		printf("Matrice C\n");
		print_matrix(C, col2 - r, col1);
		*/


		free(D);
		free(S);
		free(T);
		free(S1);
		free(T1);
		free(D1);
		free(T2);
				
		return h;
	}
	
	
#endif