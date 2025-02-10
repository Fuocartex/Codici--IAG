#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <gmp.h>
#include "..\Include\mcd.h"

bool p1_pollard (mpz_t, mpz_t);
void exp_mod (mpz_t, mpz_t, mpz_t, mpz_t);

int main () {
	mpz_t n, d, r;
	mpz_inits(n, d, r, NULL);
	printf("Inserire il numero non primo da fattorizzare: n = ");
	gmp_scanf("%Zd", n);
	
	mpz_fdiv_r_ui (r, n, 2);
	if (mpz_cmp_si(r,0)==0) { // se il numero è pari allora 2 è un divisore proprio
		gmp_printf("Un divisore di %Zd è d = 2\n", n);
	} else {
		if (p1_pollard(d, n)) {
			gmp_printf("Un divisore di %Zd è d = %Zd\n", n, d);
		} else {
			gmp_printf("Non è stato trovato un divisore di %Zd\n", n);
		}
	}
   	
  	// Pulizia della memoria
	mpz_clears(n, d, NULL);
		
	return 0; 
}

bool p1_pollard (mpz_t d, mpz_t n) {
	mpz_t b, b0, b1, a, m, y, it; 
	mpz_inits(b, b0, b1, a, m, y, it, NULL);
	mpz_set_si(a, 2);
	mpz_set_si(m, 1);
	//mpz_set_si(b0, 2);
	//mpz_set_si(b1, 66); // perchè la lista dei primi va fino a 61 (da 67 in poi non ci sono)
	mpz_set_si(b, 2);
	int primes[18] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61};
	
	mpz_t it_max; 
	mpz_inits(it_max, NULL);
	mpz_set_si(it_max, 1000);
	
	while (mpz_cmp(b, n)<0) { // finchè it < it_max
		//RandNumber(b, b0, b1);
		//gmp_printf("b=%Zd\n", b);	
		for (int i=0; i<18; i++) {
			if (mpz_cmp_si(b, primes[i])<0) { // appena b > primes[i] interrompo il for
				break; 
			} else { // finchè b < primes(i)
				mpz_t e, base; 
				mpz_inits(e, base, NULL);
			    	mpz_set_si(base, primes[i]);  // Converti int in mpz_t
				mpz_pow_ui(e, base, strlen(mpz_get_str(NULL,primes[i],b))-1);
				mpz_mul(m, m, e);  // m = m * e
				mpz_clears(e, NULL);
			}
		}

		exp_mod(y, a, m, n);

		mpz_sub_ui(y, y, 1);

		mcd_euclide(d, y, n); // d = MCD(a^m-1, n)
		// gmp_printf("d=%Zd\n", d);
		if (mpz_cmp_si(d, 1)!=0 && (mpz_cmp(d, n)!=0)) {
			return true; 
		}
		
		mpz_add_ui(b, b, 1); // it = it + 1
	}
	
	mpz_clears(b, b0, b1, a, m, y, it, NULL);
	
	return false; 
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
