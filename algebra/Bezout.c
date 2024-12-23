#include <stdio.h>
#include <gmp.h>
#include "..\Include\Bezout.h"

int main () {
	mpz_t d,x,y,n,m;
	mpz_inits(d,x,y,n,m,NULL);

	gmp_scanf("%Zd",n);
	gmp_scanf("%Zd",m);

	bezout(d,x,y,n,m);
	gmp_printf("%Zd = %Zd * %Zd + %Zd * %Zd\n",d,x,n,y,m);
	mpz_gcdext(d,x,y,n,m);
	gmp_printf("%Zd = %Zd * %Zd + %Zd * %Zd\n",d,x,n,y,m);

	mpz_clears(d,x,y,n,m,NULL);
	return 0;
}













// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include "..\Include\Smith.h"
// #include "..\Include\Bezout.h"

// int main() {
// 	int n; 
// 	printf("Il numero di elementi a cui applicare Bezout è ");
// 	scanf("%d", &n);
// 	printf("Gli elementi sono: "); 
// 	int *elem=NULL; 
// 	elem=imputBezout(n, elem);
	
// 	// inizializzazione matrici necessarie + applicazione di Bezout
// 	int **MCD=NULL;
// 	MCD=input_null(MCD, n, 1);
// 	int **S=NULL; 
// 	S=input_id(S, n);
// 	int **T=NULL; 
// 	T=input_id(T, 1);
// 	bezout(n, elem, MCD, S, T);
// 	printBezout(n, elem, MCD, S);
	
// 	return 0;
// }
