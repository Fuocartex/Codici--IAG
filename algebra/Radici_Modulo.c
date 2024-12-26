#include <stdio.h>
#include "..\Include\Radici_Modulo.h"

int main () {
    int i;
    mpz_t x,a,p;
    mpz_inits(x,a,p,NULL);

    printf("Primo p: ");
    gmp_scanf("%Zd",p);
    printf("Numero di cui si vuole calcolare la radice quadrata mod p: ");
    gmp_scanf("%Zd",a);

    i=radice_mod(x,a,p);

    if (i==0) printf("\nNon esiste la radice");
    else gmp_printf("\nRadice: %Zd\n",x);

    mpz_clears(x,a,p,NULL);
    return 0;
}
