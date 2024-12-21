#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "..\Include\Fermat.h"

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
	if (fermat(n, it_max)) {
		printf("%ld è un numero primo\n", n);
	}
	
	return 0; 
}	
