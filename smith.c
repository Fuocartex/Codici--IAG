#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int** input (int**, int, int);
int** input_id (int**, int);
int**input_null(int**, int, int);
void printMatrix (int**, int, int);
bool nullMatrix (int**, int, int);
int** traspMatrix (int**, int**, int, int);
void switchZeros (int**, int**, int**, int**, int, int, int);
void switchSigns (int**, int**, int, int, int, int);
void swapRows (int**, int, int, int);
void addRows (int**, int, int, int, int);
void minUp (int**, int**, int, int, int, int);
void div_rem (int**, int**, int, int, int, int);
bool nullVector (int**, int, int);
void Smith (int**, int**, int**, int**, int**, int, int, int);
bool nextStep (int**, int, int);
int min (int, int);
bool boolSmith (int**, int, int);
void SmithNormalForm (int**, int**, int**, int, int);

int main() {
	int n=0, m=0; 
    	printf("Righe: ");
	scanf("%d", &m);
	printf("Colonne: ");
	scanf("%d", &n);
	int **A=input(A, m, n);
	int **S=input_id(S, m);
	int **T=input_id(T, n);
	printMatrix(A, m, n);
	printMatrix(S, m, m);
	printMatrix(T, n, n);
	
	if (nullMatrix(A, m, n)) {
		printf("La matrice in esame è nulla\n");
		return 0;
	}
	
	SmithNormalForm(A, S, T, m, n);
	
	printMatrix(A, m, n);
	printMatrix(S, m, m);
	printMatrix(T, n, n);
	
	return 0; 
}

int** input (int **mat, int m, int n) {
	mat=calloc(m, sizeof(int*));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(n, sizeof(int));
	}
	printf("Elementi della matrice:\n");
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			scanf("%d", &mat[i][j]);
		}
	}
	return mat;
}

int** input_id (int **mat, int m) {
	mat=calloc(m, sizeof(int *));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(m, sizeof(int));
	}
	for (int i=0; i<m; i++) {
		mat[i][i]=1;
	}
	return mat;
}
	
int** input_null(int **mat, int m, int n) {
	mat=calloc(m, sizeof(int*));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(n, sizeof(int));
	}
	return mat; 
}

void printMatrix (int **mat, int m, int n) {
	printf("\n");
	for (int i=0; i<m; i++) {
        	for (int j=0; j<n; j++) {
            		printf("%d ", mat[i][j]);
       		}
        	printf("\n");
    	}
    	return;
}

bool nullMatrix (int **mat, int m, int n) {
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			if (mat[i][j]!=0) {
				return false;
			}
		}
	}
	return true;
}

int** traspMatrix (int **mat, int **trasp, int m, int n) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			trasp[i][j]=mat[j][i];
		}
	}
	return trasp;
}

void switchZeros (int **A, int **T, int **At, int **Tt, int m, int n, int k) {
	bool var=true; 
	for (int i=k; i<m; i++) {
		if (A[i][k]!=0) {
			var=false; 
		}
	}
	if (var) {
		At=traspMatrix(A, At, m, n);
		Tt=traspMatrix(T, Tt, n, n);
		for (int j=k; j<(n-1); j++) {
			swapRows(At, j, j+1, m);
			swapRows(Tt, j, j+1, n);
		}
		A=traspMatrix(At, A, n, m);
		T=traspMatrix(Tt, T, n, n);
	}
	return; 
}		

void switchSigns (int **A, int **mat, int c, int m, int n, int k) {
	for (int i=k; i<m; i++) {
		if (A[i][k]<0) {
			for (int j=k; j<n; j++) {
				A[i][j]=-A[i][j];
			}
			for (int l=0; l<c; l++) {
				mat[i][l]=-mat[i][l];
			}
		}
	}
	return;
}

void swapRows (int **mat, int row1, int row2, int n) {
	int temp;
	for (int i=0; i<n; i++) {
        	temp=mat[row1][i];
        	mat[row1][i]=mat[row2][i];
        	mat[row2][i]=temp;
    	}
    	return;
}

void addRows (int **mat, int row1, int row2, int mult, int n) {
	for (int j=0; j<n; j++) {
		mat[row2][j]=mat[row2][j]-mult*mat[row1][j];
	}
	return;
}

void minUp (int **A, int **mat, int c, int m, int n, int k) {
	int min=A[k][k], r_min=k;
	for(int i=k; i<m; i++) {
		if (A[i][k]<min && A[i][k]!=0 || min==0) {
			min=A[i][k];
			r_min=i;
		}
	}
	swapRows(A, k, r_min, n);
	swapRows(mat, k, r_min, c); 
	return;
}

void div_rem (int **A, int **mat, int c, int m, int n, int k) {
	int quot=0;  
	for (int i=k+1; i<m; i++) {
		quot=A[i][k]/A[k][k];
		addRows(A, k, i, quot, n);
		addRows(mat, k, i, quot, c);		
	}
	return;
}

bool nullVector (int **mat, int m, int k) {
	for (int i=0; i<m; i++) {
		if (i!=k && mat[i][k]!=0) {
			return false; 
		}
	}
	return true; 
}

void Smith (int **A, int **S, int **T, int **At, int **Tt, int m, int n, int k) {
	while(!nullVector(A, m, k) || !nullVector(traspMatrix(A, At, m, n), n, k)) {
		switchZeros(A, T, At, Tt, m, n, k);
		while (!nullVector(A, m, k)) {
			switchSigns(A, S, m ,m, n, k);
			while (!nextStep(A, m, k)) {
				minUp(A, S, m, m, n, k);
				div_rem(A, S, m, m, n, k);
			}
		}
		At=traspMatrix(A, At, m, n);
		Tt=traspMatrix(T, Tt, n, n);
		while (!nullVector(At, n, k)) {
			switchSigns(At, Tt, n, n, m, k);
			while (!nextStep(At, n, k)) {
				minUp(At, Tt, n, n, m, k);
				div_rem(At, Tt, n, n, m, k);
			}
		}
		A=traspMatrix(At, A, n, m);
		T=traspMatrix(Tt, T, n, n);
	}
	switchSigns(A, S, m, m, n, k);
	return;
}
			
bool nextStep (int **A, int m, int k) {
	for (int i=k+1; i<m; i++) {
		if (A[i][k]!=0) {
			return false; 
		}
	}
	return true; 
}		

int min (int m, int n) {
	if (m<n) {
		return m; 
	} else {
		return n;
	}
}

bool boolSmith (int **A, int r, int k) {
	int quot=0, rem=0;
	for (int i=k+1; i<r; i++) {
		if (A[k][k]==0) {
			break; 
		}
		rem=A[i][i]%A[k][k];
		if(rem!=0) {
			return false; 
		}
	}
	return true;
}

void SmithNormalForm (int **A, int **S, int **T, int m, int n) { 
	int **At=input_null(At, n, m);
	int **Tt=input_null(Tt, n, n);
	
	int k=0, r=min(m, n);
	while (k!=r) {
		Smith(A, S, T, At, Tt, m, n, k);
		k++;
	}
	
	int it=0, iter=0;
	while (it!=r) {
		if (!boolSmith(A, r, it)) {
			for (int i=it+1; i<r; i++) {
				addRows(A, i, it, -1, n);
				addRows(S, i, it, -1, m);
			}
			iter=it; 
			while (iter!=r) {
				Smith(A, S, T, At, Tt, m, n, iter);
				iter++;
			}
		}
		it++;
		if (it==r) {
			switchSigns(A, S, m, m, n, r-1);
		}	
	}
	return;
}
