#include "..\Include\Compl_Simpl.h"
#include "..\Include\Omo.h"


int main() {

	int size = 0, n = 0;
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

	printf("Il gruppo di omologia H_%d e' %d\n", - 1, h);

	return;
}