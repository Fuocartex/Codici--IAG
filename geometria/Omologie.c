#include "..\Include\Compl_Simpl.h"
#include "..\Include\Omo.h"


int main() {

	/*int size = 0, n = 0;
	printf("Inserisci la grandezza del simplesso piu' grande nel complesso: ");
	scanf("%d", &size);

	SimplicialComplex* complex = readComplex(size);

	if (!isSimplicial(complex, size)) {
		printf("Errore il simplesso non e' simpliciale inserire un nuovo simplesso\n");
		return 1;
	}

	printf("Inserisci il numero n di cui vuoi calcolare la matrice di bordo n -> n-1\n n > 1 = ");
	scanf("%d", &n);
	if (n <= 1) {
		printf("Errore n deve essere maggiore di 1\n");
		return 1;
	}

	int** matrix1 = edge_Matrix(complex, n);
	int** matrix2 = edge_Matrix(complex, n - 1);
	
	

	printf("M1 \n");
	print_matrix(matrix1, complex[n-1].size, complex[n].size);
	printf("M2 \n");
	print_matrix(matrix2, complex[n - 2].size, complex[n - 1].size);

	int h = omologia(matrix1, complex[n - 1].size, complex[n].size, matrix2, complex[n - 2].size, complex[n - 1].size);
	*/

	int m1[3][2] = {
		{8, -13},
		{-15, 29},
		{-6, 13}
	};

	int m2[1][3] = {
		{-9, -6, 3}
	};

	int** m11 = (int**)calloc(3, sizeof(int*));
	for (int i = 0; i < 3; i++) {
		m11[i] = (int*)calloc(2, sizeof(int));
	}
	int** m22 = (int**)calloc(1, sizeof(int*));	
	for (int i = 0; i < 1; i++) {
		m22[i] = (int*)calloc(3, sizeof(int));
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			m11[i][j] = m1[i][j];
		}
	}

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 3; j++) {
			m22[i][j] = m2[i][j];
		}
	}

	int h = omologia(m11, 3, 2, m22, 1, 3);
	printf("Il gruppo di omologia H_%d e' %d\n", - 1, h);

	return;
}