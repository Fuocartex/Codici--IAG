#pragma once
#ifndef Matrix
	#define Matrix

	int** mul_matrix(int**, int, int, int**, int, int);
	void print_matrix(int**, int, int);
	//int det_matrix(int**, int);
	//double** gauss_iter(int**, int, int);
	//int** inverse_matrix(int**, int, int);

	int gcd(int a, int b) {
		while (b != 0) {
			int temp = b;
			b = a % b;
			a = temp;
		}
		return a > 0 ? a : -a; // Restituisce un valore positivo
	}

	// Funzione per calcolare il minimo comune multiplo (MCM)
	int lcm(int a, int b) {
		return (a / gcd(a, b)) * b;
	}

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

    // Funzione per calcolare l'inversa di una matrice intera
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
    }



	


#endif