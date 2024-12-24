#pragma once
#ifndef Smith
#define Smith

int** input_sm (int**, int**, int, int);
int** input_id (int**, int);
int** input_null (int**, int, int);
void printMatrix (int**, int, int, int);
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
int my_min (int, int);
bool bool_dk (int**, int, int);
void SmithNormalForm (int**, int**, int**, int, int);
void multiplyMatrices(int**, int**, int**, int, int, int);
void Smith_D (int**, int, int); 
bool boolDiag (int**, int, int);
int checkDiagZeros (int**, int);
void Smith_fst5mat (int**, int**, int**, int**, int**, int**, int**, int**, int, int, int);
void SmithNormalForm5mat (int**, int**, int**, int**, int**, int, int);
void minUp5mat (int**, int**, int**, int, int, int, int);
void div_rem5mat (int**, int**, int**, int, int, int, int);
void switchSigns5mat (int**, int**, int**, int, int, int, int);
void switchZeros5mat (int**, int**, int**, int**, int**, int, int, int);

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
void printMatrix (int **mat, int m, int n, int t) {
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
        		for (int l=0; l<t; l++) {
        			printf(" ");
        		}
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

// spostare un'eventuale colonna nulla in fondo alla matrice
void switchZeros (int **A, int **T, int **At, int **Tt, int m, int n, int k) {
	bool var=true; 
	for (int i=k; i<m; i++) {
		if (A[i][k]!=0) {
			var=false;
			break; 
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
int my_min (int m, int n) {
	if (m<n) {
		return m; 
	} else {
		return n;
	}
}

// verifico che d(k) divida tutti gli elementi diagonali successivi, ossia d(k)|d(k+1), d(k)|d(k+2), ... , d(k)|d(min(m, n))
bool bool_dk (int **A, int r, int k) { // r=min(m, n) e k è l'indice dell'elemento diagonale che sto verificando  
	int quot=0, rem=0;
	for (int i=k+1; i<r; i++) { // scorro i da k+1 ad r 
		rem=A[i][i]%A[k][k];
		if(rem!=0) { // se il resto della divisione non è zero allora esco dalla verifica con false perchè d(k) non divide d(i) per almeno un indice i>k
			return false; 
		}
	}
	return true;
}

// verifichiamo se la matrice è diagonale 
bool boolDiag (int **A, int m, int n) { // m numero di righe, n numero di colonne
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			if (i!=j && A[i][j]!=0) { // appena trovo due indici i e j diversi tra loro per cui A[i][j] non è nullo ritorno false 
				return false; 
			}
		}
	}
	return true; 
}

int checkDiagZeros (int **A, int r) {
	for (int i=0; i<r; i++) {
		if (A[i][i]==0) { // appena trovo un elemento diagonale nullo verifico i successivi 
			for (int j=i; j<r; j++) {
				if (A[j][j]!=0) { // appena ne trovo uno dopo non nullo ritorno la riga i in cui ho uno zero fuori posto (la verifica è fallita)
					return i;
				}
			}
			return r; // se invece a fine del ciclo sui successivi sono tutti nulli ritorno r, senza bisogno di proseguire con il ciclo for iniziale tanto i successivi sono già stati verificati (la verifica ha avuto successo)
		}
	}
	return r; 
}

// forma normale di Smith 
void SmithNormalForm (int **A, int **S, int **T, int m, int n) { 
	int **At=NULL;
	At=input_null(At, n, m);
	int **Tt=NULL;
	Tt=input_null(Tt, n, n);
	
	int r=my_min(m, n);
	
	// se la matrice in input è gia diagonale non viene modificata da Smith_fst, quindi se avesse zero in mezzo alla diagonale e non in fondo, nel momento in cui verifico se gli elementi in diagonale si dividono consecutivamente lo zero causa problemi, motivo per cui se la matrice in input diagonale è di tale forma sommo a tutte le righe le successive, in questo modo Smith_fst modifica la matrice in maniera corretta e questo problema è evitato 
	if (boolDiag(A, m, n)) { // se la matrice in input è diagonale 
		int err=checkDiagZeros(A, r);
		if (err!=r) { // verifico che abbia gli zeri tutti in fondo, in caso contrario 
			for (int j=err+1; j<r; j++) { // sommo alla riga err (con lo zero fuori posto) le righe successive j
				addRows(A, j, err, -1, n);
				addRows(S, j, err, -1, m);
			}
		}
	}
	
	for (int k=0; k!=r; k++) {
		Smith_fst(A, S, T, At, Tt, m, n, k); // prima raggiungo la forma diagonale 
	}		
	
	for (int it=0; it<r; it++) {
		if (A[it][it]==0) { // se l'elemento diagonale è nullo lo sono anche i successivi (per definizione di Smith_fst e da quanto fatto da boolDiag e check DiagZeros)   
			break; // quindi posso uscire dal for perchè la verifica è completa
		}
		//if (it<r-1 && A[it+1][it+1]%A[it][it]!=0) {
		if (!bool_dk(A, r, it)) { // nel caso non sia di Smith 
			for (int i=it+1; i<r; i++) { // sommo alla riga it tutte le righe successive i  
				addRows(A, i, it, -1, n);
				addRows(S, i, it, -1, m);
			}
			for (int iter=it; iter!=r; iter++) {
				Smith_fst(A, S, T, At, Tt, m, n, iter); // e riapplico Smith così da ottenere in alto il mcm dei d_i
			}
		}
	}
	switchSigns(A, S, m, m, n, r-1); // cambio eventualmente il segno dell'ultimo elemento in diagonale 
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

// questa funzione produce la forma di Smith in maniera totalmente analoga alla funzione SmithNormalForm, l'unica differenza è che produce anche le matrici S_inv e T_inv tali per cui S_inv*D*T_inv=A (di seguito andiamo quindi a commentare solo le differenze rispetto alla funzione precedente)
void SmithNormalForm5mat (int **A, int **S, int **T, int **S_inv, int **T_inv, int m, int n) { 
	int **At=NULL;
	At=input_null(At, n, m);
	int **Tt=NULL;
	Tt=input_null(Tt, n, n);
	int **SinvT=NULL; 
	SinvT=input_null(SinvT, m, m);
	
	int r=my_min(m, n);
	
	if (boolDiag(A, m, n)) {
		int err=checkDiagZeros(A, r);
		if (err!=r) { 
			for (int j=err+1; j<r; j++) {
				addRows(A, j, err, -1, n);
				addRows(S, j, err, -1, m);
				addRows(S_inv, j, err, 1, m); //quanto fatto ad S viene fatto anche ad S_inv
			}
		}
	}
	
	for (int k=0; k!=r; k++) {
		Smith_fst5mat(A, S, T, S_inv, T_inv, At, Tt, SinvT, m, n, k); 
	}	
	
	for (int it=0; it<r; it++) {
		if (A[it][it]==0) {
			break;
		}
		if (!bool_dk(A, r, it)) { 
			SinvT=traspMatrix(S_inv, SinvT, m, m);
			for (int i=it+1; i<r; i++) {
				addRows(A, i, it, -1, n);
				addRows(S, i, it, -1, m);
				addRows(SinvT, it, i, 1, m); // presa S1 e S2, mentre S=S2*S1, S_inv=S1_inv*S2_inv quindi così come per le T per moltiplicare a destra usavo la trasposta lo stesso faccio con S_inv (tenendo conto che l'inversa fa i passaggi al contrario, quindi anzichè -1 metto 1 e inverto it e i)
			}
			S_inv=traspMatrix(SinvT, S_inv, m, m);
			for (int iter=it; iter!=r; iter++) {
				Smith_fst5mat(A, S, T, S_inv, T_inv, At, Tt, SinvT, m, n, iter); 
			}
		}
	}
	SinvT=traspMatrix(S_inv, SinvT, m, m);
	switchSigns5mat(A, S, SinvT, m, m, n, r-1); 
	S_inv=traspMatrix(SinvT, S_inv, m, m);
	return;
}

// come nel caso precedente, questa funzione è analoga a Smith_fst con l'aggiunta di S_inv e T_inv
void Smith_fst5mat (int **A, int **S, int **T, int **S_inv, int **T_inv, int **At, int **Tt, int **SinvT, int m, int n, int k) {
	while(!nullVector(A, m, k) || !nullVector(traspMatrix(A, At, m, n), n, k)) {
		switchZeros5mat(A, T, T_inv, At, Tt, m, n, k);
		SinvT=traspMatrix(S_inv, SinvT, m, m);
		while (!nullVector(A, m, k)) {
			switchSigns5mat(A, S, SinvT, m, m, n, k);
			while (!nullVector(A, m, k)) {
				minUp5mat(A, S, SinvT, m, m, n, k); 
				div_rem5mat(A, S, SinvT, m, m, n, k);
			}
		}
		S_inv=traspMatrix(SinvT, S_inv, m, m);
		At=traspMatrix(A, At, m, n);  
		Tt=traspMatrix(T, Tt, n, n);
		while (!nullVector(At, n, k)) {
			switchSigns5mat(At, Tt, T_inv, n, n, m, k);
			while (!nullVector(At, n, k)) {
				minUp5mat(At, Tt, T_inv, n, n, m, k);
				div_rem5mat(At, Tt, T_inv, n, n, m, k);
			}
		}
		A=traspMatrix(At, A, n, m);
		T=traspMatrix(Tt, T, n, n);
	}
	SinvT=traspMatrix(S_inv, SinvT, m, m);
	switchSigns5mat(A, S, SinvT, m, m, n, k); 
	S_inv=traspMatrix(SinvT, S_inv, m, m);
	return;
}

// funzione analoga a switchZeros con l'aggiunta di S_inv e T_inv
void switchZeros5mat (int **A, int **T, int **T_inv, int **At, int **Tt, int m, int n, int k) {
	bool var=true; 
	for (int i=k; i<m; i++) {
		if (A[i][k]!=0) {
			var=false;
			break; 
		}
	}
	if (var) {
		At=traspMatrix(A, At, m, n); 
		Tt=traspMatrix(T, Tt, n, n); 
		for (int j=k; j<(n-1); j++) {
			swapRows(At, j, j+1, m);
			swapRows(Tt, j, j+1, n);
			swapRows(T_inv, j, j+1, n);
		}
		A=traspMatrix(At, A, n, m);
		T=traspMatrix(Tt, T, n, n);
	}
	return; 
}		

// funzione analoga a switchSigns con l'aggiunta di S_inv e T_inv
void switchSigns5mat (int **A, int **mat, int **mat2, int c, int m, int n, int k) {
	for (int i=k; i<m; i++) {
		if (A[i][k]<0) {
			for (int j=k; j<n; j++) {
				A[i][j]=-A[i][j];
			}
			for (int l=0; l<c; l++) {
				mat[i][l]=-mat[i][l];
				mat2[i][l]=-mat2[i][l]; // matrice S_inv o T_inv a seconda dei casi  
			}
		}
	}
	return;
}

// funzione analoga a minUp con l'aggiunta di S_inv e T_inv
void minUp5mat (int **A, int **mat, int **mat2, int c, int m, int n, int k) {
	int min=A[k][k], r_min=k;
	for(int i=k; i<m; i++) {
		if (A[i][k]<min && A[i][k]!=0 || min==0) { 
			min=A[i][k]; 
			r_min=i;
		}
	}
	swapRows(A, k, r_min, n); 
	swapRows(mat, k, r_min, c);  
	swapRows(mat2, k, r_min, c); // stesso lavoro anche su S_inv o T_inv a seconda dei casi 
	
	return;
}

// funzione analoga a div_rem con l'aggiunta di S_inv e T_inv
void div_rem5mat (int **A, int **mat, int **mat2, int c, int m, int n, int k) {
	int quot=0;  
	for (int i=k+1; i<m; i++) {
		quot=A[i][k]/A[k][k];  
		addRows(A, k, i, quot, n); 
		addRows(mat, k, i, quot, c); 
		addRows(mat2, i, k, -quot, c); // stesso lavoro su S_inv o T_inv a seconda dei casi (l'inversa fa i passaggi al contrario) 
	}
	return;
}

#endif
