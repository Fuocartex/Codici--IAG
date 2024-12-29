#include <string.h>
#include "mcd.h"
#include "Bezout.h"

// Numero di primi da utilizzare nella fattorizzazione di n: deve essere compreso tra 1 e 18 (poiché ho salvato solo una lista dei primi 18
// numeri primi); A SEGUITO DI TEST SCRIVERò QUI VALORI RAGIONEVOLI
#define NUM_PRIMES 4

int lenstra (mpz_t, mpz_t);

// funzioni ausiliarie per algoritmo di Lenstra
int double_elliptic (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
int sum_elliptic (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
int multiply_elliptic (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, int, mpz_t, mpz_t);
void hasse (mpz_t, mpz_t);
void create_exp_primes (unsigned int*, int*, mpz_t);
void calcola_b (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
void calcola_delta (mpz_t, mpz_t, mpz_t, mpz_t);

// Fattorizza l'intero positivo n utilizzando l'algoritmo di Lenstra basato su curve ellittiche: se riesce restituisce 1 e salva in
// d un divisore proprio di n, altrimenti restituisce 0.
int lenstra (mpz_t d, mpz_t n) {

    // salvo una lista di primi da utilizzare nel tentativo di fattorizzare n: scelgo i primi 18 poiché la funzione mpz_get_str, che uso
    // per calcolare il logaritmo base p che serve per la stima di Hasse, accetta come secondo argomento solo numeri da 2 a 62. Teoricamente si
    // può aumentare, ma bisogna trovare un modo alternativo per calcolare la stima, e in ogni caso solitamente bastano pochi primi.
    int primes[18] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};

    mpz_t r;
    mpz_init(r);
    // faccio due prime divisioni test: nell'algoritmo serve n dispari, e sul libro di Koblitz chiede che i primi che dividano n siano >3.
    mpz_fdiv_r_ui(r,n,2);
    if (mpz_cmp_si(r,0)==0) {
        mpz_set_ui(d,2);
        mpz_clear(r);
        return 1;
    }
    mpz_fdiv_r_ui(r,n,3);
    if (mpz_cmp_si(r,0)==0) {
        mpz_set_ui(d,3);
        mpz_clear(r);
        return 1;
    }
    mpz_clear(r);

    mpz_t hasse_n,x_P,y_P,x_iter,y_iter,a,b,delta; // x_P,y_P coordinate del punto P sulla curva ellittica y^2=x^3+ax+b
    mpz_inits(hasse_n,x_P,y_P,x_iter,y_iter,a,b,delta,NULL);
    unsigned int exp_primes[NUM_PRIMES];
    create_exp_primes(exp_primes,primes,hasse_n); // inizializzo gli esponenti a cui elevare ciascun primo
    mpz_clear(hasse_n);

    // inizio l'algoritmo di Lenstra

    // (ciclo su x_iter e y_iter poiché nel corso dell'algoritmo x_P e y_P vengono modificati)
    // ciclo sulla componente x del punto P da 0 a n-1
    while (mpz_cmp(x_iter,n)<0) {
        mpz_set_ui(y_iter,1);

        // ciclo sulla componente y del punto P da 1 a n-1 (parte da 1 altrimenti il primo raddoppio non è definito)
        while (mpz_cmp(y_iter,n)<0) {
            mpz_set_ui(a,0);

            // ciclo sulla scelta di a tra 0 e n-1
            while (mpz_cmp(a,n)<0) {

                mpz_set(x_P,x_iter);
                mpz_set(y_P,y_iter);

                calcola_b(b,x_P,y_P,a,n); // calcolo b affinché P sia sulla curva ellittica
                calcola_delta(delta,a,b,n); // calcolo discriminante della curva ellittica (mod n)

                if (mpz_cmp_si(delta,0)==0) goto retry; // se delta=0 cambio a e riprovo

                mcd_binario(d,delta,n);

                if (mpz_cmp_si(d,1)!=0) { // se mcd(delta,n) diverso da 1, ho trovato un divisore proprio (ho escluso mcd=n nella condizione precedente)
                    return 1;
                }

                // inizio i calcoli P --> m*P

                int i,j,outcome;
                for (i=0; i<exp_primes[0]; i++) { // calcolo P--> 2^(e_2)*P

                    outcome=double_elliptic(d,x_P,y_P,x_P,y_P,a,n);
                    if (outcome==-1) goto retry; // se non posso raddoppiare cambio a e riprovo
                    if (outcome==0) return 1; // successo: ho trovato in d un divisore proprio di n
                }

                // calcolo le potenze con i primi della lista maggiori di 2
                for (j=1; j<NUM_PRIMES; j++) {
                    for (i=0; i<exp_primes[j]; i++) { // calcolo P-->k^(e_k)*P, con k primo

                        outcome=multiply_elliptic(d,x_P,y_P,x_P,y_P,primes[j],a,n);
                        if (outcome==-1) goto retry; // se non posso moltiplicare cambio a e riprovo
                        if (outcome==0) return 1; // successo: ho trovato in d un divisore proprio di n
                    }
                }

                // se arrivo qui ho calcolato mP senza trovare divisori: cambio a e riprovo

                retry: mpz_add_ui(a,a,1); // sommo 1 ad a e riprovo
            }

            mpz_add_ui(y_iter,y_iter,1); // sommo 1 a y e riprovo
        }

        mpz_add_ui(x_iter,x_iter,1); // sommo 1 a x e riprovo
    }

    // se arrivo qui non ho trovato divisori di n per alcun P e alcun a: restituisco 0

    mpz_clears(x_P,y_P,x_iter,y_iter,a,b,delta,NULL);
    return 0;
}

// tenta di calcolare P-->2P sulla curva ellittica; per farlo serve calcolare l'inverso di 2*y_P mod n, quindi si hanno i casi: se mcd(2y_P,n)=n allora
// restituisce -1 (dovrò cambiare curva); se mcd(2y_P,n)=1 il calcolo può essere portato a termine e restituisco 1; altrimenti ho trovato un divisore
// proprio d di n, e restituisco 0.
int double_elliptic (mpz_t d, mpz_t x_2P, mpz_t y_2P, mpz_t x_P, mpz_t y_P, mpz_t a, mpz_t n) {
    mpz_t y_P2_inv,temp1;
    mpz_inits(y_P2_inv,temp1,NULL);
    mpz_mul_si(temp1,y_P,2);
    inv_mod_mcd(d,y_P2_inv,temp1,n);
    if (mpz_cmp(d,n)==0) { // fallimento: restituisco -1
        mpz_clears(y_P2_inv,temp1,NULL);
        return -1;
    }
    if (mpz_cmp_si(d,1)!=0) { // successo: restituisco 0
        mpz_clears(y_P2_inv,temp1,NULL);
        return 0;
    }
    // calcolo 2P: vedi Koblitz per le formule
    mpz_t temp2,x_P_copy,y_P_copy; // creo copie per non modificare x_P e y_P nei calcoli
    mpz_inits(temp2,x_P_copy,y_P_copy,NULL);
    mpz_set(x_P_copy,x_P); mpz_set(y_P_copy,y_P);
    mpz_mul(temp1,x_P_copy,x_P_copy);
    mpz_mul_si(temp1,temp1,3);
    mpz_add(temp1,temp1,a);
    mpz_fdiv_r(temp1,temp1,n);
    mpz_mul(temp1,temp1,y_P2_inv);
    mpz_fdiv_r(temp1,temp1,n);
    mpz_mul(x_2P,temp1,temp1);
    mpz_fdiv_r(x_2P,x_2P,n);
    mpz_mul_si(temp2,x_P_copy,2);
    mpz_sub(x_2P,x_2P,temp2);
    mpz_fdiv_r(x_2P,x_2P,n);
    mpz_sub(temp2,x_P_copy,x_2P);
    mpz_mul(y_2P,temp1,temp2);
    mpz_sub(y_2P,y_2P,y_P_copy);
    mpz_fdiv_r(y_2P,y_2P,n);

    mpz_clears(y_P2_inv,temp1,temp2,x_P_copy,y_P_copy,NULL);
    return 1;
}

// tenta di calcolare P+Q sulla curva ellittica; per farlo serve calcolare l'inverso di x_Q-x_P mod n, quindi si hanno i casi: se mcd(x_Q-x_P,n)=n allora
// restituisce -1 (dovrò cambiare curva); se mcd(x_Q-x_P,n)=1 il calcolo può essere portato a termine e restituisco 1; altrimenti ho trovato un divisore
// proprio d di n, e restituisco 0.
int sum_elliptic (mpz_t d, mpz_t x_sum, mpz_t y_sum, mpz_t x_P, mpz_t y_P, mpz_t x_Q, mpz_t y_Q, mpz_t n) {
    mpz_t denom,temp1;
    mpz_inits(denom,temp1,NULL);
    mpz_sub(temp1,x_Q,x_P);
    inv_mod_mcd(d,denom,temp1,n);
    if (mpz_cmp(d,n)==0) { // fallimento: restituisco -1
        mpz_clears(denom,temp1,NULL);
        return -1;
    }
    if (mpz_cmp_si(d,1)!=0) { // successo: restituisco 0
        mpz_clears(denom,temp1,NULL);
        return 0;
    }
    // calcolo P+Q: vedi Koblitz per le formule
    mpz_t temp2,x_P_copy,y_P_copy,x_Q_copy,y_Q_copy; // creo copie per non modificare le coordinate dei punti nei calcoli
    mpz_inits(temp2,x_P_copy,y_P_copy,x_Q_copy,y_Q_copy,NULL);
    mpz_set(x_P_copy,x_P); mpz_set(y_P_copy,y_P); mpz_set(x_Q_copy,x_Q); mpz_set(y_Q_copy,y_Q);
    mpz_sub(temp1,y_Q_copy,y_P_copy);
    mpz_mul(temp1,temp1,denom);
    mpz_fdiv_r(temp1,temp1,n);
    mpz_mul(x_sum,temp1,temp1);
    mpz_sub(x_sum,x_sum,x_P_copy);
    mpz_sub(x_sum,x_sum,x_Q_copy);
    mpz_fdiv_r(x_sum,x_sum,n);
    mpz_sub(temp2,x_P_copy,x_sum);
    mpz_mul(y_sum,temp1,temp2);
    mpz_sub(y_sum,y_sum,y_P_copy);
    mpz_fdiv_r(y_sum,y_sum,n);

    mpz_clears(denom,temp1,temp2,x_P_copy,y_P_copy,x_Q_copy,y_Q_copy,NULL);
    return 1;
}

// tenta di calcolare P-->kP, con k>2 intero, utilizzando raddoppi ripetuti (analogo a esponenziazione binaria). Nel farlo deve utilizzare le formule
// per raddoppio e per somma su curve ellittiche: se falliscono restituisco -1 e dovrò cambiare curva; se trovo un divisore proprio d di n nel processo
// restituisco 0; altrimenti porto a termine il calcolo e restituisco 1.
int multiply_elliptic (mpz_t d, mpz_t x_kP, mpz_t y_kP, mpz_t x_P, mpz_t y_P, int k, mpz_t a, mpz_t n) {
    int outcome;
    mpz_t temp1,temp2;
    mpz_inits(temp1,temp2,NULL);
    mpz_set(temp1,x_P); mpz_set(temp2,y_P); // temp conterranno man mano le coordinate dei raddoppi successivi di P
    mpz_set_si(x_kP,0); mpz_set_si(y_kP,0);

    while (k>0) {
        if (k&1==1) { // controllo che l'ultimo bit di k sia un 1; se lo è sommo, altrimenti lascio così

            outcome=sum_elliptic(d,x_kP,y_kP,x_kP,y_kP,temp1,temp2,n);
            if (outcome==-1) return -1; // somma non riuscita: devo cambiare curva
            if (outcome==0) return 0; // successo: ho trovato il divisore d
            // altrimenti ho portato a termine la somma con successo, e proseguo i calcoli
        }
        // aggiorno il raddoppio
        outcome=double_elliptic(d,temp1,temp2,temp1,temp2,a,n);
        if (outcome==-1) return -1; // raddoppio non riuscito: devo cambiare curva
        if (outcome==0) return 0; // successo: ho trovato il divisore d
        // altrimenti ho portato a termine il raddoppio con successo, e proseguo i calcoli

        k>>=1; // shifto di 1 i bit di k
    }
    // se esco dal ciclo ho portato a termine la moltiplicazione P-->kP con successo

    mpz_clears(temp1,temp2,NULL);
    return 1;
}

// calcola in x la stima di Hasse per n: hasse(n)= (radice_quarta(n) + 1)^2
void hasse (mpz_t x, mpz_t n) {
    mpz_root(x,n,4);
    mpz_add_ui(x,x,2); // sommo 2 poiché mpz_root restituisce la radice troncata alla parte intera
    mpz_mul(x,x,x);

    return;
}

// calcola gli esponenti a cui deve essere elevato ciascun primo: exp=parte intera di log_(base p) (hasse_n)
void create_exp_primes (unsigned int* exponents, int* primes, mpz_t hasse_n) {
    int i;
    for (i=0; i<NUM_PRIMES; i++) {
        // trucco per calcolare il logaritmo: mpz_get_str restituisce una stringa contenente la rappresentazione di hasse_n in base
        // primes[i]; per avere la parte intera del logaritmo base p basta quindi calcolare la lunghezza della stringa meno 1
        exponents[i]=strlen(mpz_get_str(NULL,primes[i],hasse_n))-1;
    }

    return;
}

// calcola il termine noto b in Z/(n) tale che il punto (x,y) sia sulla curva ellittica y^2=x^3+ax+b, ossia b=y^2-x^3-ax mod n
void calcola_b (mpz_t b, mpz_t x, mpz_t y, mpz_t a, mpz_t n) {
    mpz_t temp;
    mpz_init(temp);
    mpz_mul(b,y,y);
    mpz_fdiv_r(b,b,n);
    exp_mod_ui(temp,x,3,n);
    mpz_sub(b,b,temp);
    mpz_mul(temp,a,x);
    mpz_sub(b,b,temp);
    mpz_fdiv_r(b,b,n);

    mpz_clear(temp);
    return;
}

// calcola il discriminante della curva ellittica: delta= 27*a^3 +4*b^2 mod n
void calcola_delta (mpz_t delta, mpz_t a, mpz_t b, mpz_t n) {
    mpz_t temp;
    mpz_init(temp);
    mpz_mul_ui(delta,a,3);
    exp_mod_ui(delta,delta,3,n);
    mpz_mul_ui(temp,b,2);
    mpz_mul(temp,temp,temp);
    mpz_add(delta,delta,temp);
    mpz_fdiv_r(delta,delta,n);

    mpz_clear(temp);
    return;
}
