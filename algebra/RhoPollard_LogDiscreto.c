#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include "..\Include\mcd.h"
#include "..\Include\Bezout.h"

void rho_pollard (mpz_t, mpz_t, mpz_t, mpz_t);
void funz (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);

int main() {
	mpz_t x, g, h, p;
	mpz_inits(x, g, h, p, NULL);
	printf("Risolvere l'equazione g^x=h  nel gruppo (Z/(p))* (p primo) con i seguenti dati:\n");
	printf("g = ");
	gmp_scanf("%Zd", g);
	printf("h = ");
	gmp_scanf("%Zd", h);
	printf("p = ");
	gmp_scanf("%Zd", p);

	rho_pollard(x, g, h, p);	
	gmp_printf("La soluzione del log discreto è x = %Zd\n", x);
		
	mpz_clears(x, g, h, p, NULL);
	return 0; 
}

void rho_pollard (mpz_t x, mpz_t g, mpz_t h, mpz_t p) {
	mpz_t gamma, a, b, lambda, alpha, beta, p1, temp_a, temp_b, invb, d, c, invc, pd, ad, temp, it; 
	mpz_inits(gamma, a, b, lambda, alpha, beta, p1, temp_a, temp_b, invb, d, c, invc, pd, ad, temp, it, NULL);
	
	mpz_set_si(gamma, 1); // poniamo gamma = 1
	mpz_set_si(a, 0); // poniamo a = 0
	mpz_set_si(b, 0); // poniamo b = 0
	
	mpz_set_si(lambda, 1); // poniamo lambda = 1
	mpz_set_si(alpha, 0); // poniamo alpha = 0
	mpz_set_si(beta, 0); // poniamo beta = 0
	
	mpz_sub_ui(p1, p, 1); // p1 = p - 1;
	mpz_set_si(it, 0);
	
	bool test=false; 

	while (mpz_cmp(it, p)<0) {
		funz(gamma, a, b, p, p1, g, h);
		funz(lambda, alpha, beta, p, p1, g, h); 
		funz(lambda, alpha, beta, p, p1, g, h); // si muove al doppio della velocità
		
		if (mpz_cmp(gamma, lambda)==0) {
			mpz_sub(temp_b, beta, b); 
			mpz_sub(temp_a, a, alpha);
			mcd_euclide(d, temp_b, p1);
			if (mpz_cmp_si(d, 1)==0) {
				inv_mod(invb, temp_b, p1); // calcolo l'inverso di temp_b mod p1
				mpz_mul(x, invb, temp_a); 
				mpz_mod(x, x, p1);
				break;
			} else {
				mpz_fdiv_q(pd, p1, d); // calcolo p2 = p1 / d
			  	mpz_fdiv_q(c, temp_b, d);
			  	mpz_fdiv_q(ad, temp_a, d);
			  	inv_mod(invc, c, pd); // calcolo l'inverso di c mod p2
			  	mpz_mul(x, invc, ad);
				mpz_mod(x, x, pd); // unica soluzione mod p1/d (ossia pd)
				
				// studio a mano le d soluzioni mod p1
				mpz_t i; 
				mpz_inits(i, NULL);
				mpz_set_si(i, 0);
				while (mpz_cmp(i, d)<0) {
					if (mpz_cmp_si(i, 0)!=0) {
						mpz_add(x, x, d); // x = x + d
					}
					mpz_powm(temp, g, x, p);
					if (mpz_cmp(temp, h)==0) {
						test=true;
						break;
					}
					mpz_add_ui(i, i, 1);
				}
				mpz_clears(i, NULL);
			} 				
		}
		if (test==true) {
			break;
		}
		
		mpz_add_ui(it, it, 1);				
	}
	
	mpz_clears(gamma, a, b, lambda, alpha, beta, p1, temp_a, temp_b, invb, d, c, invc, pd, ad, temp, it, NULL);
	return;
}

void funz (mpz_t x, mpz_t a, mpz_t b, mpz_t p, mpz_t p1, mpz_t g, mpz_t h) {
	mpz_t temp; 
	mpz_inits(temp, NULL); 
	// suddividiamo il campo in 3 sottoinsiemi di uguale cardinalità con le classi di resto mod 3
	mpz_fdiv_r_ui(temp, x, 3); // temp = x % 3
	
	if (mpz_cmp_si(temp, 0)==0) {
		mpz_mul(x, x, x); // x = x * x
		mpz_mod(x, x, p); // riporto x mod p
		mpz_mul_ui(a, a, 2); // a = a * 2
		mpz_mod(a, a, p1); // riporto a mod p-1
		mpz_mul_ui(b, b, 2); // b = b * 2
		mpz_mod(b, b, p1); // riporto b mod p-1
	} else if (mpz_cmp_si(temp, 1)==0) {
		mpz_mul(x, x, g); // x = x * g
		mpz_mod(x, x, p); // riporto x mod p
		mpz_add_ui(a, a, 1); // a = a + 1
		mpz_mod(a, a, p1); // riporto a mod p-1
		// b resta invariato
	} else {
		mpz_mul(x, x, h); // x = x * g
		mpz_mod(x, x, p); // riporto x mod p
		// a resta invariato;
		mpz_add_ui(b, b, 1); // a = a + 1
		mpz_mod(b, b, p1); // riporto a mod p-1
	}
	
	mpz_clears(temp, NULL);
	return;
}
