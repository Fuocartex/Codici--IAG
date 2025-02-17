#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>
#include "mcd.h"

// DISCLAIMER: questa versione a volte non funziona correttamente: per certi input sembra restituire come divisore 1 anziche' un divisore proprio;
// sarebbero necessarie ulteriori indagini.

int basi_primi (mpz_t, mpz_t, mpz_t, unsigned long int);

// funzioni ausiliarie
mpz_t* cont_frac_sqrt_mod(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, unsigned long int);
void cont_frac_next_term(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
mpz_t* squares_mod(mpz_t*, unsigned long int, mpz_t);
void riduci_mod_min(mpz_t, mpz_t);
unsigned long int check_null_column(bool*, int**, unsigned long int, unsigned long int);
void Gauss (bool**, bool**, unsigned long int, unsigned long int);
unsigned long int find_null_rows (bool**, unsigned long int, unsigned long int, unsigned long int);
void get_B (mpz_t, mpz_t, bool**, mpz_t*, int*, unsigned long int, unsigned long int);
void get_A (mpz_t, mpz_t, mpz_t*, int**, unsigned long int, bool**, int*, unsigned long int);

// Fattorizza l'intero positivo n utilizzando l'algoritmo delle basi di primi di Pomerance: se riesce restituisce 1 e salva in
// d un divisore proprio di n, altrimenti restituisce 0. Bound rappresenta il massimo valore che possono assumere i primi nella base,
// iter il numero di iterazioni eseguite dall'algoritmo.
int basi_primi (mpz_t d, mpz_t n, mpz_t bound, unsigned long int iter){
	int i,j;

    // controllo che n sia dispari
    mpz_t r;
    mpz_init(r);
    mpz_fdiv_r_ui(r,n,2);
    if (mpz_cmp_si(r,0)==0) {
        mpz_set_ui(d,2);
        mpz_clear(r);
        return 1;
    }
    mpz_clear(r);

    // controllo che n non sia un quadrato (serve per produrre i numeratori dello sviluppo in frazione continua di sqrt(n))
    mpz_t sqrt_n,temp;
    mpz_inits(sqrt_n,temp,NULL);
    mpz_sqrt(sqrt_n,n); // parte intera della radice di n
    mpz_mul(temp,sqrt_n,sqrt_n);
    if (mpz_cmp(temp,n)==0) {
        mpz_set(d,sqrt_n);
        mpz_clears(sqrt_n,temp,NULL);
        return 1;
    }
    mpz_clears(sqrt_n,temp,NULL);

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
    int** M=(int**)malloc((base_length+1)*sizeof(int*));
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
        bool** M_mod2=(bool**)malloc((base_length+1)*sizeof(bool*));
        for (i=0; i<base_length+1; i++) M_mod2[i]=(bool*)malloc(num_cols_M_mod2*sizeof(bool));
        for (i=0; i<base_length+1; i++) {
            for (j=0; j<num_cols_M_mod2; j++) {
                M_mod2[i][j]=M[i][j+cumul_diff[j]] % 2;
            }
        }
        
        // costruisco la matrice identità da affiancare a M_mod2 prima di applicare Gauss
        bool** Id=(bool**)malloc((base_length+1)*sizeof(bool*));
        for (i=0; i<base_length+1; i++) Id[i]=(bool*)malloc((base_length+1)*sizeof(bool));
        for (i=0; i<base_length+1; i++) for (j=0; j<base_length+1; j++) Id[i][j]=false; // inizializzo a 0
        for (i=0; i<base_length+1; i++) Id[i][i]=true; // pongo la diagonale uguale ad 1
        
        // applico Gauss e calcolo A e B
        Gauss(M_mod2, Id, base_length+1, num_cols_M_mod2);
        unsigned long int index=0; 
        unsigned long int null_row=0;
        mpz_t A, B, menoB, ApiuB;
	    mpz_inits(A, B, menoB, ApiuB, NULL);
        while (index<(base_length+1)) {
		    null_row=find_null_rows(M_mod2, base_length+1, num_cols_M_mod2, index);
		    get_B(B, n, Id, b, cumul_diff, null_row, (base_length+1));
			get_A(A, n, base, M, base_length, Id, cumul_diff, null_row);
            mpz_sub(menoB,n,B);
            mpz_fdiv_r(menoB,menoB,n);
		    
		    if (mpz_cmp(A, B)==0 || mpz_cmp(A, menoB)==0) { // se A=B oppure A=-B mod n, l'algoritmo è fallito
		    	if (index!=base_length) { // se non abbiamo esaurito le righe riproviamo con un'altra combinazione
		    		index++;
		    	} else { // altrimenti ricominciamo da capo
		    		break; // in questo modo rientriamo nel ciclo while(it<iter)
		    	}
		    } else { // se invece se A!=B oppure A!=-B mod n concludo l'algoritmo calcolando d=MCD(A+B, n)
				mpz_add(ApiuB, A, B);
				mcd_euclide(d, ApiuB, n);

/*                if (mpz_cmp_si(d,1)==0) {//// sarebbe da rimuovere; e' solo per evitare che l'algoritmo (sbagliato) restituisca 1
                    index++;
                    continue;
                }
*/

                // pulizia memoria
				mpz_clears(A, B, menoB, ApiuB, NULL);
                free(is_col_zeros); free(cumul_diff);
                for (i=0; i<base_length+1; i++) free(M_mod2[i]); free(M_mod2);
                for (i=0; i<base_length+1; i++) free(Id[i]); free(Id);
                for (i=0; i<base_length+1; i++) mpz_clear(b[i]); free(b);
                for (i=0; i<base_length+1; i++) mpz_clear(squares[i]); free(squares);
                for (i=0; i<base_length+1; i++) free(M[i]); free(M);
                for (i=0; i<base_length; i++) mpz_clear(base[i]); free(base);
                mpz_clears(a,beta,gamma,bmeno1,bmeno2,NULL);
				return 1;
	    	}
		}
        // pulizia memoria
		mpz_clears(A, B, menoB, ApiuB, NULL);
        free(is_col_zeros); free(cumul_diff);
        for (i=0; i<base_length+1; i++) free(M_mod2[i]); free(M_mod2);
        for (i=0; i<base_length+1; i++) free(Id[i]); free(Id);
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

    mpz_mul(temp,a,bmeno1);
    mpz_add(b[0],temp,bmeno2); // aggiornato b[0]
    mpz_mul(temp,beta,beta);
    mpz_sub(temp,n,temp);
    mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
    mpz_sub(temp,sqrt_n,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    riduci_mod_min(b[0],n);

    if (k==1) {
        mpz_set(bmeno2,bmeno1);
        mpz_set(bmeno1,b[0]);
        return b;
    }

    mpz_mul(temp,a,b[0]);
    mpz_add(b[1],temp,bmeno1); // aggiornato b[1]
    mpz_mul(temp,beta,beta);
    mpz_sub(temp,n,temp);
    mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
    mpz_sub(temp,sqrt_n,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    riduci_mod_min(b[1],n);

    if (k==2) {
        mpz_set(bmeno2,b[0]);
        mpz_set(bmeno1,b[1]);
        return b;
    }

    for (i=2; i<k; i++) {
        mpz_mul(temp,a,b[i-1]);
        mpz_add(b[i],temp,b[i-2]); // aggiornato b[i]
        mpz_mul(temp,beta,beta);
        mpz_sub(temp,n,temp);
        mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
        mpz_sub(temp,sqrt_n,beta);
        mpz_fdiv_q(a,temp,gamma); // aggiornato a
        mpz_mul(temp,a,gamma);
        mpz_add(temp,temp,beta);
        mpz_neg(beta,temp); // aggiornato beta
        riduci_mod_min(b[i],n);
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

    mpz_mul(temp,a,bmeno1);
    mpz_add(b,temp,bmeno2); // aggiornato b
    mpz_mul(temp,beta,beta);
    mpz_sub(temp,n,temp);
    mpz_fdiv_q(gamma,temp,gamma); // aggiornato gamma
    mpz_sub(temp,sqrt_n,beta);
    mpz_fdiv_q(a,temp,gamma); // aggiornato a
    mpz_mul(temp,a,gamma);
    mpz_add(temp,temp,beta);
    mpz_neg(beta,temp); // aggiornato beta
    riduci_mod_min(b,n);

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

//eliminazione di Gauss applicata alla matrice M_mod2 i cui passaggi vengono ripetuti anche su una matrice identità di dimensione pari al numero di righe di M_mod2
void Gauss(bool **M, bool **Id, unsigned long int rows, unsigned long int cols) {
    int row=0;
    for (int col=0; col<cols && row<rows; col++) {
        // Trova il pivot nella colonna corrente
        int pivot=-1;
        for (int i=row; i<rows; i++) {
            if (M[i][col]) {
                pivot=i;
                break;
            }
        }
        
        // Se non troviamo un pivot, passiamo alla colonna successiva
        if (pivot==-1) {
        	continue;
        }

        // Scambia la riga pivot con la riga attuale
        if (pivot!=row) {
            bool *temp=M[pivot];
            M[pivot]=M[row];
            M[row]=temp;

            temp=Id[pivot];
            Id[pivot]=Id[row];
            Id[row]=temp;
        }

        // Elimina i valori nelle altre righe (comando XOR)
        for (int i=0; i<rows; i++) {
            if (i!=row && M[i][col]) {
                for (int j=0; j<cols; j++) {
                    M[i][j] ^= M[row][j];
//                    Id[i][j] ^= Id[row][j];
                }
                for (int j=0; j<rows; j++) { // forse devo fare cosi'? Il fatto e' che Id ha piu' colonne di M
                    Id[i][j] ^= Id[row][j];
                }
            }
        }
        
        // Passa alla prossima riga
        row++;
    }
    return;
}

// troviamo la prima riga nulla di M_mod2 dopo aver applicato Gauss, l'indice indica da quela riga cominciare a fare la ricerca 
unsigned long int find_null_rows (bool** mat, unsigned long int rows, unsigned long int cols, unsigned long int index) {
	unsigned long int null_row=rows; // inizializzo ad un intero che non può assumere
	for (int i=index; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			if (mat[i][j]==true) { // appena incontro un 1 interrompo il ciclo sulle colonne
				break;
			} else if (j==cols-1 && mat[i][j]==false) { // se ho controllato tutte le colonne modifico null_row
				null_row=i; 
			}
		}
		if (null_row!=rows) { // se ho modificato null_row ho trovato una riga nulla, posso interrompere la ricerca
			break;
		}
	}
	return null_row;
}

// B equivale al prodotto dei vari b[i] con i indice trovato con gauss, ossia i=j+cumul_diff[j] se Id[null_row][j]==true
void get_B (mpz_t B, mpz_t n, bool** Id, mpz_t* b, int* cumul_diff, unsigned long int null_row, unsigned long int cols) {
	mpz_set_si(B, 1);
	for (int j=0; j<cols; j++){
		if (Id[null_row][j]==true) {
//			mpz_mul(B, B, b[j+cumul_diff[j]]);
            mpz_mul(B, B, b[j]); // i vari 1 nella matrice identita' si trovano sulla colonna j se devo considerare la riga j della matrice M,
                                 // ossia il j-esimo elemento dei b; giusto? Anche perche' get_B viene chiamata con cols=base_length+1, e b e'
                                 // lungo base_length+1, cosi' come la dimensione di Id

			mpz_fdiv_r(B,B,n);
		}
	}
	return;
}

void get_A (mpz_t A, mpz_t n, mpz_t* base, int** M, unsigned long int base_length, bool** Id, int* cumul_diff, unsigned long int null_row) {
	mpz_set_si(A, 1);
	// calcolo exp_tot, ossia sommo le righe i di M (i sono quelli ricavati da gauss)
	int* exp_tot=(int*)malloc(base_length*sizeof(int));
	for (int i=0; i<base_length; i++) exp_tot[i]=0; // inizializzo a zero
	for (int i=0; i<(base_length+1); i++) { // scorriamo la riga null_row di Id, se nella colonna i l'elemento è 1 (ossia true)
		if (Id[null_row][i]==true) { // allora consideriamo la riga i+cumul_diff[i] di M
			for (int j=0; j<base_length; j++) {
//				exp_tot[j]=exp_tot[j]+M[i+cumul_diff[i]][j];
                exp_tot[j]=exp_tot[j]+M[i][j]; // ragionamento analogo a quello per get_B: la matrice M ha dimensione (base_length+1 ) * base_length
			}
		}
	}
	
	// ora calcoliamo A=base[i]^(exp_tot[i]/2)
	mpz_t temp; 
	mpz_inits(temp, NULL);
	for (int i=0; i<base_length; i++) { 
		mpz_powm_ui(temp, base[i], exp_tot[i]/2, n); 
		mpz_mul(A, A, temp);
		mpz_fdiv_r(A,A,n);
	}
	
	mpz_clears(temp, NULL);
	free(exp_tot);
	
	return; 
}
