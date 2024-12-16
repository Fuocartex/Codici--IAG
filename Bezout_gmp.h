#include <gmp.h>

void MCD (mpz_t, mpz_t);

void MCD (mpz_t a, mpz_t b) {
    while (mpz_cmp_si(b,0)) {
        mpz_tdiv_r(a,a,b);
        mpz_swap(a,b);
    }
    
    return;
}
