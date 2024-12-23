#include <stdlib.h>
#include <gmp.h>

void bezout(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);

// Calcolo d=mcd(n,m) con algoritmo euclideo (scelgo d>=0), e anche x,y interi tali che x*n+y*m=d.
void bezout(mpz_t d, mpz_t x, mpz_t y, mpz_t n, mpz_t m) {
	int i,j;
	mpz_t a,b,q,temp1,temp2;
	mpz_inits(a,b,q,temp1,temp2,NULL);
	mpz_abs(a,n); mpz_abs(b,m); // inizializzo a,b positivi per avere d>=0; se necessario cambio segno a x,y alla fine
	// la matrice M (2*2) conterra' alla fine i coefficienti di bezout nella prima riga
	mpz_t** M=malloc(2*sizeof(mpz_t*));
	for (i=0; i<2; i++) M[i]=malloc(2*sizeof(mpz_t));
	mpz_init_set_si(M[0][0],1); mpz_init(M[0][1]); mpz_init(M[1][0]); mpz_init_set_si(M[1][1],1); // inizializzo M = identità

	// algoritmo di euclide
	while (mpz_cmp_si(b,0)>0) {  // se b>0 continuo a iterare
        mpz_fdiv_qr(q,a,a,b);  // a diventa il resto di a/b, q quoziente
        mpz_swap(a,b);

		// aggiorno matrice dei coefficienti: devo moltiplicare a sinistra per [[0, 1]; [1, -q]] (caso particolare di solo due interi)
		mpz_set(temp1,M[1][0]); mpz_set(temp2,M[1][1]); // copio prima riga
		mpz_neg(q,q);
		mpz_mul(M[1][0],M[1][0],q); mpz_add(M[1][0],M[1][0],M[0][0]); // calcolato riga 2 colonna 1
		mpz_mul(M[1][1],M[1][1],q); mpz_add(M[1][1],M[1][1],M[0][1]); // calcolato riga 2 colonna 2
		mpz_set(M[0][0],temp1); mpz_set(M[0][1],temp2); // calcolata riga 1
    }
    mpz_set(d,a);  // calcolato mcd
	mpz_set(x,M[0][0]); mpz_set(y,M[0][1]);
	// aggiusto i coefficienti per eventuali cambi di segno
	if (mpz_cmp_si(n,0)<0) mpz_neg(x,x);
	if (mpz_cmp_si(m,0)<0) mpz_neg(y,y);

	for (i=0; i<2; i++) {
		for (j=0; j<2; j++) mpz_clear(M[i][j]);
	}
	mpz_clears(a,b,q,temp1,temp2,NULL);
	return;
}
























// #pragma once
// #ifndef Bezout
// #define Bezout

// int* imputBezout (int, int*);
// void bezout (int, int*, int**, int**, int**);
// void printBezout (int, int*, int**, int**);

// // imput vettore elem contenente i numeri a cui verrà applicato Bezout
// int* imputBezout (int n, int *elem) {
// 	elem=calloc(n, sizeof(int));
// 	for (int i=0; i<n; i++) {
// 		scanf("%d", &elem[i]);
// 	}
// 	return elem; 
// }

// // Bezout(a,b): a*x+b*y=MCD
// void bezout (int n, int *elem, int **MCD, int **S, int **T) {
// 	for (int i=0; i<n; i++) { // poniamo il vettore MCD uguale ad elem per poi applicargli l'algoritmo di Smith
// 		MCD[i][0]=elem[i]; 
// 	}
// 	SmithNormalForm(MCD, S, T, n, 1); // applico Smith ad alla matrice MCD così da trovare il massimo comune divisore (in posizione MCD[1][0]) e i coefficienti x,y,... per cui moltiplicare gli elementi (prima riga della matrice S
// 	return; 
// }

// // stampa 
// void printBezout (int n, int *elem, int **MCD, int **S) {
// 	printf("Bezout: ");
// 	for (int i=0; i<n; i++) {
// 		if (i!=0) {
// 			printf(" + ");
// 		}
// 		printf("%d*", elem[i]);
// 		if (S[0][i]<0) {
// 			printf("(%d)", S[0][i]);
// 		} else {
// 			printf("%d", S[0][i]);
// 		}
// 		if (i==(n-1)) {
// 			printf(" = %d\n", MCD[0][0]);
// 		}
// 	}
// 	return; 
// }

// #endif
