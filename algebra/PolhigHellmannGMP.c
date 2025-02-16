#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>

// Funzione per calcolare il MCD con algoritmo di Euclide
// usa la libreria GMP (mpz_gcd)
// Algoritmo di Euclide esteso per trovare l'inverso modulo
void mod_inverse(mpz_t result, mpz_t a, mpz_t m) {
    mpz_invert(result, a, m);
}

// Esponenziazione modulare binaria
// calcola base^exp % mod, e utilizza la funzione GMP mpz_pow
void mod_exp(mpz_t result, mpz_t base, mpz_t exp, mpz_t mod) {
    mpz_t b, e;
    mpz_init_set(b, base);
    mpz_init_set(e, exp);
    mpz_set_ui(result, 1);
    
    while (mpz_cmp_ui(e, 0) > 0) {
        if (mpz_odd_p(e))
            mpz_mul(result, result, b), mpz_mod(result, result, mod);
        mpz_fdiv_q_2exp(e, e, 1);
        mpz_mul(b, b, b), mpz_mod(b, b, mod);
    }
    
    mpz_clear(b);
    mpz_clear(e);
}

// Algoritmo di Baby-Step Giant-Step per trovare il logaritmo discreto modulo un primo q
// L'algoritmo è una tecnica di ricerca per trovare x tale che: g^x = h (mod p)
// dove g è la base, h è il valore di cui vogliamo trovare il logaritmo, e p è un modulo primo. 
// L'algoritmo divide il problema in due parti:
// La baby-step (precalcolo) crea una tabella con i valori di g^(j*m) % p per j da 0 a m, dove m è la radice quadrata di q (approssimato).
// La giant-step calcola successivamente i valori h * g^(-i*m) % p e cerca se uno di questi è nella tabella.
// Se trova una corrispondenza, restituisce la soluzione come la somma degli indici i e j. Se non trova nulla, restituisce 0.
void baby_giant_step(mpz_t result, mpz_t g, mpz_t h, mpz_t p) {
    mpz_t m, val, g_m, inv_g_m, temp;
    mpz_init(m);
    mpz_sqrt(m, p);
    mpz_add_ui(m, m, 1);
    
    mpz_init_set_ui(val, 1);
    mpz_init(temp);
    
    // Creazione della tabella dei baby steps
    size_t m_ui = mpz_get_ui(m);
    mpz_t *table = malloc(m_ui * sizeof(mpz_t));
    for (size_t i = 0; i < m_ui; i++) {
        mpz_init_set(table[i], val);
        mpz_mul(val, val, g);
        mpz_mod(val, val, p);
    }
    
    // Calcola g^(-m) mod p
    mpz_init(g_m);
    mpz_invert(g_m, g, p);
    mpz_init(inv_g_m);
    mod_exp(inv_g_m, g_m, m, p);
    
    mpz_set(val, h);
    for (size_t i = 0; i < m_ui; i++) {
        for (size_t j = 0; j < m_ui; j++) {
            if (mpz_cmp(table[j], val) == 0) {
                mpz_set_ui(result, i * m_ui + j);
                goto cleanup;
            }
        }
        mpz_mul(val, val, inv_g_m);
        mpz_mod(val, val, p);
    }
    mpz_set_ui(result, 0);
    
cleanup:
    for (size_t i = 0; i < m_ui; i++) {
        mpz_clear(table[i]);
    }
    free(table);
    mpz_clears(m, val, g_m, inv_g_m, temp, NULL);
}

// Risoluzione del logaritmo discreto modulo q^e usando il metodo di Pohlig-Hellman
// approccio efficiente quando il modulo è fattorizzato come un prodotto di potenze di primi piccoli
// Questa funzione sfrutta la fattorizzazione del modulo p come un prodotto di potenze di primi. 
// La funzione esegue i seguenti passi:
// Calcola q^e (dove q è un primo e e è l'esponente corrispondente alla potenza di q).
// Calcola i valori di g_i e h_i come potenze di g e h modulo p, utilizzando q^e.
// Risolve il logaritmo discreto di h_i rispetto a g_i modulo q^e usando l'algoritmo di Baby-Step Giant-Step.
// Restituisce il risultato parziale del logaritmo discreto modulo q^e.
// Funzione per Pohlig-Hellman per un fattore q^e
void pohlig_hellman(mpz_t result, mpz_t g, mpz_t h, mpz_t p, mpz_t q, int exp) {
    mpz_t q_e, exponent, g_i, h_i;
    mpz_inits(q_e, exponent, g_i, h_i, NULL);
    
    // q_e = q^exp
    mpz_pow_ui(q_e, q, exp);
    // Calcola l'esponente: (p-1) / q^exp
    mpz_sub_ui(exponent, p, 1);
    mpz_divexact(exponent, exponent, q_e);
    // g_i = g^( (p-1)/q^exp ) mod p, h_i = h^( (p-1)/q^exp ) mod p
    mod_exp(g_i, g, exponent, p);
    mod_exp(h_i, h, exponent, p);
    
    baby_giant_step(result, g_i, h_i, p);
    
    mpz_clears(q_e, exponent, g_i, h_i, NULL);
}

// Teorema Cinese del Resto per combinare i risultati modulo q_i^e_i
// questa funzione combina i risultati del logaritmo discreto calcolati per moduli diversi, ottenuti tramite il metodo di Pohlig-Hellman.
// Il teorema permette di risolvere un sistema di congruenze, restituendo un risultato finale modulo M, dove M è il prodotto di tutti i moduli. 
// La formula del CRT è la seguente:
// x = sum_i{xi*Mi·inv(Mi)) (mod M)
// dove M_i è il prodotto dei moduli escluso l' i-esimo, e inv(M_i) è l'inverso di M_i modulo il modulo corrente.
void chinese_remainder(mpz_t result, mpz_t *x, mpz_t *m, size_t len) {
    mpz_t M, Mi, inv, sum;
    mpz_init_set_ui(M, 1);
    for (size_t i = 0; i < len; i++) {
        mpz_mul(M, M, m[i]);
    }
    
    mpz_init_set_ui(sum, 0);
    for (size_t i = 0; i < len; i++) {
        mpz_init(Mi);
        mpz_divexact(Mi, M, m[i]);
        mpz_init(inv);
        mod_inverse(inv, Mi, m[i]);
        mpz_mul(Mi, Mi, inv);
        mpz_mul(Mi, Mi, x[i]);
        mpz_add(sum, sum, Mi);
        mpz_clear(Mi);
        mpz_clear(inv);
    }
    mpz_mod(result, sum, M);
    
    mpz_clears(M, sum, NULL);
}

// Algoritmo di Pohlig-Hellman completo
// Funzione principale per il logaritmo discreto
// utilizza tutte le precedenti per risolvere il logaritmo discreto in modo efficiente. 
// Per ogni primo q_i e il corrispondente esponente e_i, calcola il logaritmo discreto modulo q_i^e_i utilizzando il metodo di Pohlig-Hellman.
// Combina i risultati ottenuti usando il Teorema Cinese del Resto.
void pohlig_hellman_algorithm(mpz_t result, mpz_t g, mpz_t h, mpz_t p, mpz_t *q, int *e, size_t len) {
    mpz_t *x = malloc(len * sizeof(mpz_t));
    mpz_t *moduli = malloc(len * sizeof(mpz_t));
    
    for (size_t i = 0; i < len; i++) {
        mpz_init(moduli[i]);
        mpz_pow_ui(moduli[i], q[i], e[i]);  // moduli[i] = q[i]^(e[i])
        mpz_init(x[i]);
        pohlig_hellman(x[i], g, h, p, q[i], e[i]);
    }
    
    chinese_remainder(result, x, moduli, len);
    
    for (size_t i = 0; i < len; i++) {
        mpz_clear(x[i]);
        mpz_clear(moduli[i]);
    }
    free(x);
    free(moduli);
}

// Main
// Legge i valori di g (base), h (target), e p (modulo).
// Fa inserire all'utente la fattorizzazione di p, con una lista di primi q con i loro esponenti e .
// Chiama pohligHellmanAlgorithm() per calcolare il logaritmo discreto.
// Stampa il risultato finale.
int main() {
    mpz_t g, h, p, result;
    mpz_inits(g, h, p, result, NULL);
    
    printf("Inserisci base g, valore h e modulo p (p primo):\n");
    gmp_scanf("%Zd %Zd %Zd", g, h, p);
    
    // Calcoliamo l'ordine del gruppo: p - 1
    mpz_t order;
    mpz_init(order);
    mpz_sub_ui(order, p, 1);
    
    int num_factors;
    printf("Inserisci il numero di fattori distinti nella fattorizzazione di (p-1): ");
    scanf("%d", &num_factors);
    
    // Allochiamo dinamicamente gli array per i fattori e gli esponenti
    mpz_t *q = malloc(num_factors * sizeof(mpz_t));
    int *exp_array = malloc(num_factors * sizeof(int));
    
    for (int i = 0; i < num_factors; i++) {
        mpz_init(q[i]);
        printf("Fattore primo #%d: ", i + 1);
        gmp_scanf("%Zd", q[i]);
        printf("Esponente per questo fattore: ");
        scanf("%d", &exp_array[i]);
    }
    
    pohlig_hellman_algorithm(result, g, h, p, q, exp_array, num_factors);
    gmp_printf("Logaritmo discreto x trovato: %Zd\n", result);
    
    // Pulizia della memoria GMP
    mpz_clear(order);
    mpz_clears(g, h, p, result, NULL);
    for (int i = 0; i < num_factors; i++) {
        mpz_clear(q[i]);
    }
    free(q);
    free(exp_array);
    
    return 0;
}
