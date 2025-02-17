#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Funzione per calcolare il massimo comune divisore (MCD)
void mcd(mpz_t result, mpz_t a, mpz_t b) {
    mpz_gcd(result, a, b);
}

// Esponenziazione modulare binaria
type void moduloExponentiation(mpz_t result, mpz_t base, mpz_t exp, mpz_t mod) {
    mpz_powm(result, base, exp, mod);
}

// Calcolo del simbolo di Jacobi
int jacobiSymbol(mpz_t a, mpz_t n) {
    return mpz_jacobi(a, n);
}

// Test di Solovay-Strassen
int solovayStrassen(mpz_t n, int iterations) {
    if (mpz_cmp_ui(n, 2) < 0) return 0;
    if (mpz_cmp_ui(n, 2) == 0) return 1;
    if (mpz_even_p(n)) return 0;

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    mpz_t a, n_minus1, exp, gcd, modExp;
    mpz_inits(a, n_minus1, exp, gcd, modExp, NULL);

    mpz_sub_ui(n_minus1, n, 1);
    mpz_fdiv_q_ui(exp, n_minus1, 2);

    for (int i = 0; i < iterations; i++) {
        mpz_urandomm(a, state, n_minus1);
        mpz_add_ui(a, a, 2);

        mcd(gcd, a, n);
        if (mpz_cmp_ui(gcd, 1) != 0) return 0;

        moduloExponentiation(modExp, a, exp, n);
        int jacobi = jacobiSymbol(a, n);
        if (jacobi == 0 || mpz_cmp_ui(modExp, (jacobi + mpz_get_ui(n)) % mpz_get_ui(n)) != 0) return 0;
    }

    mpz_clears(a, n_minus1, exp, gcd, modExp, NULL);
    gmp_randclear(state);
    return 1;
}

// Main
int main() {
    mpz_t n;
    int iterations;

    mpz_init(n);

    printf("Inserisci il numero da testare: ");
    gmp_scanf("%Zd", n);

    printf("Inserisci il numero di iterazioni k: ");
    scanf("%d", &iterations);

    if (solovayStrassen(n, iterations)) {
        gmp_printf("%Zd e' molto probabilmente primo.\n", n);
    } else {
        gmp_printf("%Zd non e' primo.\n", n);
    }

    mpz_clear(n);
    return 0;
}
 
