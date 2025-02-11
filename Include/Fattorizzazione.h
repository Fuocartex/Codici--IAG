#include <stdlib.h>
#include <gmp.h>

// definizione della struttura dei fattori: l'obiettivo e' costruire una lista di fattori in modo da poter scrivere un intero
// come prodotto di primi (in ordine), ciascuno con la rispettiva potenza.
typedef struct Factor {
    mpz_t prime;
    unsigned int exponent;
    struct Factor* next;
} Factor;

Factor* add_factor(Factor*, mpz_t, unsigned int);
Factor* cons(Factor*, mpz_t, unsigned int);
void print_factors(Factor*);

// aggiunge in testa alla lista il primo p elevato alla potenza exp
Factor* cons(Factor* factor, mpz_t p, unsigned int exp) {
    Factor* new_factor = (Factor*)malloc(sizeof(Factor));
    mpz_init_set(new_factor->prime,p);
    new_factor->exponent=exp;
    new_factor->next=factor;
    return new_factor;
}

// aggiunge alla lista di fattori "factor", in modo ordinato, il primo p elevato alla potenza exp
Factor* add_factor(Factor* factor, mpz_t p, unsigned int exp) {
    // caso in cui sono arrivato a fine lista, oppure ho trovato il primo numero primo presente nella lista maggiore di p: aggiungo un fattore
    if (factor==NULL || mpz_cmp(factor->prime,p)>0) {
        return cons(factor,p,exp);
    }

    // caso in cui ho trovato nella lista un primo identico a p: sommo l'esponente
    if (mpz_cmp(factor->prime,p)==0) {
        factor->exponent=factor->exponent+exp;
        return factor;
    }

    // proseguo la ricerca in modo ricorsivo
    factor->next=add_factor(factor->next,p,exp);
    // riaggancio i puntatori al ritorno della ricorsione
    return factor;
}

// stampa la lista dei fattori
void print_factors(Factor* factor) {
    if (factor==NULL) return;

    gmp_printf("%Zd^(%u)",factor->prime,factor->exponent);
    factor=factor->next;
    while (factor!=NULL) {
        gmp_printf(" * %Zd^(%u)",factor->prime,factor->exponent);
        factor=factor->next;
    }

    return;
}
