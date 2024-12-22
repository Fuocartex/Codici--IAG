#include <stdio.h>
#include "mcd.h"

int main () {
    mpz_t a,b,x;
    mpz_init(a);
    mpz_init(b);
    mpz_init(x);
    gmp_scanf("%Zd",a);
    gmp_scanf("%Zd",b);

    mcd_binario(x,a,b);
    gmp_printf("%Zd",x);

    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(x);
    return 0;
}
