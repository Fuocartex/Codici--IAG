#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Smith.h"

int main() {
	int n=0, m=0; 
    	printf("Righe: ");
	scanf("%d", &m);
	printf("Colonne: ");
	scanf("%d", &n);
	int **A=input_null(A, m, n);
	int **D=input_sm(D, A, m, n);
	int **S=input_id(S, m);
	int **T=input_id(T, n);
	
	if (nullMatrix(A, m, n)) {
		printf("La matrice in esame è nulla\n");
		return 0;
	}
	
	SmithNormalForm(D, S, T, m, n);
	
	printf("\nmat A = ");
	printMatrix(A, m, n);
	printf("\nmat D = ");
	printMatrix(D, m, n);
	printf("\nmat S = ");
	printMatrix(S, m, m);
	printf("\nmat T = ");
	printMatrix(T, n, n);
	
	// verifichiamo che la funzione Smith funzioni correttamente vericando che S*A*T=D
	int **SA=input_null(SA, m, n);
	int **SAT=input_null(SAT, m, n);
	multiplyMatrices(S, A, SA, m, m, n);
	multiplyMatrices(SA, T, SAT, m, n, n); 
	printf("\nS*A*T = ");
	printMatrix(SAT, m, n);
	
	return 0; 
}
