#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include "mcd.h"

bool fermat (mpz_t, mpz_t);
void RandNumber (mpz_t, mpz_t, gmp_randstate_t);
void exp_mod (mpz_t, mpz_t, mpz_t, mpz_t);

int main() {
	mpz_t n, r, it_max; 
	mpz_inits(n, r, it_max, NULL);
	printf("n=");
	gmp_scanf("%Zd", &n);
	
	// 2 e 3 sono primi
	if (mpz_cmp_si(n, 2)==0 || mpz_cmp_si(n, 3)==0) { 
		gmp_printf("%Zd è un numero primo\n", n);
		return 0; 
	}
	
	// i numeri pari diversi da 2 ovviamente non sono primi 
	mpz_fdiv_r_ui(r, n, 2);
	if (mpz_cmp_si(r, 0)==0) { 
		gmp_printf("%Zd non è un numero primo e 2 è un suo divisore\n", n);
		return 0;
	}
	
	// studiamo la primalità dei numeri dispari 
	mpz_set_ui(it_max, mpz_sizeinbase(n, 2));
	if (fermat(n, it_max)) {
		gmp_printf("%Zd è un numero primo\n", n);
	}

	mpz_clears(n, r, it_max, NULL);
	
	return 0; 
}	

// studiamo la primalità dei numeri dispari: la funzione ritorna true se il numero è primo, altrimenti ritorna false
bool fermat (mpz_t n, mpz_t it_max) {
	mpz_t a, it, d, c, exp; 
	mpz_inits(a, it, d, c, exp, NULL);
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, time(NULL));
	mpz_set_si(it, 0);
	mpz_set_si(a, 2);
	
	while (mpz_cmp(it, it_max)<0) { // finchè it < it_max
		mcd_euclide(d, a, n); // calcolo il massimo comune divisore tra a e n 
		if (mpz_cmp_si(d, 1)!=0) { // se d!=1, n non è primo e d è un suo divisore
			gmp_printf("%Zd non è un numero primo e %Zd è un suo divisore\n", n, d);
			
			mpz_clears(a, it, d, c, exp, NULL);
			gmp_randclear(state);
			
			return false;
		} else { // altrimenti verifico se a^(n-1) é congruo o meno a 1 mod n, in caso affermativo n è uno pseudoprimo e continuo la verifica con un'altra base a, se non lo è n non è primo, ma in questo caso non ho informazioni sul suo divisore 
			mpz_sub_ui(exp, n, 1); // exp = n - 1
			exp_mod(c, a, exp, n); // calcoliamo a^(n-1) mod n
			if (mpz_cmp_si(c, 1)!=0) {
				gmp_printf("%Zd non è un numero primo\n", n);
				
				mpz_clears(a, it, d, c, exp, NULL);
				gmp_randclear(state);
				
				return false; 
			} else {
				RandNumber(a, n, state);
				mpz_add_ui(it, it, 1); // it = it + 1
			}
		}
	}
	
	// Pulizia della memoria
	mpz_clears(a, it, d, c, exp, NULL);
	gmp_randclear(state);
	
	return true; 
}

void RandNumber(mpz_t a, mpz_t n, gmp_randstate_t state) {
    mpz_t range;
    mpz_init(range);
       
    mpz_sub_ui(range, n, 3);  // range = (n - 2) - 2 + 1 = n - 3

    // Genera un numero casuale in [0, range]
    mpz_urandomm(a, state, range);
    // Sposta il numero generato nell'intervallo [2, n-2]
    mpz_add_ui(a, a, 2);

    mpz_clear(range);
}

// calcola in x la base b elevata alla potenza exp>=0 modulo n (exp binario)
void exp_mod (mpz_t x, mpz_t b, mpz_t exp, mpz_t n) {
	mpz_t squares, e;
	mpz_init_set(squares, b);
	mpz_init_set(e, exp);
	mpz_set_si(x, 1);

	while (mpz_cmp_si(e, 0)>0) { // finchè e > 0
		if (mpz_tstbit(e, 0)) { // se bit è 1 moltiplico x per il quadrato (mod n), altrimenti lascio così
			mpz_mul(x, x, squares);
			mpz_mod(x, x, n);
		}
		mpz_mul(squares, squares, squares); // aggiorno il quadrato (mod n)
		mpz_mod(squares, squares, n); // squares mod n 
		mpz_fdiv_q_2exp(e, e, 1); // shifto di 1 i bit di e
	}

	mpz_clears(squares, e, NULL);
	
	return;
}
