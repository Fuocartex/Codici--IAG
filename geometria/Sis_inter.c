#include "..\Include\Matrix.h"

//risolvo un sistema di interi con n incognite e smith

int main() {

	int n = 0;
	printf("Quante incognite ha il sistema? n = ");
	scanf("%d", &n);

	//richiedo i dati per la matrice e il termine noto
	int** matrix = NULL;
	matrix = input_null(matrix, n, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("Inserisci l'elemento %d %d della matrice: ", i, j);
			scanf("%d", &matrix[i][j]);
		}
	}

	int** B = NULL;
	B = input_null(B, n, 1);
	for (int i = 0; i < n; i++) {
		printf("Inserisci il termine noto %d: ", i);
		scanf("%d", &B[i][0]);
	}

	//calcolo la forma normale di Smith della matrice
	int** S = NULL;
	S = input_id(S, n);
	int** S_inv = NULL;
	S_inv = input_id(S_inv, n);
	int** T = NULL;
	T = input_id(T, n);
	int** T_inv = NULL;
	T_inv = input_id(T_inv, n);
	int** D = NULL;
	D = input_null(D, n, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			D[i][j] = matrix[i][j];
		}
	}

	//risolvo Ax = B con la forma normale di Smith

	SmithNormalForm5mat(D, S, T, S_inv, T_inv, n, n);

	//SDT x = B

	//Y = DT^-1 B
	int** Y = NULL;
	Y = mul_matrix(S, n, n, B, n, 1);

	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 1; j++) {
			if (D[i][i] == 0) {
				Y[i][j] = 0;
			}
			else if (Y[i][j] % D[i][i] != 0) {
				printf("Il sistema non ha soluzione negli interi\n");
				return 1;
			}
			else {
				Y[i][j] = Y[i][j] / D[i][i];
			}
		}
	}

	print_matrix(Y, n, 1);

	//X = TY
	int** X = NULL;
	X = mul_matrix(T, n, n, Y, n, 1);

	printf("La soluzione del sistema con matrice A :\n");
	print_matrix(matrix, n, n);
	printf("e termine noto B : \n");
	print_matrix(B, n, 1);
	printf("e' X:\n");
	print_matrix(X, n, 1);

	return 0;
}