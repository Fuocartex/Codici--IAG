#include "..\Include\Complesso_da_1_simplessi.h"

int main () {
    int n,i,j;
    printf("Numero di vertici:\n");
    scanf("%d",&n);
    int** M = malloc(n*sizeof(int*));
    for (i=0; i<n; i++) M[i]= malloc(n*sizeof(int));
    printf("Inserisci la matrice di adiacenza:\n");
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            scanf("%d",&M[i][j]);
        }
    }

    SimplicialComplex* complex = complex_from_adjacency_matrix_complete(M,n);
    printComplex(complex,n);

    return 0;
}
