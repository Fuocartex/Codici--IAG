#include <stdio.h>
#include "..\Include\Bezout.h"

void print_bezout (mpz_t, mpz_t*, mpz_t*, int);

int main () {
	int i,n;
	printf("Numero di interi a cui applicare Bezout: ");
	scanf("%d",&n);
	printf("\nInteri:\n");
	mpz_t d; mpz_init(d);
	mpz_t* integers = malloc(n*sizeof(mpz_t));
	mpz_t* coeff = malloc(n*sizeof(mpz_t));
	for (i=0; i<n; i++) {
		mpz_init(integers[i]);
		mpz_init(coeff[i]);
		gmp_scanf("%Zd",integers[i]);
	}

	bezout_array(d,coeff,integers,n);
	print_bezout(d,coeff,integers,n);

	mpz_clear(d);
	for (i=0; i<n; i++) {
		mpz_clear(integers[i]);
		mpz_clear(coeff[i]);
	}
	return 0;
}

void print_bezout (mpz_t d, mpz_t* coeff, mpz_t* integers, int n) {
	int i;
	printf("\nIdentita' di Bezout:\n");
	gmp_printf("%Zd = ",d);
	for (i=0; i<n-1; i++) {
		gmp_printf("(%Zd)*(%Zd) + ",coeff[i],integers[i]);
	}
	gmp_printf("(%Zd)*(%Zd)\n",coeff[n-1],integers[n-1]);

	return;
}
