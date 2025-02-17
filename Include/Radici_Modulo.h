#include <time.h>
#include "Bezout.h"

int radice_mod (mpz_t, mpz_t, mpz_t);

// calcolo in x, se esiste, la radice quadrata di a modulo p, con p primo. Se esiste restituisce 1, altrimenti 0.
int radice_mod (mpz_t x, mpz_t a, mpz_t p) {
    mpz_t a_modp;
    mpz_init(a_modp);
    mpz_fdiv_r(a_modp,a,p);

    // caso p=2:
    if (mpz_cmp_si(p,2)==0) {
        if (mpz_cmp_si(a_modp,0)==0) mpz_set_si(x,0); // se a==0 mod 2 la radice è 0
        else mpz_set_si(x,1); // se a==1 mod 2 la radice è 1
        mpz_clear(a_modp);
        return 1;
    }
    // caso a==0 mod p: la radice è 0
    if (mpz_cmp_si(a_modp,0)==0) {
        mpz_set_si(x,0);
        mpz_clear(a_modp);
        return 1;
    }

    // controllo che a sia un quadrato mod p: se il simbolo di jacobi è diverso da 1, a non è un quadrato
    int j = mpz_jacobi(a_modp,p);
    if (j!=1) {
        mpz_clear(a_modp);
        return 0;
    }

    // per numeri casuali:
    gmp_randstate_t randstate;
    gmp_randinit_default(randstate);
    unsigned long int t = time(NULL);
    gmp_randseed_ui(randstate, t);

    unsigned long int h;
    mpz_t d,alpha,beta,a_inv,exp,temp;
    mpz_inits(d,alpha,beta,a_inv,temp,NULL);
    inv_mod(a_inv,a,p);

    // scrivo p-1 = 2^h *d, con d dispari
    mpz_sub_ui(d,p,1);
    h = mpz_scan1(d,0);
    mpz_fdiv_q_2exp(d,d,h);

    // cerco un non-quadrato
    do {
        mpz_urandomm(beta,randstate,p); // genero un numero casuale tra 0 e p-1
    } while (mpz_jacobi(beta,p)!=-1);

    exp_mod(beta,beta,d,p); // inizializzo beta=(non-quadrato)^d
    // inizializzo x=a^((d+1)/2), ossia x^2*a^-1=a^d radice 2^(h-1)-esima di 1
    mpz_add_ui(temp,d,1);
    mpz_fdiv_q_2exp(temp,temp,1);
    exp_mod(x,a_modp,temp,p);
    // inizializzo alpha=x^2*a^-1
    exp_mod(alpha,a_modp,d,p);

    h--;
    mpz_init_set_si(exp,1);
    mpz_mul_2exp(exp,exp,h);

/*  Parto con x=a^((d+1)/2) che so essere tale che alpha=x^2*a^-1 è una radice 2^(h-1)-esima di 1 (dove p-1=2^h *d); voglio arrivare a h=1, ossia
    con alpha radice 1-esima di 1 ==> alpha=1, da cui x radice di a (la condizione nel ciclo è h>0 e non h>1 poiché ho già diminuito h di 1).
    Ad ogni iterazione so che alpha è radice 2^(h-1)-esima di 1, e testo se alpha è anche radice 2^(h-2)-esima di 1 (qui exp=2^(h-1)): se lo è
    proseguo lasciando x e alpha invariati; se invece non lo è, devo correggere x che diventa x*beta, dove beta sarà radice 2^h-esima primitiva
    di 1, e aggiornare alpha di conseguenza*/

    while (h>0) {
        // aggiorno h e exp
        h--;
        mpz_fdiv_q_2exp(exp,exp,1);
        exp_mod(temp,alpha,exp,p);
        // testo su alpha
        if (mpz_cmp_si(temp,1)!=0) { // test fallito: aggiorno x e alpha
            mpz_mul(x,x,beta);
            mpz_fdiv_r(x,x,p);

            mpz_mul(alpha,x,x);
            mpz_mul(alpha,alpha,a_inv);
            mpz_fdiv_r(alpha,alpha,p);
        }

        mpz_mul(beta,beta,beta); // aggiorno beta che diventa il suo quadrato
        mpz_fdiv_r(beta,beta,p);
    }
    // quando esco h=0 ossia alpha=x^2*a^-1 è radice 1-esima di 1, quindi x radice di a

    mpz_clears(a_modp,d,alpha,beta,a_inv,exp,temp,NULL);
    gmp_randclear(randstate);
    return 1;
}
