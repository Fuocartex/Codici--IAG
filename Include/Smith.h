#pragma once
#ifndef Smith
#define Smith

int** input_sm (int**, int**, int, int);
int** input_id (int**, int);
int** input_null (int**, int, int);
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
void Smith_fst (int**, int**, int**, int**, int**, int, int, int);
int min (int, int);
bool boolSmith (int**, int, int);
void SmithNormalForm (int**, int**, int**, int, int);
void multiplyMatrices(int**, int**, int**, int, int, int);
void Smith_D (int**, int, int); 

// inizializzazione matrice i cui elementi sono scelti dall'utente  
int** input_sm (int **mat, int **temp, int m, int n) {
	mat=calloc(m, sizeof(int*));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(n, sizeof(int));
	}
	printf("Elementi della matrice:\n");
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			scanf("%d", &mat[i][j]);
			temp[i][j]=mat[i][j];
		}
	}
	return mat;
}

// inizializzazione matrice identità
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
	
// inizializzazione matrice nulla
int** input_null(int **mat, int m, int n) {
	mat=calloc(m, sizeof(int*));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(n, sizeof(int));
	}
	return mat; 
}

// stampare la matrice
void printMatrix (int **mat, int m, int n) {
	for (int i=0; i<m; i++) {
        	for (int j=0; j<n; j++) {
        		if (mat[i][j]<0) {
				printf("%d ", mat[i][j]);
			} else {
				printf(" %d ", mat[i][j]);
			}
       		}
        	printf("\n");
        	if (i!=(m-1)) {
        		printf("\t");
        	}
    	}
    	return;
}

// verificare che la matrice non sia nulla
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

// trasporre una matrice
int** traspMatrix (int **mat, int **trasp, int m, int n) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			trasp[i][j]=mat[j][i];
		}
	}
	return trasp;
}

// spostare un'eventuale colla nulla in fondo alla matrice
void switchZeros (int **A, int **T, int **At, int **Tt, int m, int n, int k) {
	bool var=true; 
	for (int i=k; i<m; i++) {
		if (A[i][k]!=0) {
			var=false; 
		}
	}
	if (var) {
		At=traspMatrix(A, At, m, n); // swapRows lavora sulle righe, quindi per invertire le colonne bisogna lavorare sulla trasposta 
		Tt=traspMatrix(T, Tt, n, n); // invertire le colonne è un'operazione che lavora su T
		for (int j=k; j<(n-1); j++) {
			swapRows(At, j, j+1, m); // inverte le righe j e j+1 di una matrice 
			swapRows(Tt, j, j+1, n);
		}
		A=traspMatrix(At, A, n, m); // trasposta di trasposta per tornare nella forma originaria 
		T=traspMatrix(Tt, T, n, n);
	}
	return; 
}		

// invertire i segni di una riga della matrice
void switchSigns (int **A, int **mat, int c, int m, int n, int k) {
	for (int i=k; i<m; i++) {
		if (A[i][k]<0) {
			for (int j=k; j<n; j++) {
				A[i][j]=-A[i][j]; // matrice A
			}
			for (int l=0; l<c; l++) {
				mat[i][l]=-mat[i][l]; // matrice S o T a seconda dei casi  
			}
		}
	}
	return;
}

// scambaire le righe row1 e row2 di una matrice 
void swapRows (int **mat, int row1, int row2, int n) {
	int temp;
	for (int i=0; i<n; i++) {
        	temp=mat[row1][i];
        	mat[row1][i]=mat[row2][i];
        	mat[row2][i]=temp;
    	}
    	return;
}

// aggiungere alla riga row2 un multiplo (mult) della riga row1
void addRows (int **mat, int row1, int row2, int mult, int n) {
	for (int j=0; j<n; j++) {
		mat[row2][j]=mat[row2][j]-mult*mat[row1][j];
	}
	return;
}

// portare il minimo della colonna k di una matrice in posizione [k][k]
void minUp (int **A, int **mat, int c, int m, int n, int k) {
	int min=A[k][k], r_min=k;
	for(int i=k; i<m; i++) {
		if (A[i][k]<min && A[i][k]!=0 || min==0) { 
			min=A[i][k]; // individuare il minimo 
			r_min=i; // individuare la riga del minimo
		}
	}
	swapRows(A, k, r_min, n); // invertire la riga k e r_min della matrice per portare il minimo in alto 
	swapRows(mat, k, r_min, c); // stesso lavoro anche su S o T a seconda dei casi 
	return;
}

// divisione con i resti 
void div_rem (int **A, int **mat, int c, int m, int n, int k) {
	int quot=0;  
	for (int i=k+1; i<m; i++) {
		quot=A[i][k]/A[k][k]; // individuare il quzionte della divisione 
		addRows(A, k, i, quot, n); // modificare le righe di A (inserendo i resti)
		addRows(mat, k, i, quot, c); // stesso lavoro su S o T a seconda dei casi 
	}
	return;
}

// verificare se la colonna è della forma [0, ..., 0, d, 0, ..., 0] con d in posizione k
bool nullVector (int **mat, int m, int k) {
	for (int i=0; i<m; i++) {
		if (i!=k && mat[i][k]!=0) {
			return false; 
		}
	}
	return true; 
}

// prima forma di Smith che porta a un matrice diagonale ma i cui elementi in diagonale non necessariamente sono multipli
void Smith_fst (int **A, int **S, int **T, int **At, int **Tt, int m, int n, int k) {
	while(!nullVector(A, m, k) || !nullVector(traspMatrix(A, At, m, n), n, k)) { //lavoro sulla riga e colonna k fino a che entrambe non siano contemporaneamente della forma [d, 0, ..., 0]
		switchZeros(A, T, At, Tt, m, n, k); // per prima cosa sposto l'eventuale colonna nulla
		while (!nullVector(A, m, k)) { // comincio a lavorare sulle colonne 
			switchSigns(A, S, m ,m, n, k); // cambio i segni della colonna k 
			while (!nullVector(A, m, k)) { // finchè la colonna non è della forma corretta applico a ripetizione le seguenti funzioni 
				minUp(A, S, m, m, n, k); // porto il minimo in alto 
				div_rem(A, S, m, m, n, k); // divisione con i resti 
			}
		}
		// per poter lavorare sulle righe traspongo la matrice e ripeto lo stesso procedimento fatto per la colonna 
		At=traspMatrix(A, At, m, n);  
		Tt=traspMatrix(T, Tt, n, n);
		while (!nullVector(At, n, k)) {
			switchSigns(At, Tt, n, n, m, k);
			while (!nullVector(At, n, k)) {
				minUp(At, Tt, n, n, m, k);
				div_rem(At, Tt, n, n, m, k);
			}
		}
		A=traspMatrix(At, A, n, m);
		T=traspMatrix(Tt, T, n, n);
	}
	switchSigns(A, S, m, m, n, k); // cambio il segno dell'ultimo elemento in diagonale nel caso sia negativo
	return;
}

// individuo il min tra due interi 
int min (int m, int n) {
	if (m<n) {
		return m; 
	} else {
		return n;
	}
}

// verifico che la matrice sia di Smith, ossia d1|d2|...|d(min(m, n))
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

// forma normale di Smith 
void SmithNormalForm (int **A, int **S, int **T, int m, int n) { 
	int **At=NULL;
	At=input_null(At, n, m);
	int **Tt=NULL;
	Tt=input_null(Tt, n, n);
	
	int k=0, r=min(m, n);
	while (k!=r) {
		Smith_fst(A, S, T, At, Tt, m, n, k); // prima raggiungo la forma diagonale 
		k++;
	}
	
	int it=0, iter=0;
	while (it!=r) {
		if (!boolSmith(A, r, it)) { // nel caso non sia di Smith 
			for (int i=it+1; i<r; i++) { // sommo tutte le righe alla prima riga 
				addRows(A, i, it, -1, n);
				addRows(S, i, it, -1, m);
			}
			iter=it; 
			while (iter!=r) {
				Smith_fst(A, S, T, At, Tt, m, n, iter); // e riapplico Smith così da ottenere in alto il mcm dei d_i
				iter++;
			}
		}
		it++;
		if (it==r) {
			switchSigns(A, S, m, m, n, r-1); // cambio eventualmente il segno dell'ultimo elemento in diagonale 
		}	
	}
	return;
}

// moltiplicare due matrici: C=A*B
void multiplyMatrices(int **A, int **B, int **C, int m, int n, int p) {
	// Itera sulle righe di A
	for (int i=0; i<m; i++) {
		// Itera sulle colonne di B
		for (int j=0; j<p; j++) {
	   		 C[i][j] = 0;
	    		// Calcola il prodotto scalare tra la riga i di A e la colonna j di B
	    		for (int k=0; k<n; k++) {
				C[i][j]=C[i][j]+A[i][k]*B[k][j];
	   		}
		}
	}
}

// forma normale di Smith senza ricordare le matrici S e T
void Smith_D (int **A, int m, int n) { 
	int **S=NULL; 
	S=input_id(S, m);
	int **T=NULL; 
	T=input_id(T, n);
	int **At=NULL; 
	At=input_null(At, n, m);
	int **Tt=NULL; 
	Tt=input_null(Tt, n, n);
	
	SmithNormalForm(A, S, T, m, n);
	return;
}

#endif
