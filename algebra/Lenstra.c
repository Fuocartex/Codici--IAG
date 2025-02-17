#include <stdio.h>
#include "..\Include\Lenstra.h"
#include "..\Include\Fattorizzazione.h"
#include "..\Include\MillerRabin.h"

Factor* fattorizza_Lenstra(mpz_t,Factor*);

int main () {
    mpz_t n;
    mpz_init(n);
    int iter=20;
    printf("Inserisci il numero da fattorizzare:\n");
    gmp_scanf("%Zd",n);

    Factor* fattori=NULL;

    if (millerRabin(n,iter)!=0) fattori=cons(fattori,n,1);
    else fattori=fattorizza_Lenstra(n,fattori);
    print_factors(fattori);

    mpz_clear(n);
    return 0;
}

// fattorizza (tramite l'algoritmo di Lenstra) in maniera ricorsiva l'intero (composto) n, aggiungendo i fattori trovati alla lista
Factor* fattorizza_Lenstra(mpz_t n, Factor* fattori) {
    int iter=20; // iterazioni da fare nel test di Miller-Rabin

    mpz_t d1,d2;
    mpz_inits(d1,d2,NULL);
    
    if (lenstra(d1,n)==0) { // caso in cui non sono riuscito a fattorizzare
        printf("Non sono riuscito a fattorizzare");
        return fattori;
    }
    // ho trovato un divisore d1 di n; calcolo l'altro (d2=n/d1)
    mpz_fdiv_q(d2,n,d1);

    // se i divisori che ho trovato non sono primi, richiamo l'algoritmo di fattorizzazione; altrimenti aggiungo il fattore trovato alla lista
    if (millerRabin(d1,iter)==0) {
        fattori=fattorizza_Lenstra(d1,fattori);
    }
    else {
        fattori=add_factor(fattori,d1,1);
    }
    if (millerRabin(d2,iter)==0) {
        fattori=fattorizza_Lenstra(d2,fattori);
    }
    else {
        fattori=add_factor(fattori,d2,1);
    }

    mpz_clears(d1,d2,NULL);
    return fattori;
}
