#pragma once
#ifndef Bezout
#define Bezout

int* imputBezout (int, int*);
void bezout (int, int*, int**, int**, int**);
void printBezout (int, int*, int**, int**);
long int MCD (long int, long int);

// imput vettore elem contenente i numeri a cui verrà applicato Bezout
int* imputBezout (int n, int *elem) {
	elem=calloc(n, sizeof(int));
	for (int i=0; i<n; i++) {
		scanf("%d", &elem[i]);
	}
	return elem; 
}

// Bezout(a,b): a*x+b*y=MCD
void bezout (int n, int *elem, int **MCD, int **S, int **T) {
	for (int i=0; i<n; i++) { // poniamo il vettore MCD uguale ad elem per poi applicargli l'algoritmo di Smith
		MCD[i][0]=elem[i]; 
	}
	SmithNormalForm(MCD, S, T, n, 1); // applico Smith ad alla matrice MCD così da trovare il massimo comune divisore (in posizione MCD[1][0]) e i coefficienti x,y,... per cui moltiplicare gli elementi (prima riga della matrice S
	return; 
}

// stampa 
void printBezout (int n, int *elem, int **MCD, int **S) {
	printf("Bezout: ");
	for (int i=0; i<n; i++) {
		if (i!=0) {
			printf(" + ");
		}
		printf("%d*", elem[i]);
		if (S[0][i]<0) {
			printf("(%d)", S[0][i]);
		} else {
			printf("%d", S[0][i]);
		}
		if (i==(n-1)) {
			printf(" = %d\n", MCD[0][0]);
		}
	}
	return; 
}

// funzione compatta per MCD tra due numeri 
int MCD (int a, int b) {
	int **MCD=NULL;
	MCD=input_null(MCD, 2, 1);
	MCD[0][0]=a; 
	MCD[1][0]=b;
	int **S=NULL; 
	S=input_id(S, 2);
	int **T=NULL; 
	T=input_id(T, 1);
	SmithNormalForm(MCD, S, T, 2, 1);
	int mcd=MCD[0][0];
	return mcd;
}

#endif
