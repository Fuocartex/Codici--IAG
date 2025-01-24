#include "..\Include\Compl_Simpl.h"
#include "..\Include\Omo.h"


int main() {

	int size = 0, n = 2;
	int** matrix1 = NULL, ** matrix2 = NULL;
	printf("Inserisci la grandezza del simplesso piu' grande nel complesso: ");
	scanf("%d", &size);

	SimplicialComplex* complex = readComplex_file("simplicial_complex.txt");
	printComplex(complex, 3);

	if (!isSimplicial(complex, size)) {
		printf("Errore il simplesso non e' simpliciale inserire un nuovo simplesso\n");
		return 1;
	}

	printf("Inserisci il numero n di cui vuoi calcolare l'omologia H_n \n (n >= 0) = ");
	scanf("%d", &n);

	n++;
	//printf("n = %d", n);

	if (n == 1) {
		matrix2 = NULL;
		matrix1 = edge_Matrix(complex, n);
	}
	else if (n == size) {
		matrix1 = NULL;
		matrix2 = edge_Matrix(complex, n - 1);
	}
	else if (n >= size + 1) {
		matrix1 = NULL;
		matrix2 = NULL;
	}
	else {
		matrix1 = edge_Matrix(complex, n);
		matrix2 = edge_Matrix(complex, n - 1);
	}

	/*printf("Matrice M1 \n");
	print_matrix(matrix1, complex[n-1].size, complex[n].size);
	printf("Matrice M2 \n");
	print_matrix(matrix2, complex[n - 2].size, complex[n - 1].size);*/

	int h = dim_omologia(matrix1, complex[n - 1].size, complex[n].size, matrix2, complex[n - 2].size, complex[n - 1].size);

	/*int** M = NULL;
	M = input_null(M, 3, 2);
	int** M2 = NULL;
	M2 = input_null(M2, 1, 3);
	M[0][0] = 8;
	M[0][1] = -15;
	M[1][0] = -15;
	M[1][1] = 29;
	M[2][0] = -6;
	M[2][1] = 13;
	M2[0][0] = -9;
	M2[0][1] = -6;
	M2[0][2] = 3;
		
	int h = omologia(M, 3, 2, M2, 1, 3);*/

	printf("Il gruppo di omologia H_%d del complesso simpliciale inserito ha dimensione = %d\n", n - 1, h);

	return;
}