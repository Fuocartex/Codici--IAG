#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>
#include "mcd.h"

int basi_primi (mpz_t, mpz_t, mpz_t, unsigned long int);

// funzioni ausiliarie
mpz_t* cont_frac_sqrt_mod(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, unsigned long int);
void cont_frac_next_term(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
mpz_t* squares_mod(mpz_t*, unsigned long int, mpz_t);
void riduci_mod_min(mpz_t, mpz_t);
unsigned long int check_null_column(bool*, int**, unsigned long int, unsigned long int);

// Fattorizza l'intero positivo n utilizzando l'algoritmo delle basi di primi di Pomerance: se riesce restituisce 1 e salva in
// d un divisore proprio di n, altrimenti restituisce 0. Bound rappresenta il massimo valore che possono assumere i primi nella base,
// iter il numero di iterazioni eseguite dall'algoritmo.
int basi_primi (mpz_t d, mpz_t n, mpz_t bound, unsigned long int iter){
    int i,j;

    // Costruisco una base formata da -1, 2 e tutti i numeri dispari da 3 fino a bound (non saranno tutti primi; e' una scelta semplice ma non ottimale)
    unsigned long int base_length = mpz_get_ui(bound);
    base_length=(base_length-1)/2 +2;
    mpz_t *base = (mpz_t*)malloc(base_length*sizeof(mpz_t));
    mpz_init_set_si(base[0],-1);
    mpz_init_set_si(base[1],2);
    for (i=2; i<base_length; i++) {
        mpz_init_set_si(base[i],2*i-1);
    }

    // la matrice M ((l+1)*l, con l=base_length) conterra', nell'entrata M[i][j], l'esponente massimo exp tale che base[j]^exp divide b[i]
    int** M=(int**)malloc((base_length+1)*sizeof(int));
    for (i=0; i<base_length+1; i++) M[i]=(int*)malloc(base_length*sizeof(int));
    for (i=0; i<base_length+1; i++) for (j=0; j<base_length; j++) M[i][j]=0; // inizializzo a 0

    // dati iniziali delle relazioni ricorsive per lo sviluppo in frazione continua di radice di n
    mpz_t a,beta,gamma,bmeno1,bmeno2;
    mpz_inits(a,beta,gamma,bmeno1,bmeno2,NULL);
    mpz_sqrt(a,n);
    mpz_neg(beta,a);
    mpz_set_ui(gamma,1);
    mpz_set_ui(bmeno1,1);
    mpz_set_ui(bmeno2,0);

    unsigned long int it=0;
    while (it<iter) {
        // costruisco i b_i con quadrato piccolo mod n: ne voglio base_length+1 cosi' da assicurarmi almeno una combinazione lineare di righe
        // nulla nella matrice che costruiro' dopo
        mpz_t* b=cont_frac_sqrt_mod(n,bmeno1,bmeno2,a,beta,gamma,base_length+1);
        mpz_t* squares=squares_mod(b,base_length+1,n);

        // calcolo la matrice degli esponenti massimi dei primi che dividono ciascun quadrato
        for (i=0; i<base_length+1; i++) {
            // primo termine (corrisponde a -1 nella base)
            if (mpz_cmp_si(squares[i],0)<0) {
                M[i][0]=1;
                mpz_neg(squares[i],squares[i]);
            }

            // secondo termine (corrisponde a 2 nella base)
            while (mpz_divisible_ui_p(squares[i],2)) {
                mpz_divexact_ui(squares[i],squares[i],2);
                M[i][1]++;
            }

            // termini rimanenti (corrispondono ai dispari (2*j -1) nella base)
            for (j=2; j<base_length; j++) {
                while (mpz_divisible_ui_p(squares[i],2*j-1)) {
                    mpz_divexact_ui(squares[i],squares[i],2*j-1);
                    M[i][j]++;
                }
            }

            // controllo se il quadrato e' stato completamente scomposto nella base; in caso contrario lo sostituisco con un nuovo b
            // (e il suo quadrato) e ritento la scomposizione
            if (mpz_cmp_si(squares[i],1)!=0) {
                cont_frac_next_term(b[i],n,bmeno1,bmeno2,a,beta,gamma);
                mpz_mul(squares[i],b[i],b[i]);
                riduci_mod_min(squares[i],n);
                i--; // attenzione: in questo modo il ciclo for su i potrebbe non terminare: aggiungiamo una qualche condizione?
            }
        }

        // conto le colonne nulle poiche' possono essere ignorate nell'eliminazione di Gauss, facendo risparmiare spazio e tempo
        unsigned long int num_null_cols;
        bool* is_col_zeros=(bool*)malloc(base_length*sizeof(bool));
        num_null_cols = check_null_column(is_col_zeros,M,base_length+1,base_length);
        unsigned long int num_cols_M_mod2 = base_length-num_null_cols;
        // creo un vettore di "differenze cumulative", lungo come il numero di colonne di M_mod2, che tiene conto di quante colonne sono state
        // rimosse fino a quel punto: l'idea e' che, se sto guardando la colonna j di M_mod2, questa corrisponde alla colonna (j+cumul_diff[j]) di M,
        // e cioe' all'elemento della base di indice (j+cumul_diff[j]).
        int* cumul_diff=(int*)malloc(num_cols_M_mod2*sizeof(int));
        int contatore=0;
        i=0; j=0;
        while (j<num_cols_M_mod2) {
            while (is_col_zeros[i]) {
                contatore++;
                i++;
                if (i>base_length) break;
            }
            cumul_diff[j]=contatore;
            i++; j++;
        }
        // creo la matrice booleana M_mod2 che contiene i coefficienti di M ridotti modulo 2, escluse le colonne nulle (true=1, false=0).
        // sara' una matrice di dimensioni (base_length+1)*(base_length-num_null_cols).
        bool** M_mod2=(bool**)malloc((base_length+1)*sizeof(bool));
        for (i=0; i<base_length+1; i++) M_mod2[i]=(bool*)malloc(num_cols_M_mod2*sizeof(bool));
        for (i=0; i<base_length+1; i++) {
            for (j=0; j<num_cols_M_mod2; j++) {
                M_mod2[i][j]=M[i][j+cumul_diff[j]] % 2;
            }
        }
        
        



        ////// da proseguire qui ///////    (affiancare identita' (base_length+1), fare Gauss, cercare congruenze ...)





        free(is_col_zeros); free(cumul_diff);
        for (i=0; i<base_length+1; i++) free(M_mod2[i]); free(M_mod2);
        for (i=0; i<base_length+1; i++) mpz_clear(b[i]); free(b);
        for (i=0; i<base_length+1; i++) mpz_clear(squares[i]); free(squares);
        it++;
    }

    // se sono arrivato qui ho completato tutte le iterazioni senza trovare un divisore: fallimento.

    for (i=0; i<base_length+1; i++) free(M[i]); free(M);
    for (i=0; i<base_length; i++) mpz_clear(base[i]); free(base);
    mpz_clears(a,beta,gamma,bmeno1,bmeno2,NULL);
    return 0;
}

// Restituisce k>0 numeratori dell'approssimazione in frazione continua di sqrt(n) modulo n, espressi con valore assoluto minimo, a partire
// dai due numeratori precedenti e dai corrispondenti valori di a, beta, gamma (che vengono modificati durante l'esecuzione).
mpz_t* cont_frac_sqrt_mod(mpz_t n, mpz_t bmeno1, mpz_t bmeno2, mpz_t a, mpz_t beta, mpz_t gamma, unsigned long int k) {
    int i;
    mpz_t sqrt_n,temp;
    mpz_inits(sqrt_n,temp,NULL);
    mpz_sqrt(sqrt_n,n);
    mpz_t* b=(mpz_t*)malloc(k*sizeof(mpz_t));
    for (i=0;i<k;i++) mpz_init(b[i]);

    // esplicito i primi due termini (poiché utilizzano dati iniziali specifici su b_-1 e b_-2)

    mpz_mul(temp,beta,beta);
    mpz_sub(temp,n,temp);
    mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
    mpz_sub(temp,sqrt_n,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    mpz_mul(temp,a,bmeno1);
    mpz_add(b[0],temp,bmeno2);
    riduci_mod_min(b[0],n); // aggiornato b[0]

    if (k==1) {
        mpz_set(bmeno2,bmeno1);
        mpz_set(bmeno1,b[0]);
        return b;
    }

    mpz_mul(temp,beta,beta);
    mpz_sub(temp,n,temp);
    mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
    mpz_sub(temp,sqrt_n,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    mpz_mul(temp,a,b[0]);
    mpz_add(b[1],temp,bmeno1);
    riduci_mod_min(b[1],n); // aggiornato b[1]
    
    if (k==2) {
        mpz_set(bmeno2,b[0]);
        mpz_set(bmeno1,b[1]);
        return b;
    }

    for (i=2; i<k; i++) {
        mpz_mul(temp,beta,beta);
        mpz_sub(temp,n,temp);
        mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
        mpz_sub(temp,sqrt_n,beta);
        mpz_fdiv_q(a,temp,gamma); // aggiornato a
        mpz_mul(temp,a,gamma);
        mpz_add(temp,temp,beta);
        mpz_neg(beta,temp); // aggiornato beta
        mpz_mul(temp,a,b[i-1]);
        mpz_add(b[i],temp,b[i-2]);
        riduci_mod_min(b[i],n); // aggiornato b[i]
    }
    mpz_set(bmeno2,b[k-2]);
    mpz_set(bmeno1,b[k-1]);
    
    mpz_clears(sqrt_n,temp,NULL);
    return b;
}

// salva in b il termine successivo dello sviluppo in frazione continua di sqrt(n) modulo n, espresso con valore assoluto minimo, a partire
// dai due numeratori precedenti e dai corrispondenti valori di a, beta, gamma (che vengono modificati durante l'esecuzione).
void cont_frac_next_term(mpz_t b, mpz_t n, mpz_t bmeno1, mpz_t bmeno2, mpz_t a, mpz_t beta, mpz_t gamma) {
    mpz_t sqrt_n,temp;
    mpz_inits(sqrt_n,temp,NULL);
    mpz_sqrt(sqrt_n,n);

    mpz_mul(temp,beta,beta);
    mpz_sub(temp,n,temp);
    mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
    mpz_sub(temp,sqrt_n,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    mpz_mul(temp,a,bmeno1);
    mpz_add(b,temp,bmeno2);
    riduci_mod_min(b,n); // aggiornato b
    mpz_set(bmeno2,bmeno1);
    mpz_set(bmeno1,b); // aggiornati i termini precedenti

    mpz_clears(sqrt_n,temp,NULL);
    return;
}

// restituisce un array con i quadrati degli elementi nell'array in input (lungo k) modulo n, espressi con valore assoluto minimo.
mpz_t* squares_mod(mpz_t* array, unsigned long int k, mpz_t n) {
    int i;
    mpz_t* squares=(mpz_t*)malloc(k*sizeof(mpz_t));
    for (i=0;i<k;i++){
        mpz_init(squares[i]);
        mpz_mul(squares[i],array[i],array[i]);
        riduci_mod_min(squares[i],n);
    }

    return squares;
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

// per una matrice m*n, salva in is_zero[j] true se la colonna j di M e' tutta nulla, false altrimenti; restituisce il numero di colonne nulle.
unsigned long int check_null_column(bool* is_zero, int** M, unsigned long int m, unsigned long int n) {
    int i,j;
    unsigned long int num_null_columns=0;
    for (j=0; j<n; j++) {
        is_zero[j]=true;
        num_null_columns++;
        for (i=0; i<m; i++) {
            if (M[i][j]!=0) {
                is_zero[j]=false;
                num_null_columns--;
                break;
            }
        }
    }
    return num_null_columns;
}
