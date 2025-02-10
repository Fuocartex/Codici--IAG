#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include "..\Include\mcd.h"

bool rho_pollard (mpz_t, mpz_t);
void funz_f (mpz_t, mpz_t, mpz_t);
void funz_g (mpz_t, mpz_t, mpz_t);

int main () {
	mpz_t n, d, r;
	mpz_inits(n, d, r, NULL);
	printf("Inserire il numero non primo da fattorizzare: n = ");
	gmp_scanf("%Zd", n);
	
	mpz_fdiv_r_ui (r, n, 2);
	if (mpz_cmp_si(r,0)==0) { // se il numero è pari allora 2 è un divisore proprio
		gmp_printf("Un divisore di %Zd è d = 2\n", n);
	} else {
		if (rho_pollard(d, n)) {
			gmp_printf("Un divisore di %Zd è d = %Zd\n", n, d);
		} else {
			gmp_printf("Non è stato trovato un divisore di %Zd\n", n);
		}
	}
	
	// Pulizia della memoria
	mpz_clears(n, d, r, NULL);
		
	return 0; 
}

// Fattorizza l'intero positivo n utilizzando l'algoritmo rho di Pollard:
// - true = d è un divisore proprio di n
// - false = non ha trovato un divisore di n 
bool rho_pollard (mpz_t d, mpz_t n) {
	mpz_t x, y, f, g, x_new, it;
	mpz_inits(x, y, f, g, x_new, it, NULL);
	
	// Assegniamo i valori iniziali x, y e it
	mpz_set_si(x, 1); // poniamo x = 1
	mpz_set_si(y, 1); // poniamo y = 1
	mpz_set_si(it, 0); // poniamo it = 0
	
	while (mpz_cmp(it, n)<0) { // finchè it < it_max
		// Calcoliamo f(x) e g(x)
    		funz_f(f, x, n);
    		funz_g(g, y, n);
	
		// calcoliamo f(x)-g(x) mod n 
		mpz_sub(x_new, g, f); // x_new = g - f
		mpz_mod(x_new, x_new, n); // x_new = x_new mod n
		mcd_euclide(d, x_new, n); // d = MCD(x_new, n)
		
		if (mpz_cmp_si(d, 1)!=0 && mpz_cmp(d, n)!=0) { // se d è diverso da 1 ho trovato un divisore di n
			return true; 
		}
		
		mpz_set(x, f); // la nuova x è f(x) calcolato prima
		mpz_set(y, g); // la nuova y è g(y) calcolato prima
		mpz_add_ui(it, it, 1); // it = it + 1
	}
		
	// Pulizia della memoria 
	mpz_clears(x, y, f, g, x_new, it, NULL);
	
	return false; 
}

void funz_f (mpz_t result, mpz_t x, mpz_t n) {
	mpz_t temp;
	mpz_init(temp);

	mpz_mul(temp, x, x);  // temp = x * x
	mpz_add_ui(result, temp, 1);  // result = temp + 1

	mpz_mod (result, result, n); // calcolo result mod n

        // Pulizia memoria
	mpz_clear(temp);
}

void funz_g (mpz_t result, mpz_t x, mpz_t n) {
	mpz_t temp, temp2, temp4;
	mpz_inits(temp, temp2, temp4, NULL);
	
	mpz_mul(temp2, x, x);  // temp2 = x * x (calcolo x^2)
	mpz_mul(temp4, temp2, temp2);  // temp4 = temp2 * temp2 (calcolo x^4)
	mpz_mul_ui(temp2, temp2, 2);  // temp2 = temp2 * 2 (calcolo 2*x^2)
	mpz_add(temp, temp4, temp2);  // temp = temp4 + temp2 (calcolo x^4+2*x^2)
	mpz_add_ui(result, temp, 2);  // result = temp + 1 (calcolo x^4+2*x^2+1)

	mpz_mod (result, result, n); // calcolo result mod n

        // Pulizia memoria
	mpz_clears(temp, temp2, temp4, NULL);
}
