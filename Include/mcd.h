#include <stdlib.h>
#include <gmp.h>

void mcd_euclide (mpz_t, mpz_t, mpz_t);
void mcd_binario (mpz_t, mpz_t, mpz_t);
void mcd_euclide_array (mpz_t, mpz_t*, int);
void mcd_binario_array (mpz_t, mpz_t*, int);
void mcd_euclide_concat (mpz_t, mpz_t*, int);
void mcd_binario_concat (mpz_t, mpz_t*, int);

// massimo comun divisore tra n e m, salvato in a, con algoritmo di Euclide. Il risultato è sempre non negativo.
void mcd_euclide (mpz_t a, mpz_t n, mpz_t m) {
    // creo nuove variabili per non modificare n e m, e le rendo positive per avere mcd>=0
    mpz_t x,y;
    mpz_inits(x,y,NULL);
    mpz_abs(x,n); mpz_abs(y,m);
    
    while (mpz_cmp_si(y,0)>0) {  // se y>0 continuo a iterare
        mpz_fdiv_r(x,x,y);  // x diventa il resto di x/y
        mpz_swap(x,y);
    }
    mpz_set(a,x);

    mpz_clears(x,y,NULL);
    return;
}

// massimo comun divisore tra n e m, salvato in a, con algoritmo binario. Il risultato è sempre non negativo.
void mcd_binario (mpz_t a, mpz_t n, mpz_t m) {
    // casi base: uno dei due numeri è 0
    if (mpz_cmp_si(n,0)==0) {
        mpz_set(a,m);
        mpz_abs(a,a);
        return;
    }
    else if (mpz_cmp_si(m,0)==0) {
        mpz_set(a,n);
        mpz_abs(a,a);
        return;
    }

    // creo nuove variabili per non modificare n e m, e le rendo positive per avere mcd>=0
    mpz_t x,y;
    mpz_inits(x,y,NULL);
    mpz_abs(x,n); mpz_abs(y,m);
    
 /* ottimizzazione che ho trovato io facendo qualche test: se i due numeri hanno ordini di grandezza molto diversi l'algoritmo diventa
    inefficiente, quindi eseguo una sola passata dell'algoritmo euclideo facendo una prima divisione con resto. In questo modo i due
    numeri avranno ordine di grandezza simile al più piccolo, e l'algoritmo binario si velocizza molto. */
    if (mpz_cmp(x,y)>0) {
        mpz_fdiv_r(x,x,y);
        if (mpz_cmp_si(x,0)==0) {
            mpz_set(a,y);
            mpz_clears(x,y,NULL);
            return;
        }
    }
    else {
        mpz_fdiv_r(y,y,x);
        if (mpz_cmp_si(y,0)==0) {
            mpz_set(a,x);
            mpz_clears(x,y,NULL);
            return;
        }
    }

    // calcolo (in k) la più alta potenza di 2 che divide i due numeri
    unsigned long int i,j,k;
    i = mpz_scan1(x,0);
    j = mpz_scan1(y,0);
    if (i<j) k=i;
    else k=j;
    
    // rendo dispari i due numeri, avendo già salvato la più alta potenza di 2 che divide entrambi
    mpz_fdiv_q_2exp(x,x,i);
    mpz_fdiv_q_2exp(y,y,j);

    // ciclo lavorando sempre su numeri dispari, ed esco quando uno dei due è 0
    while (mpz_cmp_si(x,0)>0) {
        if (mpz_cmp(x,y)<0) mpz_swap(x,y); // in questo modo x>=y
        mpz_sub(x,x,y);  // eseguo la sottrazione x=x-y; ora x è pari
        if (mpz_cmp_si(x,0)==0) break;  // se provo a scannerizzare il primo 1 nella rappresentazione binaria di 0 ottengo -1, quindi devo uscire prima
        i = mpz_scan1(x,0);
        mpz_fdiv_q_2exp(x,x,i);  // tolgo tutte le potenze di 2 che dividono x: così torna dispari
    }

    // quando esco dal ciclo in y ci sarà mcd dei numeri resi dispari; dopo rimoltiplico per la giusta potenza di 2 calcolata prima
    mpz_set(a,y);
    mpz_mul_2exp(a,a,k);

    mpz_clears(x,y,NULL);
    return;
}

// massimo comun divisore degli n>0 interi dell'array "integers", salvato in a, con algoritmo di Euclide esteso. Il risultato è sempre non negativo.
void mcd_euclide_array (mpz_t a, mpz_t* integers, int n) {
    // caso banale: n=1
    if (n==1) {
        mpz_set(a,integers[0]);
        mpz_abs(a,a);
        return;
    }
    // creo nuove variabili per non modificare gli interi dati, e le rendo positive per avere mcd>=0
    int i;
    mpz_t* nums = malloc(n*sizeof(mpz_t));
    for (i=0; i<n; i++) {
        mpz_init_set(nums[i],integers[i]);
        mpz_abs(nums[i],nums[i]);
    }
    
    int k=n-1; i=0;
    // sposto gli zeri in fondo
    while (i<k) {
        while (mpz_cmp_si(nums[i],0)>0 && i<n) i++;
        if (i>=n) break;
        while (mpz_cmp_si(nums[k],0)==0 && k>=0) k--;
        if (k<0) break;
        if (i>=k) break;
        // ho individuato in posizione i un elemento ==0 e in posizione k un elemento >0: li scambio
        mpz_swap(nums[i],nums[k]);
        i++; k--;
    }
    k=n; // k indicherà l'indice dal quale gli elementi sono nulli (se ci sono zeri): nums[k]==0, nums[k-1]>0. (se non ce ne sono k=n)
    for (i=0; i<n; i++) {
        if (mpz_cmp_si(nums[i],0)==0) {
            k=i;
            break;
        }
    }
    // caso vettore tutto nullo: restituisco mcd=0
    if (k==0) {
        mpz_set_si(a,0);
        for (i=0; i<n; i++) mpz_clear(nums[i]);
        return;
    }
    
    // ciclo finché ho più di un elemento non nullo
    while (k>1) {
        // trovo il minimo elemento non nullo e lo porto in prima posizione
        int index_min=0;
        for (i=1; i<k; i++) {
            if (mpz_cmp(nums[index_min],nums[i])>0) index_min=i;
        }
        mpz_swap(nums[index_min],nums[0]);

        // divido per il minimo, calcolo i resti e sposto eventuali zeri ottenuti
        for (i=1; i<k; i++) {
            mpz_fdiv_r(nums[i],nums[i],nums[0]); // nums[i] diventa nums[i] mod nums[0], dove nums[0] è il minimo

            // se dopo la divisione ho ottenuto resto 0, porto il numero in fondo
            if (mpz_cmp_si(nums[i],0)==0) {
                mpz_swap(nums[i],nums[k-1]);
                k--; i--; // aggiorno gli indici
            }
        }
    }

    // quando esco dal ciclo ho k=1 ossia solo il primo elemento è non nullo: ho trovato mcd.
    mpz_set(a,nums[0]);
    for (i=0; i<n; i++) mpz_clear(nums[i]);
    return;
}

// massimo comun divisore degli n>0 interi dell'array "integers", salvato in a, con algoritmo binario esteso. Il risultato è sempre non negativo.
void mcd_binario_array (mpz_t a, mpz_t* integers, int n) {
    // caso banale: n=1
    if (n==1) {
        mpz_set(a,integers[0]);
        mpz_abs(a,a);
        return;
    }
    // creo nuove variabili per non modificare gli interi dati, e le rendo positive per avere mcd>=0
    int i;
    mpz_t* nums = malloc(n*sizeof(mpz_t));
    for (i=0; i<n; i++) {
        mpz_init_set(nums[i],integers[i]);
        mpz_abs(nums[i],nums[i]);
    }
    
    int k=n-1; i=0;
    // sposto gli zeri in fondo
    while (i<k) {
        while (mpz_cmp_si(nums[i],0)>0 && i<n) i++;
        if (i>=n) break;
        while (mpz_cmp_si(nums[k],0)==0 && k>=0) k--;
        if (k<0) break;
        if (i>=k) break;
        // ho individuato in posizione i un elemento ==0 e in posizione k un elemento >0: li scambio
        mpz_swap(nums[i],nums[k]);
        i++; k--;
    }
    k=n; // k indicherà l'indice dal quale gli elementi sono nulli (se ci sono zeri): nums[k]==0, nums[k-1]>0. (se non ce ne sono k=n)
    for (i=0; i<n; i++) {
        if (mpz_cmp_si(nums[i],0)==0) {
            k=i;
            break;
        }
    }
    // caso vettore tutto nullo: restituisco mcd=0
    if (k==0) {
        mpz_set_si(a,0);
        for (i=0; i<n; i++) mpz_clear(nums[i]);
        return;
    }
    
 /* ottimizzazione che ho trovato io facendo qualche test: se i numeri hanno ordini di grandezza molto diversi l'algoritmo diventa
    inefficiente, quindi eseguo una sola passata dell'algoritmo euclideo facendo una prima divisione con resto. In questo modo i
    numeri avranno ordine di grandezza simile al più piccolo, e l'algoritmo binario si velocizza molto. */
    int index_min=0;
    for (i=1; i<k; i++) {
        if (mpz_cmp(nums[index_min],nums[i])>0) index_min=i;
    }
    mpz_swap(nums[index_min],nums[0]);
    // divido per il minimo, calcolo i resti e sposto eventuali zeri ottenuti
    for (i=1; i<k; i++) {
        mpz_fdiv_r(nums[i],nums[i],nums[0]); // nums[i] diventa nums[i] mod nums[0], dove nums[0] è il minimo
        // se dopo la divisione ho ottenuto resto 0, porto il numero in fondo
        if (mpz_cmp_si(nums[i],0)==0) {
            mpz_swap(nums[i],nums[k-1]);
            k--; i--; // aggiorno gli indici
        }
    }

    // inizio (finalmente) con la parte di algoritmo binario: salvo la più alta potenza di 2 che divide tutti i numeri
    unsigned long int j,exp;
    exp=mpz_scan1(nums[0],0);
    mpz_fdiv_q_2exp(nums[0],nums[0],exp);
    for (i=1; i<k; i++) {
        j=mpz_scan1(nums[i],0);
        mpz_fdiv_q_2exp(nums[i],nums[i],j);
        if (j<exp) exp=j;
    }

    // ciclo finché ho più di un elemento non nullo; saranno sempre dispari
    while (k>1) {
        // trovo il minimo elemento non nullo e lo porto in prima posizione
        int index_min=0;
        for (i=1; i<k; i++) {
            if (mpz_cmp(nums[index_min],nums[i])>0) index_min=i;
        }
        mpz_swap(nums[index_min],nums[0]);

        // sottraggo il minimo, poi se ho ottenuto 0 lo sposto in fondo, altrimenti tolgo le potenze di 2
        for (i=1; i<k; i++) {
            mpz_sub(nums[i],nums[i],nums[0]);

            if (mpz_cmp_si(nums[i],0)==0) {
                mpz_swap(nums[i],nums[k-1]);
                k--; i--;
            }
            else {
                j=mpz_scan1(nums[i],0);
                mpz_fdiv_q_2exp(nums[i],nums[i],j);
            }
        }
    }

    // quando esco dal ciclo ho k=1 ossia solo il primo elemento è non nullo: ho trovato mcd, a patto di rimoltiplicare per la giusta potenza di 2
    mpz_set(a,nums[0]);
    mpz_mul_2exp(a,a,exp);
    for (i=0; i<n; i++) mpz_clear(nums[i]);
    return;
}

// massimo comun divisore degli n>0 interi dell'array "integers", salvato in a, ottenuto concatenando l'algoritmo euclideo a coppie
void mcd_euclide_concat (mpz_t a, mpz_t* integers, int n) {
    // caso banale: n=1
    if (n==1) {
        mpz_set(a,integers[0]);
        mpz_abs(a,a);
        return;
    }

    mcd_euclide(a,integers[0],integers[1]);
    for (int i=2; i<n; i++) {
        mcd_euclide(a,a,integers[i]);
    }

    return;
}

// massimo comun divisore degli n>0 interi dell'array "integers", salvato in a, ottenuto concatenando l'algoritmo binario a coppie
void mcd_binario_concat (mpz_t a, mpz_t* integers, int n) {
    // caso banale: n=1
    if (n==1) {
        mpz_set(a,integers[0]);
        mpz_abs(a,a);
        return;
    }

    mcd_binario(a,integers[0],integers[1]);
    for (int i=2; i<n; i++) {
        mcd_binario(a,a,integers[i]);
    }

    return;
}
