#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Smith.h"
#include "Bezout.h"

int main() {
	int n; 
	printf("Il numero di elementi a cui applicare Bezout è ");
	scanf("%d", &n);
	printf("Gli elementi sono: "); 
	int *elem=NULL; 
	elem=imputBezout(n, elem);
	
	// inizializzazione matrici necessarie + applicazione di Bezout
	int **MCD=NULL;
	MCD=input_null(MCD, n, 1);
	int **S=NULL; 
	S=input_id(S, n);
	int **T=NULL; 
	T=input_id(T, 1);
	bezout(n, elem, MCD, S, T);
	printBezout(n, elem, MCD, S);
	
	return 0;
}
