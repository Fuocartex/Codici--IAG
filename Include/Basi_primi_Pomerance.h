#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>
#include "mcd.h"



// funzioni ausiliarie
mpz_t* cont_frac_sqrt_mod(mpz_t, unsigned long int);
void riduci_mod_min(mpz_t, mpz_t);

// Restituisce i primi k>0 numeratori dell'approssimazione in frazione continua di sqrt(n) modulo n, espressi con valore assoluto minimo.
mpz_t* cont_frac_sqrt_mod(mpz_t n, unsigned long int k) {
    int i;
    mpz_t a,beta,gamma,temp;
    mpz_inits(a,beta,gamma,temp,NULL);
    mpz_t* b=(mpz_t*)malloc(k*sizeof(mpz_t));
    for (i=0;i<k;i++) mpz_init(b[i]);

    // dati iniziali delle relazioni ricorsive
    mpz_sqrt(a,n);
    mpz_neg(beta,a);
    mpz_set_ui(gamma,1);
    mpz_set(b[0],a);

    // esplicito i primi due termini (poiché utilizzano dati iniziali specifici su b_-1 e b_-2)
    if (k==1) return b;

    mpz_mul(temp,beta,beta);
    mpz_sub(gamma,n,temp); // aggiornato gamma
    mpz_sub(temp,a,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    mpz_mul(temp,a,b[0]);
    mpz_add_ui(b[1],temp,1);
    riduci_mod_min(b[1],n);
    
    if (k==2) return b;

    for (i=2; i<k; i++) {
        mpz_mul(temp,beta,beta);
        mpz_sub(temp,n,temp);
        mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
        mpz_sub(temp,b[0],beta);
        mpz_fdiv_q(a,temp,gamma); // aggiornato a
        mpz_mul(temp,a,gamma);
        mpz_add(temp,temp,beta);
        mpz_neg(beta,temp); // aggiornato beta
        mpz_mul(temp,a,b[i-1]);
        mpz_add(b[i],temp,b[i-2]);
        riduci_mod_min(b[i],n); // aggiornato b[i]
    }
    
    mpz_clears(a,beta,gamma,temp,NULL);
    for (i=0;i<k;i++) mpz_clear(b[i]);
    return b;
}

// riduce a modulo n esprimendolo con valore assoluto minimo (potra' essere positivo o negativo, ma avra' abs(a)<= n/2).
void riduci_mod_min(mpz_t a, mpz_t n){
    mpz_t n_mezzi;
    mpz_init(n_mezzi);
    mpz_fdiv_q_2exp(n_mezzi,n,1);

    mpz_fdiv_r(a,a,n);
    if (mpz_cmp(a,n_mezzi)>0) mpz_sub(a,a,n); // se a>n/2, allora a-->a-n

    mpz_clear(n_mezzi);
    return;
}
