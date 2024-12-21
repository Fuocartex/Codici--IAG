#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "..\Include\Fermat.h"

bool eulero (long int, int);

int main() {
	long int n=0; 
	printf("n=");
	scanf("%ld", &n);
	
	// 2 e 3 sono primi
	if (n==2 || n==3) { 
		printf("%ld è un numero primo\n", n);
		return 0; 
	}
	
	// i numeri pari diversi da 2 ovviamente non sono primi 
	if (n%2==0) { 
		printf("%ld non è un numero primo e 2 è un suo divisore\n", n);
		return 0;
	}
	
	// studiamo la primalità dei numeri dispari 
	int it_max=2*ceil(log2(n));
	if (eulero(n, it_max)) {
		printf("%ld è un numero primo\n", n);
	}
	
	return 0; 
}

bool eulero (long int n, int it_max) {
	long int a=2, max=n-2, min=2;
	for (int it=0; it<it_max; it++) { 
		long int d=MCD(a, n); // calcolo il massimo comune divisore tra a e n 
		if (d!=1) { // se è diverso da 1 n non è primo e d è un suo divisore
			printf("%ld non è un numero primo e %ld è un suo divisore\n", n, d);
			return false;
		} else { // altrimenti verifico se a^((n-1)/2) é congruo o meno a ±1 mod n, in caso affermativo n non è primo, in caso contrario continuo la verifica con un'altra base a
			long int c=congruenza_a(a, (n-1)/2, n);
			//printf("c=%d\n", c);
			if(c!=1 && c!=-1) {
				printf("%ld non è un numero primo\n", n);
				return false; 
			} else {
				a=change_a(a, max, min); 
			}
		}
	}
	return true; 
}
