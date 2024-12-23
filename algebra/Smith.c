#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\Include\Smith.h"

int main() {
	int n=0, m=0; 
    	printf("Righe: ");
	scanf("%d", &m);
	printf("Colonne: ");
	scanf("%d", &n);
	int **A=NULL;
	A=input_null(A, m, n);
	int **D=NULL;
	D=input_sm(D, A, m, n);
	int **S=NULL;
	S=input_id(S, m);
	int **T=NULL;
	T=input_id(T, n);
	int **S_inv=NULL;
	S_inv=input_id(S_inv, m);
	int **T_inv=NULL;
	T_inv=input_id(T_inv, n);
	
	if (nullMatrix(A, m, n)) {
		printf("La matrice in esame è nulla\n");
		return 0;
	}
	
	SmithNormalForm5mat(D, S, T, S_inv, T_inv, m, n);
	
	printf("\nmat A = ");
	printMatrix(A, m, n, 8);
	printf("\nmat D = ");
	printMatrix(D, m, n, 8);
	printf("\nmat S = ");
	printMatrix(S, m, m, 8);
	printf("\nmat T = ");
	printMatrix(T, n, n, 8);
	printf("\nmat S_inv = ");
	printMatrix(S_inv, m, m, 12);
	printf("\nmat T_inv = ");
	printMatrix(T_inv, n, n, 12);
	
	// verifichiamo che la funzione Smith funzioni correttamente vericando che S*A*T=D e S_inv*D*T_inv=A
	int **SA=NULL; 
	SA=input_null(SA, m, n);
	int **SAT=NULL;
	SAT=input_null(SAT, m, n);
	multiplyMatrices(S, A, SA, m, m, n);
	multiplyMatrices(SA, T, SAT, m, n, n); 
	printf("\nS*A*T = ");
	printMatrix(SAT, m, n, 8);
	
	int **SinvD=NULL; 
	SinvD=input_null(SinvD, m, n);
	int **SinvDTinv=NULL;
	SinvDTinv=input_null(SinvDTinv, m, n);
	multiplyMatrices(S_inv, D, SinvD, m, m, n);
	multiplyMatrices(SinvD, T_inv, SinvDTinv, m, n, n); 
	printf("\nS_inv*D*T_inv = ");
	printMatrix(SinvDTinv, m, n, 16);
	
	return 0; 
}
