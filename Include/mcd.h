#include <gmp.h>

void mcd_euclide (mpz_t, mpz_t, mpz_t);
void mcd_binario (mpz_t, mpz_t, mpz_t);

// massimo comun divisore tra n e m, salvato in a, con algoritmo di Euclide. Il risultato è sempre non negativo.
void mcd_euclide (mpz_t a, mpz_t n, mpz_t m) {
    // creo nuove variabili per non modificare n e m, e le rendo positive per avere mcd>=0
    mpz_t x,y;
    mpz_init(x); mpz_init(y);
    mpz_abs(x,n); mpz_abs(y,m);
    
    while (mpz_cmp_si(y,0)>0) {  // se y>0 continuo a iterare
        mpz_fdiv_r(x,x,y);  // x diventa il resto di x/y
        mpz_swap(x,y);
    }
    mpz_set(a,x);

    mpz_clear(x); mpz_clear(y);
    return;
}

// massimo comun divisore tra n e m, salvato in a, con algoritmo binario. Il risultato è sempre non negativo.
void mcd_binario (mpz_t a, mpz_t n, mpz_t m) {
    // casi base: uno dei due numeri è 0
    if (mpz_cmp_si(n,0)==0) {
        mpz_set(a,m);
        mpz_abs(a,a);
        return;
    }
    else if (mpz_cmp_si(m,0)==0) {
        mpz_set(a,n);
        mpz_abs(a,a);
        return;
    }

    // creo nuove variabili per non modificare n e m, e le rendo positive per avere mcd>=0
    mpz_t x,y;
    mpz_init(x); mpz_init(y);
    mpz_abs(x,n); mpz_abs(y,m);
    
 /* ottimizzazione che ho trovato io facendo qualche test: se i due numeri hanno ordini di grandezza molto diversi l'algoritmo diventa
    inefficiente, quindi eseguo una sola passata dell'algoritmo euclideo facendo una prima divisione con resto. In questo modo i due
    numeri avranno ordine di grandezza simile al più piccolo, e l'algoritmo binario si velocizza molto. */
    if (mpz_cmp(x,y)>0) {
        mpz_fdiv_r(x,x,y);
        if (mpz_cmp_si(x,0)==0) {
            mpz_set(a,y);
            return;
        }
    }
    else {
        mpz_fdiv_r(y,y,x);
        if (mpz_cmp(y,0)==0) {
            mpz_set(a,x);
            return;
        }
    }

    // calcolo (in k) la più alta potenza di 2 che divide i due numeri
    unsigned long int i,j,k;
    i = mpz_scan1(x,0);
    j = mpz_scan1(y,0);
    if (i<j) k=i;
    else k=j;
    
    // rendo dispari i due numeri, avendo già salvato la più alta potenza di 2 che divide entrambi
    mpz_fdiv_q_2exp(x,x,i);
    mpz_fdiv_q_2exp(y,y,j);

    // ciclo lavorando sempre su numeri dispari, ed esco quando uno dei due è 0
    while (mpz_cmp_si(x,0)>0) {
        if (mpz_cmp(x,y)<0) mpz_swap(x,y); // in questo modo x>=y
        mpz_sub(x,x,y);  // eseguo la sottrazione x=x-y; ora x è pari
        i = mpz_scan1(x,0);
        mpz_fdiv_q_2exp(x,x,i);  // tolgo tutte le potenze di 2 che dividono x: così torna dispari
    }

    // quando esco dal ciclo in y ci sarà mcd dei numeri resi dispari; dopo rimoltiplico per la giusta potenza di 2 calcolata prima
    mpz_set(a,y);
    mpz_mul_2exp(a,a,k);

    mpz_clear(x); mpz_clear(y);
    return;
}
