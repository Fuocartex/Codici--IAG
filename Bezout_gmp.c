#include <stdio.h>
#include "Bezout_gmp.h"

int main () {
    mpz_t a,b;
    mpz_init(a);
    mpz_init(b);
    gmp_scanf("%Zd",a);
    gmp_scanf("%Zd",b);

    MCD(a,b);
    gmp_printf("%Zd",a);

    mpz_clear(a);
    mpz_clear(b);
    return 0;
}
