#pragma once
#ifndef Matrix
	#define Matrix
	#include "..\Include\Smith.h"

	// Funzioni per la manipolazione delle matrici
	int** mul_matrix(int**, int, int, int**, int, int);
	void print_matrix(int**, int, int);
	void gauss(float**, int);
	float det_matrix_triangular(float**, int);
    int rank_matrix_diag(int**, int, int);
	int** rank_base(int**, int, int);
	int** link2matrix_same_row(int**, int, int, int**, int, int);
	int rank_matrix(int**, int, int);
	
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

    /* Funzione per calcolare l'inversa di una matrice intera
    int** invert_matrix_integer(int** matrix, int size) {
        // Costruzione della matrice estesa [A | I]
		int** augmented = (int**)calloc(size, sizeof(int*));
        int** inverse = (int**)calloc(size, sizeof(int*));
		for (int i = 0; i < size; i++) {
			augmented[i] = (int*)calloc(2 * size, sizeof(int));
			inverse[i] = (int*)calloc(size, sizeof(int));
		}
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                augmented[i][j] = matrix[i][j]; // Parte della matrice originale
                augmented[i][j + size] = (i == j) ? 1 : 0; // Parte della matrice identità
            }
        }

        // Riduzione a scala
        for (int i = 0; i < size; i++) {
            // Trova il pivot non nullo
            if (augmented[i][i] == 0) {
                // Cerca un'altra riga da scambiare
                int swap_row = -1;
                for (int k = i + 1; k < size; k++) {
                    if (augmented[k][i] != 0) {
                        swap_row = k;
                        break;
                    }
                }
                if (swap_row == -1) {
                    printf("Errore: La matrice non è invertibile.\n");
                    return 0;
                }
                // Scambia le righe
                for (int j = 0; j < 2 * size; j++) {
                    int temp = augmented[i][j];
                    augmented[i][j] = augmented[swap_row][j];
                    augmented[swap_row][j] = temp;
                }
            }

            // Normalizza la riga corrente usando l'MCD
            for (int k = 0; k < size; k++) {
                if (k != i) {
                    int factor = augmented[k][i] / gcd(augmented[i][i], augmented[k][i]);
                    for (int j = 0; j < 2 * size; j++) {
                        augmented[k][j] -= factor * augmented[i][j];
                    }
                }
            }
        }

        // Controlla se il determinante è ±1
        for (int i = 0; i < size; i++) {
            if (augmented[i][i] != 1 && augmented[i][i] != -1) {
                printf("Errore: La matrice non è unimodulare, quindi non ha un'inversa intera.\n");
                return 0;
            }
        }

        // Estrai la matrice inversa dalla parte destra
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                inverse[i][j] = augmented[i][j + size];
            }
        }

        return inverse; // Successo
    }*/

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
						// Non scambio le righe perchè mi serve sapere quali sono le righe non nulle
						r = k;
						break;
					}
				}
			}
			// Normalizza la riga pivot
			int pivot = matrice[r][i];
			
			// Eliminazione verso il basso
			for (int k = 0; k < row && k != r; k++) {
				int coeff = matrice[k][i]/pivot;
				for (int j = i; j < col; j++) {
					matrice[k][j] -= coeff * matrice[r][j];
				}
			}
		}
	}

	
	float det_matrix_triangular(float** matrice, int n) {
		float det = 1;
		for (int i = 0; i < n; i++) {
			det *= matrice[i][i];
		}
		return det;
	}

    int rank_matrix_diag(int** matrix, int row, int col) {
        int r = 0;
        for (int i = 0; i < min(row, col); i++) {
            if (matrix[i][i] != 0)
                r++;
        }
        return r;
    }

    int** rank_base(int** M, int r, int c) {
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
        B = input_null(B, c, c - rk);
		for (int i = 0; i < c; i++) {
			for (int j = 0; j < c - rk; j++) {
				B[i][j] = T[i][j + rk];
			}
		}

       
		free(D);
		free(S);
		free(T);
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

#endif