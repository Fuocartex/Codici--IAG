#pragma once
#ifndef Matrix
	#define Matrix
	#include "..\Include\Smith.h"

	// Funzioni per la manipolazione delle matrici
	int** mul_matrix(int**, int, int, int**, int, int);
	void print_matrix(int**, int, int);
	void gauss(float**, int);
	int det_matrix_triangular(int**, int);
    int rank_matrix_diag(int**, int, int);
	int** rank_base(int**, int, int, int*);
	int** link2matrix_same_row(int**, int, int, int**, int, int);
	int rank_matrix(int**, int, int);
	char* matrix_to_json(int**, int); // Funzione per convertire una matrice in una stringa JSON per passarla a python
	
	int** mul_matrix(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		if (col1 != row2) {
			return NULL;
		}
		int** result = NULL;
		result = (int**)calloc(row1, sizeof(int*));
		for (int i = 0; i < row1; i++) {
			result[i] = (int*)calloc(col2, sizeof(int));
		}
		for (int i = 0; i < row1; i++) {
			for (int j = 0; j < col2; j++) {
				for (int k = 0; k < col1; k++) {
					result[i][j] += matrix1[i][k] * matrix2[k][j];
				}
			}
		}
		return result;
	}

	void print_matrix(int** matrix, int row, int col) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
		return;
	}

    

    //Funzione per calcolare l'inversa di una matrice intera
    int** invert_matrix_integer(int** matrix, int size) {
        int** A = NULL;
        A = input_null(A, size, size);
        int** inv = NULL;
        inv = input_null(inv, size, size);
        if (!A || !inv) {
            free(A);
            free(inv);
            return NULL;
        }

        // Copia della matrice originale in A e inizializzazione della matrice identità in inv
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                A[i][j] = matrix[i][j];
                inv[i][j] = (i == j) ? 1 : 0;
            }
        }


        // Eliminazione di Gauss-Jordan
        for (int i = 0; i < size; i++) {
            // Se il pivot A[i][i] è 0, cerca una riga sottostante da scambiare
            if (A[i][i] == 0) {
                int swapRow = i + 1;
                while (swapRow < size && A[swapRow][i] == 0)
                    swapRow++;
                if (swapRow == size) {  // Matrice singolare
                    free(A);
                    free(inv);
                    return NULL;
                }
                swapRows(A, i, swapRow, size);
                swapRows(inv, i, swapRow, size);
            }

            // Per ottenere l'inversa intera, il pivot deve essere 1 o -1
            if (A[i][i] != 1 && A[i][i] != -1) {
                // La matrice non è unimodulare: non possiamo ottenere un'inversa intera
                free(A);
                free(inv);
                return NULL;
            }

            // Se il pivot è -1, moltiplica l'intera riga per -1 per renderlo 1
            if (A[i][i] == -1) {
                for (int j = 0; j < size; j++) {
                    A[i][j] = -A[i][j];
                    inv[i][j] = -inv[i][j];
                }
            }

            // Elimina tutti gli altri elementi nella colonna i
            for (int k = 0; k < size; k++) {
                if (k != i && A[k][i] != 0) {
                    int factor = A[k][i];  // in una matrice unimodulare dovrebbe essere ±1
                    for (int j = 0; j < size; j++) {
                        A[k][j] -= factor * A[i][j];
                        inv[k][j] -= factor * inv[i][j];
                    }
                }
            }
        }

        free(A);
        return inv;
    }

    void gauss(float** matrice, int n) {
        for (int i = 0; i < n; i++) {
            // Pivot
            if (matrice[i][i] == 0) {
                for (int k = i + 1; k < n; k++) {
                    if (matrice[k][i] != 0) {
                        // Scambia righe
                        for (int j = 0; j < n; j++) {
                            float temp = matrice[i][j];
                            matrice[i][j] = matrice[k][j];
                            matrice[k][j] = temp;
                        }
                        break;
                    }
                }
            }

            // Normalizza la riga pivot
            float pivot = matrice[i][i];
            for (int j = 0; j < n; j++) {
                matrice[i][j] /= pivot;
            }

            // Eliminazione verso il basso
            for (int k = i + 1; k < n; k++) {
                float coeff = matrice[k][i];
                for (int j = 0; j < n; j++) {
                    matrice[k][j] -= coeff * matrice[i][j];
                }
            }
        }
    }

	void gauss_rectangular(int** matrice, int row, int col) {
		int r = 0;
		for (int i = 0; i < col; i++) {
			
			// Pivot
            if (matrice[i][i] == 0) {
                for (int k = i + 1; k < row; k++) {
                    if (matrice[k][i] != 0) {
                        printf("matrice[%d][%d] = %d\n", k, i, matrice[k][i]);
                        // Non scambio le righe perchè mi serve sapere quali sono le righe non nulle
                        r = k;
                        break;
                    }
                    else
                        return;
                }
            }
            else r = i;
			// Normalizza la riga pivot
			int pivot = matrice[r][i];
			printf("pivot = %d\n", pivot);
			
			
			// Eliminazione verso il basso
			for (int k = 0; k < row; k++) {
				if (k == r) continue;
				double coeff = (double)matrice[k][i]/pivot;
                printf("matrice[%d][%d] = %d\n", k, i, matrice[k][i]);
				printf("coeff = %lf\n", coeff);
				for (int j = i; j < col; j++) {
					matrice[k][j] -= coeff * matrice[r][j];
				}
			}
		}
	}

	
	int det_matrix_triangular(int** matrice, int n) {
		int det = 1;
		for (int i = 0; i < n; i++) {
			det *= matrice[i][i];
		}
		return det;
	}

    int rank_matrix_diag(int** matrix, int row, int col) {
        int r = 0;
        for (int i = 0; i < my_min(row, col); i++) {
            if (matrix[i][i] != 0)
                r++;
        }
        return r;
    }

    int** rank_base(int** M, int r, int c, int *n) {
        int** D = NULL;
        int** S = NULL;
        int** T = NULL;

        D = input_null(D, r, c);
        S = input_id(S, r);
        T = input_id(T, c);

        for (int i = 0; i < r; i++) { 
            for (int j = 0; j < c; j++) {
                D[i][j] = M[i][j];
            }
        }

               
        SmithNormalForm(D, S, T, r, c);
        int rk = rank_matrix_diag(D, r, c);

		int** B = NULL;
		if (c - rk == 0) {
			free(D);
			free(S);
			free(T);
			return B;
		}
        B = input_null(B, c, c - rk);
		for (int i = 0; i < c; i++) {
			for (int j = 0; j < c - rk; j++) {
				B[i][j] = T[i][j + rk];
			}
		}

		print_matrix(B, c, c - rk);
       
		free(D);
		free(S);
		free(T);
		*n = c - rk;
		return B;
    }

	int** link2matrix_same_row(int** matrix1, int row1, int col1, int** matrix2, int row2, int col2) {
		int** result = NULL;
		if (row1 != row2) {
			printf("Errore: Le matrici non hanno lo stesso numero di righe.\n");
			return NULL;
		}
		result = input_null(result, row1, col1 + col2);
		for (int i = 0; i < row1; i++) {
			for (int j = 0; j < col1; j++) {
				result[i][j] = matrix1[i][j];
			}
			for (int j = 0; j < col2; j++) {
				result[i][j + col1] = matrix2[i][j];
			}
		}
		return result;
	}

	int rank_matrix(int** M, int r, int c) {
        int** D = NULL;
        int** S = NULL;
        int** T = NULL;

        D = input_null(D, r, c);
        S = input_id(S, r);
        T = input_id(T, c);

        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                D[i][j] = M[i][j];
            }
        }

		

        SmithNormalForm(D, S, T, r, c);
        int rk = rank_matrix_diag(D, r, c);

        free(D);    
		free(S);
		free(T);

		return rk;
	}

    char* matrix_to_json(int** matrix, int n) {
        // Calcoliamo una dimensione massima per il buffer.
        // Per ogni numero, assumiamo al massimo 12 caratteri (inclusi segno, cifra e separatore).
        int buffer_size = n * (n * 12 + 2) + 2;
        char* buffer = (char*)malloc(buffer_size);
        if (!buffer) {
            perror("Errore nell'allocazione della memoria");
            exit(EXIT_FAILURE);
        }
        int pos = 0;
        pos += snprintf(buffer + pos, buffer_size - pos, "[");
        for (int i = 0; i < n; i++) {
            pos += snprintf(buffer + pos, buffer_size - pos, "[");
            for (int j = 0; j < n; j++) {
                pos += snprintf(buffer + pos, buffer_size - pos, "%d", matrix[i][j]);
                if (j < n - 1) {
                    pos += snprintf(buffer + pos, buffer_size - pos, ",");
                }
            }
            pos += snprintf(buffer + pos, buffer_size - pos, "]");
            if (i < n - 1) {
                pos += snprintf(buffer + pos, buffer_size - pos, ",");
            }
        }
        pos += snprintf(buffer + pos, buffer_size - pos, "]");
        return buffer;
    }

#endif