#include "..\Include\Compl_Simpl.h"
#include "..\Include\Matrix.h"


int main() {

	int size = 0, n = 0;
	printf("Inserisci la grandezza del simplesso piu' grande nel complesso: ");
	scanf("%d", &size);

	SimplicialComplex* complex = readComplex(size);

	if (!isSimplicial(complex, size)) {
		printf("Errore il simplesso non e' simpliciale inserire un nuovo simplesso\n");
		return 1;
	}

	printf("Inserisci il numero n di cui vuoi calcolare la matrice di bordo n -> n-1\n n > 0 = ");
	scanf("%d", &n);
	if (!n) {
		printf("Errore n deve essere maggiore di 0\n");
		return 1;
	}

	int** matrix = edge_Matrix(complex, n);

	printf("M ");
	printMatrix(matrix, complex[n - 1].size, complex[n].size, 2);

	return;
}