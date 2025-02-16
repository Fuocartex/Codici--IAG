#include <stdio.h> //per printf, scanf
#include <math.h> //per funzioni matematiche (pow, sqrt, ceil)
#include <stdlib.h> //per gestione dinamica della memoria (malloc, free)

// Il problema che stiamo risolvendo è il logaritmo discreto, cioè trovare x tale che:
// g^x = h mod p dove:
// g è la base (un generatore del gruppo moltiplicativo modulo p)
// h è il valore noto
// p è un numero primo (o comunque l'ordine del gruppo in cui stiamo lavorando)
// Se p-1 è composto, possiamo sfruttare il teorema di Lagrange e il teorema cinese del resto (CRT)
// per spezzare il problema in sottoproblemi più piccoli.

// Esponenziazione modulare binaria
// Funzione che calcola base^exp mod mod in modo efficiente con esponenziazione modulare veloce.
// Vantaggi:
// Evita overflow usando la proprietà (a·b) mod c = [(a mod c)·(b mod c)] mod c
// Usa esponenziazione binaria che riduce il numero di moltiplicazioni da O(exp) a O(log(exp)).
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Algoritmo di Euclide esteso per trovare l'inverso modulo
/* Questa funzione calcola l'inverso moltiplicativo di a modulo m, ovvero x tale che: a·x = 1 mod m
Funzionamento:
Si usa l'algoritmo di Euclide esteso per trovare i coefficienti di Bézout x,y che soddisfano: a·x + m·y = mcd (a,m)
Se mcd(a,m) = 1, allora x è l'inverso di a modulo m.
*/
unsigned long long mod_inverse(unsigned long long a, unsigned long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;

    return x1;
}

// Algoritmo di Baby-Step Giant-Step
/* Si divide il problema in due passaggi:
Baby step: si calcola una tabella di valori g^j per j=0,…,m-1.
Giant step: si cerca un valore g^(-im)·h che corrisponda a uno di quelli nella tabella.
m = sqrt(p), che riduce la complessità a O(p).
*/
unsigned long long baby_giant_step(unsigned long long g, unsigned long long h, unsigned long long p) {
    unsigned long long m = ceil(sqrt(p)) + 1;
    unsigned long long *table = (unsigned long long *)malloc(m * sizeof(unsigned long long));
    unsigned long long val, inv_g_m;
    unsigned long long i, j;

    // Precalcola la tabella: table[i] = g^i mod p per i = 0,..., m-1
    val = 1;
    for (i = 0; i < m; i++) {
        table[i] = val;
        val = (val * g) % p;
    }

    // Calcola g^(-m) mod p usando l'inverso modulare di g
    unsigned long long inv_g = mod_inverse(g, p);
    inv_g_m = mod_exp(inv_g, m, p);
    //Cerca g^(-im)·h nella tabella. Se trovato, restituisce x = i·m + j.
    val = h;
    for (i = 0; i < m; i++) {
        for (j = 0; j < m; j++) {
            if (table[j] == val) {
                free(table);
                return i * m + j;
            }
        }
        val = (val * inv_g_m) % p;
    }

    free(table);
    return 0;
}

// Logaritmo discreto modulo q^e (algoritmo di Pohlig-Hellman per un fattore)
unsigned long long pohlig_hellman(unsigned long long g, unsigned long long h, unsigned long long p, unsigned long long q, int exp) {
    unsigned long long q_e = 1;
    int i;
    for (i = 0; i < exp; i++) {
        q_e *= q;
    }
    // Calcola l'esponente (p-1) / q^e
    unsigned long long exponent = (p - 1) / q_e;
    unsigned long long g_i = mod_exp(g, exponent, p);
    unsigned long long h_i = mod_exp(h, exponent, p);
    return baby_giant_step(g_i, h_i, p);
}

// Teorema Cinese del resto per ricostruire x modulo p-1
// Calcola il prodotto M dei moduli m[i]. Per ogni congruenza x = a_i mod m, calcola la soluzione combinata.
unsigned long long chinese_remainder(unsigned long long *x, unsigned long long *m, int len) {
    unsigned long long M = 1, result = 0;
    unsigned long long i, Mi, inv;

    for (i = 0; i < len; i++) {
        M *= m[i];
    }

    for (i = 0; i < len; i++) {
        Mi = M / m[i];
        inv = mod_inverse(Mi, m[i]);
        result += x[i] * Mi * inv;
    }

    return result % M;
}

// Algoritmo di Pohlig-Hellman completo
// Risolve il logaritmo discreto per ogni fattore primo q^e
unsigned long long pohlig_hellman_algorithm(unsigned long long g, unsigned long long h, unsigned long long p,
                                             unsigned long long *q, int *e, int len) {
    unsigned long long *x = (unsigned long long *)malloc(len * sizeof(unsigned long long));
    unsigned long long *moduli = (unsigned long long *)malloc(len * sizeof(unsigned long long));
    int i, j;
    for (i = 0; i < len; i++) {
        unsigned long long q_e = 1;
        for (j = 0; j < e[i]; j++) {
            q_e *= q[i];
        }
        moduli[i] = q_e;
        x[i] = pohlig_hellman(g, h, p, q[i], e[i]);
    }
    // Combina i risultati con il Teorema Cinese del Resto.
    unsigned long long result = chinese_remainder(x, moduli, len);

    free(x);
    free(moduli);

    return result;
}

int main() {
    unsigned long long g, h, p;
    printf("Inserisci base g, valore h e modulo p (p primo):\n");
    scanf("%llu %llu %llu", &g, &h, &p);

    // L'ordine del gruppo moltiplicativo è p-1.
    // L'utente deve inserire la fattorizzazione di p-1: ad esempio, se p=11 allora p-1=10=2*5.
    int num_factors;
    printf("Inserisci il numero di fattori distinti nella fattorizzazione di (p-1): ");
    scanf("%d", &num_factors);

    unsigned long long *q = (unsigned long long *)malloc(num_factors * sizeof(unsigned long long));
    int *e = (int *)malloc(num_factors * sizeof(int));
    int i;
    for (i = 0; i < num_factors; i++) {
        printf("Fattore primo #%d: ", i + 1);
        scanf("%llu", &q[i]);
        printf("Esponente per %llu: ", q[i]);
        scanf("%d", &e[i]);
    }

    unsigned long long result = pohlig_hellman_algorithm(g, h, p, q, e, num_factors);
    printf("Logaritmo discreto x trovato: %llu\n", result);
    printf("Verifica: %llu^%llu mod %llu = %llu (dovrebbe essere %llu)\n",
           g, result, p, mod_exp(g, result, p), h);

    free(q);
    free(e);

    return 0;
}

