#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

// Struttura per memorizzare un baby step: (valore, esponente)
typedef struct {
    mpz_t value;            // memorizza a^j mod m
    unsigned long exponent; // esponente j
} Pair;

// Funzione di confronto per qsort e bsearch (ordinamento in base a value)
int cmp_pair(const void* a, const void* b) {
    const Pair* pa = (const Pair*)a;
    const Pair* pb = (const Pair*)b;
    return mpz_cmp(pa->value, pb->value);
}

/*
 * Funzione babyStepGiantStep:
 *  - result: (mpz_t) verrà impostato con la soluzione x, se trovata.
 *  - a, b, m: valori in mpz_t che definiscono l'equazione a^x ? b mod m.
 *
 * Restituisce 1 se la soluzione è trovata, 0 altrimenti.
 */
int babyStepGiantStep(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t m) {
    // Calcoliamo order = m - 1 (nel caso in cui m sia primo)
    mpz_t order;
    mpz_init(order);
    mpz_sub_ui(order, m, 1);

    // Calcoliamo n = ceil(sqrt(order))
    double order_d = mpz_get_d(order);
    double n_d = sqrt(order_d);
    unsigned long n = (unsigned long)ceil(n_d);

    // Allocazione dell'array per i baby step (di dimensione n)
    Pair* baby = malloc(n * sizeof(Pair));
    if (!baby) {
        fprintf(stderr, "Errore di allocazione della memoria.\n");
        mpz_clear(order);
        return 0;
    }

    // Inizializziamo i baby step: baby[j].value = a^j mod m, per j = 0,..., n-1.
    mpz_t cur;
    mpz_init_set_ui(cur, 1); // a^0 mod m = 1
    for (unsigned long j = 0; j < n; j++) {
        mpz_init(baby[j].value);
        mpz_set(baby[j].value, cur);
        baby[j].exponent = j;
        // cur = cur * a mod m
        mpz_mul(cur, cur, a);
        mpz_mod(cur, cur, m);
    }
    mpz_clear(cur);

    // Ordiniamo la tabella dei baby step
    qsort(baby, n, sizeof(Pair), cmp_pair);

    // Calcoliamo a^n mod m
    mpz_t a_n;
    mpz_init(a_n);
    mpz_powm_ui(a_n, a, n, m);

    // Calcoliamo factor = (a^n)^{-1} mod m
    mpz_t factor;
    mpz_init(factor);
    if (mpz_invert(factor, a_n, m) == 0) {
        fprintf(stderr, "Inversione modulo fallita.\n");
        for (unsigned long j = 0; j < n; j++) {
            mpz_clear(baby[j].value);
        }
        free(baby);
        mpz_clear(a_n);
        mpz_clear(factor);
        mpz_clear(order);
        return 0;
    }
    mpz_clear(a_n);

    // Impostiamo gamma = b
    mpz_t gamma;
    mpz_init_set(gamma, b);

    // Loop sui giant step: per ogni i = 0,..., n, cerchiamo gamma nei baby step
    for (unsigned long i = 0; i <= n; i++) {
        // Prepariamo una chiave temporanea per la ricerca
        Pair key;
        mpz_init(key.value);
        mpz_set(key.value, gamma);
        key.exponent = 0; // non rilevante

        Pair* found = bsearch(&key, baby, n, sizeof(Pair), cmp_pair);
        mpz_clear(key.value);

        if (found != NULL) {
            // Soluzione trovata: x = i * n + found->exponent
            unsigned long x = i * n + found->exponent;
            mpz_set_ui(result, x);

            // Liberiamo la memoria allocata per i baby step
            for (unsigned long j = 0; j < n; j++) {
                mpz_clear(baby[j].value);
            }
            free(baby);
            mpz_clear(gamma);
            mpz_clear(factor);
            mpz_clear(order);
            return 1;
        }
        // Aggiorniamo gamma: gamma = gamma * factor mod m
        mpz_mul(gamma, gamma, factor);
        mpz_mod(gamma, gamma, m);
    }

    // Nessuna soluzione trovata: liberiamo la memoria e restituiamo 0.
    for (unsigned long j = 0; j < n; j++) {
        mpz_clear(baby[j].value);
    }
    free(baby);
    mpz_clear(gamma);
    mpz_clear(factor);
    mpz_clear(order);
    return 0;
}

int main(void) {
    // Inizializziamo le variabili GMP
    mpz_t a, b, m, result;
    mpz_inits(a, b, m, result, NULL);

    // Lettura degli input
    gmp_printf("Inserisci a: ");
    gmp_scanf("%Zd", a);
    gmp_printf("Inserisci b: ");
    gmp_scanf("%Zd", b);
    gmp_printf("Inserisci m: ");
    gmp_scanf("%Zd", m);

    // Calcoliamo il logaritmo discreto
    if (babyStepGiantStep(result, a, b, m)) {
        gmp_printf("Soluzione trovata: x = %Zd\n", result);
    }
    else {
        gmp_printf("Nessuna soluzione trovata per l'equazione %Zd^x ? %Zd (mod %Zd).\n", a, b, m);
    }

    // Pulizia
    mpz_clears(a, b, m, result, NULL);
    return 0;
}
