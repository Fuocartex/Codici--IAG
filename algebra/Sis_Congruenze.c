#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\Include\Smith.h"
#include "..\Include\Bezout.h"

int** input_sys (int**, int);
void print_sys (int**, int);
int* input_vec (int*, int);
int* input_vec_one (int*, int);
void solve_sys (int**, int*, int*, int*, int*, int);
int adjust_sol (int, int);

int main() {
	int m;
	printf("Numero di equazioni nel sistema: ");
	scanf("%d", &m);
	printf("Dati del sistema (ad x=a mod b corrisponde l'input da tastiera 'a b'):\n"); 
	int **sys=NULL;
	sys=input_sys(sys, m);
	print_sys(sys, m);
	
	// vettori necessari per la risoluzione del sistema 
	int *A=NULL; 
	A=input_vec(A, m);
	int *B=NULL; 
	B=input_vec_one(B, m);
	int *Alpha=NULL; 
	Alpha=input_vec(Alpha, m);
	int *Gamma=NULL; 
	Gamma=input_vec(Gamma, m);	
	solve_sys(sys, A, B, Alpha, Gamma, m);
	
	// troviamo la soluzione x
	int x=0; 
	int mod=1;
	for (int i=0; i<m; i++) {
		x=x+Gamma[i]*B[i]*Alpha[i];
		mod=mod*A[i];
	}
	// aggiustiamo la soluzione x rendendola della forma x=a mod b con 0<=a<b
	x=adjust_sol(x, mod);
	printf ("Soluzione: x=%d mod %d\n", x, mod);
	
	return 0; 
}

// inserire la matrice dei dati del sistema (matrice m*2)
int** input_sys (int** mat, int m) {
	mat=calloc(m, sizeof(int*));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(2, sizeof(int));
	}
	for (int i=0; i<m; i++) {
		for (int j=0; j<2; j++) {
			scanf("%d", &mat[i][j]);
		}
	}
	return mat; 
}

// stampare il sistema
void print_sys (int** sys, int m) {
	printf("\nSistema: ");
	for (int i=0; i<m; i++) {
		printf("x=%d mod %d\n", sys[i][0], sys[i][1]);
		if (i!=(m-1)) {
			printf("\t ");
		}
	}
	printf("\n");
	return; 
}

// inizializzare un vettore nullo di m componenti 
int* input_vec (int *vec, int m) {
	vec=calloc(m, sizeof(int)); 
	return vec;
}

// inizializzare un vettore di 1 di m componenti 
int* input_vec_one (int* vec, int m) {
	vec=calloc(m, sizeof(int)); 
	for (int i=0; i<m; i++) {
		vec[i]=1; 
	}
	return vec; 
}

// creare i vettori che serviranno poi nella risoluzione del sistema 
void solve_sys (int **sys, int *A, int *B, int *Alpha, int *Gamma, int m) {
	for (int i=0; i<m; i++) { // prese congruenze del tipo x=a mod alpha 
		A[i]=sys[i][1]; // A è il vettore delle a
		for (int j=0; j<m; j++) {
			if (j!=i) {
				B[i]=B[i]*sys[j][1]; // la componente B[i] è il prodotto delle A[j] per j!=i
			}
		}
		Alpha[i]=sys[i][0]; // Alpha è vettore delle alpha 
	}
	
	int **D=NULL;
	D=input_null(D, m, 1);
	
	
	for (int i=0; i<m; i++) {
		D[i][0]=B[i]; // inizializziamo D a B
	}
	
	int **S=NULL; 
	S=input_id(S, m);
	int **T=NULL; 
	T=input_id(T, 1);
	bezout (m, B, D, S, T);
	
	for (int i=0; i<m; i++)  {
		Gamma[i]=S[0][i]; // Gamma corrisponde ai coefficienti di Bezout, ossia è la prima riga della matrice S
	}
	return;
}

// aggiustiamo la soluzione x rendendola della forma x=a mod b con 0<=a<b
int adjust_sol (int x, int mod) {
	if (abs(x)>=mod) { // se abs(x)>=mod togliamo ad x i multipli di mod perchè a*x=x mod a
		int temp=0;  
		temp=x/mod;
		x=x-temp*mod;
	}
	if (x<0) { // se x<0 prendiamo la sua classe equivalente positiva
		x=mod+x; 
	}
	return x;
}	
