#include <stdio.h>
#include <string.h>
#include <math.h>
#include "..\Include\Basi_primi_Pomerance.h"
#include "..\Include\MillerRabin.h"

int main () {
    mpz_t n,bound,d;
    mpz_inits(n,bound,d,NULL);
    printf("Inserisci il numero da fattorizzare:\n");
    gmp_scanf("%Zd",n);
    if (millerRabin(n,20)) printf("Numero primo");
    else {
        unsigned int r=strlen(mpz_get_str(NULL,2,n));
        unsigned int s=sqrt(r*log(r)/2);
        mpz_set_ui(bound,1);
        mpz_mul_2exp(bound,bound,s);
        mpz_add_ui(bound,bound,2); // calcolo bound seguendo (circa) la stima del Koblitz

        int outcome=basi_primi(d,n,bound,1);
        if (outcome==1) gmp_printf("Divisore: %Zd",d);
        else printf("Non sono riuscito a fattorizzare");
    }

    mpz_clears(n,bound,d,NULL);
    return 0;
}
