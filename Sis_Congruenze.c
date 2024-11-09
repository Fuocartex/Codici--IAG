#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Smith.h"

int** input_sys (int**, int);
void print_sys(int**, int);
int* input_vec (int*, int);
int* input_vec_one (int*, int);
void solve_sys (int**, int*, int*, int*, int*, int);

int main() {
	int m;
	printf("Numero di equazioni nel sistema: ");
	scanf("%d", &m);
	printf("Dati del sistema (ad x=a mod b corrisponde l'input da tastiera 'a b'):\n"); 
	int **sys=input_sys(sys, m);
	print_sys(sys, m);
	
	// vettori necessari per la risoluzione del sistema 
	int *A=input_vec(A, m);
	int *B=input_vec_one(B, m);
	int *Alpha=input_vec(Alpha, m);
	int *Gamma=input_vec(Gamma, m);	
	solve_sys(sys, A, B, Alpha, Gamma, m);
	
	// troviamo la soluzione x
	int x=0, mod=1;
	for (int i=0; i<m; i++) {
		x=x+Gamma[i]*B[i]*Alpha[i];
		mod=mod*A[i];
	}
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
	
	int **D=input_null(D, m, 1);
	for (int i=0; i<m; i++) {
		D[i][0]=B[i]; // inizializziamo D a B
	}
	int **S=input_id(S, m);
	int **T=input_id(T, 1);
	SmithNormalForm(D, S, T, m, 1); // applichiamo Smith alla matrice/vettore D
	for (int i=0; i<m; i++)  {
		Gamma[i]=S[0][i]; // Gamma è la prima riga della matrice S prodotta da Smith(D) (corrisponde a D*S essendo D del tipo (1, 0, ..., 0)
	}
	return;
}
