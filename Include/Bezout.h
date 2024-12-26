#include <stdlib.h>
#include <gmp.h>

void bezout (mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
void bezout_array (mpz_t, mpz_t*, mpz_t*, int);
int inv_mod (mpz_t, mpz_t, mpz_t);
void exp_mod (mpz_t, mpz_t, mpz_t, mpz_t);

// funzioni ausiliarie per bezout:
void init_id_matrix (mpz_t**, int);
void swap_rows (mpz_t**, int, int, int);
void add_multiple_row (mpz_t**, int, mpz_t, int, int);

// Calcolo d=mcd(n,m) con algoritmo euclideo (scelgo d>=0), e anche x,y interi tali che x*n+y*m=d.
void bezout(mpz_t d, mpz_t x, mpz_t y, mpz_t n, mpz_t m) {
	int i,j;
	mpz_t a,b,q,temp1,temp2;
	mpz_inits(a,b,q,temp1,temp2,NULL);
	mpz_abs(a,n); mpz_abs(b,m); // inizializzo a,b positivi per avere d>=0; se necessario cambio segno a x,y alla fine
	// la matrice M (2*2) conterra' alla fine i coefficienti di bezout nella prima riga
	mpz_t** M=malloc(2*sizeof(mpz_t*));
	for (i=0; i<2; i++) M[i]=malloc(2*sizeof(mpz_t));
	mpz_init_set_si(M[0][0],1); mpz_init(M[0][1]); mpz_init(M[1][0]); mpz_init_set_si(M[1][1],1); // inizializzo M = identità

	// algoritmo di euclide
	while (mpz_cmp_si(b,0)>0) {  // se b>0 continuo a iterare
        mpz_fdiv_qr(q,a,a,b);  // a diventa il resto di a/b, q quoziente
        mpz_swap(a,b);

		// aggiorno matrice dei coefficienti: devo moltiplicare a sinistra per [[0, 1]; [1, -q]] (caso particolare di solo due interi)
		mpz_set(temp1,M[1][0]); mpz_set(temp2,M[1][1]); // copio prima riga
		mpz_neg(q,q);
		mpz_mul(M[1][0],M[1][0],q); mpz_add(M[1][0],M[1][0],M[0][0]); // calcolato riga 2 colonna 1
		mpz_mul(M[1][1],M[1][1],q); mpz_add(M[1][1],M[1][1],M[0][1]); // calcolato riga 2 colonna 2
		mpz_set(M[0][0],temp1); mpz_set(M[0][1],temp2); // calcolata riga 1
    }
    mpz_set(d,a);  // calcolato mcd
	mpz_set(x,M[0][0]); mpz_set(y,M[0][1]);
	// aggiusto i coefficienti per eventuali cambi di segno
	if (mpz_cmp_si(n,0)<0) mpz_neg(x,x);
	if (mpz_cmp_si(m,0)<0) mpz_neg(y,y);

	for (i=0; i<2; i++) {
		for (j=0; j<2; j++) mpz_clear(M[i][j]);
	}
	mpz_clears(a,b,q,temp1,temp2,NULL);
	return;
}

// Calcolo d=mcd (d>=0) dell'array "integers" di n>0 interi, salvando in "coeff" i coefficienti tali che d=coeff[0]*integers[0]+...+coeff[n-1]*integers[n-1] 
void bezout_array (mpz_t d, mpz_t* coeff, mpz_t* integers, int n) {
	// caso banale: n=1
    if (n==1) {
        mpz_set(d,integers[0]);
		if (mpz_cmp_si(d,0)<0) {
			mpz_neg(d,d);
			mpz_set_si(coeff[0],-1);
		}
		else mpz_set_si(coeff[0],1);
        return;
    }

	// creo nuove variabili per non modificare gli interi dati (positive per avere mcd>=0), e alloco la matrice necessaria per calcolare i coefficienti
    int i,j;
    mpz_t* nums = malloc(n*sizeof(mpz_t));
    for (i=0; i<n; i++) {
        mpz_init_set(nums[i],integers[i]);
        mpz_abs(nums[i],nums[i]);
    }
	mpz_t** M=malloc(n*sizeof(mpz_t*));
	for (i=0; i<n; i++) M[i]=malloc(n*sizeof(mpz_t));
	init_id_matrix(M,n);

	// algoritmo di euclide:

	int k=n-1; i=0;
    // sposto gli zeri in fondo
    while (i<k) {
        while (mpz_cmp_si(nums[i],0)>0 && i<n) i++;
        if (i>=n) break;
        while (mpz_cmp_si(nums[k],0)==0 && k>=0) k--;
        if (k<0) break;
        if (i>=k) break;
        // ho individuato in posizione i un elemento ==0 e in posizione k un elemento >0: li scambio, e tengo traccia nella matrice M
        mpz_swap(nums[i],nums[k]);
		swap_rows(M,n,i,k);
        i++; k--;
    }
	k=n; // k indicherà l'indice dal quale gli elementi sono nulli (se ci sono zeri): nums[k]==0, nums[k-1]>0. (se non ce ne sono k=n)
    for (i=0; i<n; i++) {
        if (mpz_cmp_si(nums[i],0)==0) {
            k=i;
            break;
        }
    }
    // caso vettore tutto nullo: restituisco d=0, coeff = array nullo.
    if (k==0) {
        mpz_set_si(d,0);
        for (i=0; i<n; i++) mpz_set_si(coeff[i],0);
		for (i=0; i<n; i++) mpz_clear(nums[i]);
		for (i=0; i<n; i++) for (j=0; j<n; j++) mpz_clear(M[i][j]);
        return;
    }

	mpz_t q; mpz_init(q);
	// ciclo finché ho più di un elemento non nullo
    while (k>1) {
        // trovo il minimo elemento non nullo e lo porto in prima posizione, tenendo traccia in M
        int index_min=0;
        for (i=1; i<k; i++) {
            if (mpz_cmp(nums[index_min],nums[i])>0) index_min=i;
        }
        mpz_swap(nums[index_min],nums[0]);
		swap_rows(M,n,index_min,0);

        // divido per il minimo, calcolo i resti e sposto eventuali zeri ottenuti
        for (i=1; i<k; i++) {
            mpz_fdiv_qr(q,nums[i],nums[i],nums[0]); // nums[i] diventa nums[i] mod nums[0], dove nums[0] è il minimo
			mpz_neg(q,q);
			add_multiple_row(M,n,q,0,i); // alla riga i tolgo q volte la prima riga (con indice 0)

            // se dopo la divisione ho ottenuto resto 0, porto il numero in fondo
            if (mpz_cmp_si(nums[i],0)==0) {
                mpz_swap(nums[i],nums[k-1]);
				swap_rows(M,n,i,k-1);
                k--; i--; // aggiorno gli indici
            }
        }
    }

	// quando esco dal ciclo ho k=1 ossia solo il primo elemento è non nullo: ho trovato mcd, e nella prima riga di M ho i coefficienti cercati
    mpz_set(d,nums[0]);
	for (i=0; i<n; i++) {
		mpz_set(coeff[i],M[0][i]);
		if (mpz_cmp_si(integers[i],0)<0) mpz_neg(coeff[i],coeff[i]); // aggiusto i segni poiché all'inizio avevo reso tutto positivo
	}

	mpz_clear(q);
	for (i=0; i<n; i++) mpz_clear(nums[i]);
	for (i=0; i<n; i++) for (j=0; j<n; j++) mpz_clear(M[i][j]);
	return;
}

// Calcola in x l'inverso di a modulo n: se esiste restituisce 1, altrimenti 0. Scelgo x>=0, e suppongo n>0.
int inv_mod (mpz_t x, mpz_t a, mpz_t n) {
	mpz_t d,y;
	mpz_inits(d,y,NULL);

	bezout(d,x,y,a,n);
	mpz_fdiv_r(x,x,n); // rendo x tale che 0<x<n
	if (mpz_cmp_si(d,1)==0) { // caso mcd(a,n)=1: l'inverso esiste ed è x
		mpz_clears(d,y,NULL);
		return 1;
	}
	// caso mcd(a,n)!=1: l'inverso non esiste
	mpz_clears(d,y,NULL);
	return 0;
}

// calcola in x la base b elevata alla potenza exp modulo n, con esponenziazione binaria
void exp_mod (mpz_t x, mpz_t b, mpz_t exp, mpz_t n) {
	int bit;
	mpz_t squares,e;
	mpz_init_set(squares,b);
	mpz_init_set(e,exp);
	mpz_set_si(x,1);

	while (mpz_cmp_si(e,0)>0) {
		bit=mpz_tstbit(e,0); // controllo l'ultima cifra della rappresentazione binaria di e
		if (bit==1) { // se bit è 1 moltiplico x per il quadrato (mod n), altrimenti lascio così
			mpz_mul(x,x,squares);
			mpz_fdiv_r(x,x,n);
		}
		mpz_mul(squares,squares,squares); // aggiorno il quadrato (mod n)
		mpz_fdiv_r(squares,squares,n);
		mpz_fdiv_q_2exp(e,e,1); // shifto di 1 i bit di e
	}

	mpz_clears(squares,e,NULL);
	return;
}

// inizializza M matrice identità n*n
void init_id_matrix (mpz_t** M, int n) {
	int i,j;
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			mpz_init(M[i][j]);
		}
	}
	for (i=0; i<n; i++) mpz_set_si(M[i][i],1);

	return;
}

// scambia le righe i e k della matrice M (n*n)
void swap_rows (mpz_t** M, int n, int i, int k) {
	int j;
	for (j=0; j<n; j++) mpz_swap(M[i][j],M[k][j]);

	return;
}

// nella matrice M n*n, aggiungo alla riga k la riga i moltiplicata per mult
void add_multiple_row (mpz_t** M, int n, mpz_t mult, int i, int k) {
	int j;
	mpz_t temp;
	mpz_init(temp);
	for (j=0; j<n; j++) {
		mpz_mul(temp,M[i][j],mult);
		mpz_add(M[k][j],M[k][j],temp);
	}

	mpz_clear(temp);
	return;
}
