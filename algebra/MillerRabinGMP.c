#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Calcolo MCD con algoritmo di Euclide
void mcd(mpz_t result, const mpz_t a, const mpz_t b) {
    mpz_gcd(result, a, b);
}

// Esponenziazione modulare binaria
void moduloExponentiation(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod) {
    mpz_powm(result, base, exp, mod);
}

// Funzione ausiliaria per verificare se n è composto
int isComposite(const mpz_t n, const mpz_t a, const mpz_t d, long h) {
    mpz_t x, prev, temp;
    mpz_inits(x, prev, temp, NULL);

    // x = a^d mod n
    moduloExponentiation(x, a, d, n);

    // Se x == 1 o x == n - 1, non posso concludere che sia composto
    mpz_sub_ui(temp, n, 1);
    if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, temp) == 0) {
        mpz_clears(x, prev, temp, NULL);
        return 0;
    }

    // Itero attraverso le squadrature
    for (long i = 1; i < h; i++) {
        mpz_set(prev, x);
        mpz_mul(x, x, x);
        mpz_mod(x, x, n);

        // Se x == 1 e prev != n - 1, allora è composto
        if (mpz_cmp_ui(x, 1) == 0) {
            if (mpz_cmp(prev, temp) != 0) {
                mpz_clears(x, prev, temp, NULL);
                return 1;
            } else {
                mpz_clears(x, prev, temp, NULL);
                return 0;
            }
        }

        // Se x == n - 1, esco dal ciclo
        if (mpz_cmp(x, temp) == 0) {
            mpz_clears(x, prev, temp, NULL);
            return 0;
        }
    }

    // Se dopo tutte le iterazioni x ? 1, allora è sicuramente composto
    if (mpz_cmp_ui(x, 1) != 0) {
        mpz_clears(x, prev, temp, NULL);
        return 1;
    }

    mpz_clears(x, prev, temp, NULL);
    return 1;
}

// Test di Miller-Rabin
int millerRabin(const mpz_t n, int iterations) {
    if (mpz_cmp_ui(n, 2) < 0) return 0;
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) return 1;
    if (mpz_even_p(n)) return 0;

    mpz_t d, a, g, temp;
    mpz_inits(d, a, g, temp, NULL);

    // Scriviamo n - 1 come 2^h * d
    mpz_sub_ui(d, n, 1);
    long h = 0;
    while (mpz_even_p(d)) {
        mpz_fdiv_q_2exp(d, d, 1);
        h++;
    }

    gmp_randstate_t state;
    gmp_randinit_mt(state);

    for (int i = 0; i < iterations; i++) {
        // Genera un numero casuale a tra [2, n-2]
        mpz_sub_ui(temp, n, 4);
        mpz_urandomm(a, state, temp);
        mpz_add_ui(a, a, 2);

        // Controllo MCD
        mcd(g, a, n);
        if (mpz_cmp_ui(g, 1) > 0) {
            mpz_clears(d, a, g, temp, NULL);
            gmp_randclear(state);
            return 0;
        }

        if (isComposite(n, a, d, h)) {
            mpz_clears(d, a, g, temp, NULL);
            gmp_randclear(state);
            return 0;
        }
    }

    mpz_clears(d, a, g, temp, NULL);
    gmp_randclear(state);
    return 1;
}

int main() {
    mpz_t n;
    int iterations;

    mpz_init(n);

    printf("Inserisci un numero da verificare: ");
    gmp_scanf("%Zd", n);

    printf("Inserisci il numero di iterazioni del test: ");
    scanf("%d", &iterations);

    if (millerRabin(n, iterations)) {
        gmp_printf("%Zd è molto probabilmente primo.\n", n);
    } else {
        gmp_printf("%Zd è composto.\n", n);
    }

    mpz_clear(n);
    return 0;
}

