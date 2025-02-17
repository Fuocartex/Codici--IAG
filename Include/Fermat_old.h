#pragma once
#ifndef Fermat
#define Fermat

long int** matNull (long int**, int, int);
int* trasfBin (int*, long int, int);
void printVec (int*, int);
long int** inputMatPot (long int**, int*, int, long int, long int);
long int potAmod (long int, long int);
long int congruenza_a (long int, long int, long int);
long int adjust_x (long int, long int);
long int change_a (long int, long int, long int);
bool fermat (long int, int);
long int MCD(long int, long int);

long int** matNull (long int **mat, int m, int n) {
	mat=calloc(m, sizeof(long int*));
	for (int i=0; i<m; i++) {
		mat[i]=calloc(n, sizeof(long int));
	}
	return mat; 
}

int* trasfBin (int *vecBin, long int n, int l) {
	vecBin=calloc(l, sizeof(int));
	for (int i=l-1; i>=0; i--) {
		vecBin[i]=n%2; 
		n=n/2;
	}
	return vecBin; 
}

void printVec (int *vecBin, int l) {
	for (int i=0; i<l; i++) {
		printf("%d", vecBin[i]);
	}
	printf("\n");
	return; 
}

long int** inputMatPot (long int **matPot, int *vecBin, int l, long int a, long int n) {
	matPot=matNull(matPot, l, 2);
	int j=l-1;
	for (int i=0; i<l; i++) {
		matPot[i][0]=vecBin[j]; //nella prima colonna scrivimao il numero in base binaria (al contrario)
 		j=j-1;
 		if (i==0) { // nella seconda colonna mettiamo le potenze di a mod n (a, a^2, (a^2)^2 ...)
 			matPot[i][1]=a; 
 		} else {
 			matPot[i][1]=potAmod(matPot[i-1][1], n);
 		} 		
	}
	return matPot; 
}

// calcola l'elevazione al quadrato di k mod mod 
long int potAmod (long int k, long int mod) { 
	long int x=k*k;
	x=adjust_x(x,mod);
	return x;
}

// calcola a^(n-1) mod n 
long int congruenza_a (long int a, long int m, long int n) {
	int lBin=floor(log2(m))+1; // calcolo la lunghezza di n-1 (ossia m) in binario per poi inizializzare il vettore
	int *vecBin=NULL;
	vecBin=trasfBin(vecBin, m, lBin); //m sarà n-1, quindi trasformo n-1 in binario
	long int **matPot=NULL;
	matPot=inputMatPot(matPot, vecBin, lBin, a, n); // matPot è una matrice lBin*2, dove la prima colonna corrisponde ad n_binario e la seconda alle potenze di a (come meglio specificato nei commenti della funzione matPot)
	long int c=1; // inizializziamo c=a^(n-1) mod n
	for (int i=0; i<lBin; i++) { // modifichiamo c a dovere 
		if (matPot[i][0]==1) { // se la cifra in binario è 1 moltiplico c per la potenza di a corrispondente, sennò non modifico c
			c=c*matPot[i][1];
		}
		c=adjust_x(c, n); // riporto c congruo modulo n 
	}
	return c; 
}

// prendo la classe di congruenza mod mod di x 
long int adjust_x (long int x, long int mod) {
	if ((x>0 && x>=mod) || (x<0 && (-x)>=mod)) { // se abs(x)>=mod togliamo ad x i multipli di mod perchè a*x=x mod a (abs scrittto così perche abs lavora su int)
		long int temp=0;  
		temp=x/mod;
		x=x-temp*mod;
	}
	long int ceilModMezzi=(mod/2)+1; // perchè ceil lavora su double
	if (x>ceilModMezzi) { // prendiamo la classe di equivalente di modulo minore
		x=x-mod;
	} else if (x<0 && (-x)>=ceilModMezzi) {
		x=mod+x; 
	}
	return x;
}

// modifico in maniera casuale la base a 
long int change_a (long int a, long int max, long int min) {
	srand(time(NULL));
	a=rand()%(max-min+1)+min; // prendiamo una base a scelta casualmente nell'intervallo [2, n-2]
	return a; 
}

// studiamo la primalità dei numeri dispari 
bool fermat (long int n, int it_max) {
	long int a=2, max=n-2, min=2;
	for (int it=1; it<=it_max; it++) { 
		long int d=MCD(a, n); // calcolo il massimo comune divisore tra a e n 
		if (d!=1) { // se è diverso da 1 n non è primo e d è un suo divisore
			printf("%ld non è un numero primo e %ld è un suo divisore\n", n, d);
			return false;
		} else { // altrimenti verifico se a^(n-1) é congruo o meno a 1 mod n, in caso affermativo n è uno pseudoprimo e continuo la verifica con un'altra base a, se non lo è n non è primo, ma in questo caso non ho informazioni sul suo divisore 
			long int c=congruenza_a(a, n-1, n);
			if(c!=1) {
				printf("%ld non è un numero primo\n", n);
				return false; 
			} else {
				a=change_a(a, max, min); 
			}
		}
	}
	return true; 
}

long int MCD(long int a, long int b) {
	long int temp=0; 
	while (b!=0) {
		temp=b;
		b=a%b;
		a=temp;
	}
	return a;
}

#endif	
