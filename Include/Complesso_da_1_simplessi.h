#include "Compl_Simpl.h"

SimplicialComplex* complex_from_adjacency_matrix_complete (int**, int);
SimplicialComplex* complex_from_adjacency_matrix_truncated (int**, int, int);
SimplicialComplex* complex_from_1_simplices_complete (SimplicialComplex*, int);
SimplicialComplex* complex_from_1_simplices_truncated (SimplicialComplex*, int, int);
void add_k_simplices_adjacency_matrix (SimplicialComplex*, int**, int, int);

int check_neighbor_adjacency_matrix (int**, int*, int, int);
int check_neighbor (SimplicialComplex*, int, int);


// Costruisce il complesso simpliciale completo a partire dagli 1-simplessi, utilizzando la matrice di adiacenza.
// I vertici sono nominati da 0 a n-1, come gli indici della matrice.
SimplicialComplex* complex_from_adjacency_matrix_complete (int** M, int n) {
    SimplicialComplex* complex = (SimplicialComplex*) malloc((n)*sizeof(SimplicialComplex));
    int i;

    // aggiungo i vertici
    complex[0].size=n;
    Simplex *current, *new_simplex; // current sarà un puntatore all'ultimo elemento della lista degli 0-simplessi, new_simplex servirà per creare quelli da aggiungere

    new_simplex = (Simplex*) malloc(sizeof(Simplex));
    new_simplex->next = NULL;
    new_simplex->position = 0;
    new_simplex->vertices = (int*) malloc(sizeof(int));
    new_simplex->vertices[0]=0;
    complex[0].simplices=new_simplex;
    current=new_simplex;

    for (i=1; i<n; i++) {
        new_simplex = (Simplex*) malloc(sizeof(Simplex));
        new_simplex->next = NULL;
        new_simplex->position = i;
        new_simplex->vertices = (int*) malloc(sizeof(int));
        new_simplex->vertices[0]=i;
        current->next=new_simplex;
        current=current->next;
    }

    // costruisco fino al livello n
    for (i=1; i<n; i++) {
        if (complex[i-1].simplices!=NULL) { // posso costruire i-simplessi solo se esistono degli (i-1)-simplessi
            add_k_simplices_adjacency_matrix(complex,M,n,i);
        }
        else {
            complex[i].simplices=NULL;
            complex[i].size=0;
        }
    }

    return complex;
}

// Costruisce il complesso simpliciale a partire dagli 1-simplessi troncato fino ai k-simplessi (inclusi), utilizzando la matrice di adiacenza.
// I vertici sono nominati da 0 a n-1, come gli indici della matrice.
// Utile ad esempio se serve calcolare il gruppo di omologia H_(k-1) e non serve l'intero complesso simpliciale.
SimplicialComplex* complex_from_adjacency_matrix_truncated (int** M, int n, int k) {
    SimplicialComplex* complex = (SimplicialComplex*) malloc((k+1)*sizeof(SimplicialComplex));
    int i;

    // aggiungo i vertici
    complex[0].size=n;
    Simplex *current, *new_simplex; // current sarà un puntatore all'ultimo elemento della lista degli 0-simplessi, new_simplex servirà per creare quelli da aggiungere

    new_simplex = (Simplex*) malloc(sizeof(Simplex));
    new_simplex->next = NULL;
    new_simplex->position = 0;
    new_simplex->vertices = (int*) malloc(sizeof(int));
    new_simplex->vertices[0]=0;
    complex[0].simplices=new_simplex;
    current=new_simplex;

    for (i=1; i<n; i++) {
        new_simplex = (Simplex*) malloc(sizeof(Simplex));
        new_simplex->next = NULL;
        new_simplex->position = i;
        new_simplex->vertices = (int*) malloc(sizeof(int));
        new_simplex->vertices[0]=i;
        current->next=new_simplex;
        current=current->next;
    }

    // costruisco fino al livello k
    for (i=1; i<=k; i++) {
        if (complex[i-1].simplices!=NULL) { // posso costruire i-simplessi solo se esistono degli (i-1)-simplessi
            add_k_simplices_adjacency_matrix(complex,M,n,i);
        }
        else {
            complex[i].simplices=NULL;
            complex[i].size=0;
        }
    }

    return complex;
}

// Costruisce il complesso simpliciale completo a partire dagli n vertici e dagli 1-simplessi.
// I vertici verranno rinominati a partire da 0 e con interi consecutivi (se vogliamo posso scrivere una funzione per farli ritornare ai nomi originali)
SimplicialComplex* complex_from_1_simplices_complete (SimplicialComplex* complex, int n) {
    int i,j;
    int** M = (int**) malloc(n*sizeof(int*));
    for (i=0; i<n; i++) M[i] = (int*) malloc(n*sizeof(int));
    // salvo per comodita' in un array i vertici (per ovviare a problemi di nomenclatura ed evitare di scorrere sempre la lista)
    int* vertices = (int*) malloc(n*sizeof(int));
    Simplex* current=complex[0].simplices;
    i=0;
    while (current!=NULL) {
        vertices[i]=current->vertices[0];
        i++;
        current=current->next;
    }
    
    // costruisco la matrice di adiacenza
    for (i=0; i<n; i++) {
        M[i][i]=1;
        for (j=i+1; j<n; j++) {
            // controllo se il vertice i-esimo e il j-esimo sono collegati da un 1-simplesso
            if (check_neighbor(complex,vertices[i],vertices[j])!=0) {
                M[i][j]=1; M[j][i]=1;
            }
            else {
                M[i][j]=0; M[j][i]=0;
            }
        }
    }

    return complex_from_adjacency_matrix_complete(M,n);
}

// Costruisce il complesso simpliciale a partire dagli n vertici e dagli 1-simplessi troncato fino ai k-simplessi (inclusi).
// I vertici verranno rinominati a partire da 0 e con interi consecutivi (se vogliamo posso scrivere una funzione per farli ritornare ai nomi originali)
SimplicialComplex* complex_from_1_simplices_truncated (SimplicialComplex* complex, int n, int k) {
    int i,j;
    int** M = (int**) malloc(n*sizeof(int*));
    for (i=0; i<n; i++) M[i] = (int*) malloc(n*sizeof(int));
    // salvo per comodita' in un array i vertici (per ovviare a problemi di nomenclatura ed evitare di scorrere sempre la lista)
    int* vertices = (int*) malloc(n*sizeof(int));
    Simplex* current=complex[0].simplices;
    i=0;
    while (current!=NULL) {
        vertices[i]=current->vertices[0];
        i++;
        current=current->next;
    }

    // costruisco la matrice di adiacenza
    for (i=0; i<n; i++) {
        M[i][i]=1;
        for (j=i+1; j<n; j++) {
            // controllo se il vertice i-esimo e il j-esimo sono collegati da un 1-simplesso
            if (check_neighbor(complex,vertices[i],vertices[j])!=0) {
                M[i][j]=1; M[j][i]=1;
            }
            else {
                M[i][j]=0; M[j][i]=0;
            }
        }
    }

    return complex_from_adjacency_matrix_truncated(M,n,k);
}

// Aggiunge al complesso simpliciale (che contiene fino ai (k-1)-simplessi) i k-simplessi specificati dalla matrice di adiacenza M n*n.
// E' necessario che il complesso sia ordinato e k>0. M[i][j] sara' diversa da 0 se (i,j) e' nel complesso (denomino i vertici a partire da 0).
void add_k_simplices_adjacency_matrix (SimplicialComplex* complex, int** M, int n, int k) {
    // parto con 0 k-simplessi: la lista complex[k].simplices e' vuota
    complex[k].simplices=NULL;
    int num_k_simplices=0;
    int i,j;

    // ricerco i k-simplessi da aggiungere:

    Simplex* previous_simplex = complex[k-1].simplices; // scorrerà i (k-1)-simplessi
    Simplex *current = NULL, *new_simplex; // current sarà un puntatore all'ultimo elemento della lista dei k-simplessi, new_simplex servirà per creare quelli da aggiungere
    while (previous_simplex!=NULL) { // per costruire i k-simplessi devo partire dai (k-1)-simplessi

        // cerco nuovi vertici a partire da quelli successivi all'ultimo gia' presente (funziona se il complesso e' ordinato)
        for (j=previous_simplex->vertices[k-1]+1; j<n; j++) {
            if (check_neighbor_adjacency_matrix(M,previous_simplex->vertices,k,j)!=0) {
                // ho trovato il vicino j: costruisco il nuovo k-simplesso

                if (num_k_simplices==0) { // caso in cui aggiungo il primo k-simplesso alla lista
                    new_simplex = (Simplex*) malloc(sizeof(Simplex));
                    new_simplex->next = NULL;
                    new_simplex->position = num_k_simplices;
                    new_simplex->vertices = (int*) malloc((k+1)*sizeof(int));
                    for (i=0; i<k; i++) new_simplex->vertices[i]=previous_simplex->vertices[i];
                    new_simplex->vertices[k]=j;

                    complex[k].simplices=new_simplex;
                    current=new_simplex;
                }
                else { // caso in cui la lista dei k-simplessi e' non vuota
                    new_simplex = (Simplex*) malloc(sizeof(Simplex));
                    new_simplex->next = NULL;
                    new_simplex->position = num_k_simplices;
                    new_simplex->vertices = (int*) malloc((k+1)*sizeof(int));
                    for (i=0; i<k; i++) new_simplex->vertices[i]=previous_simplex->vertices[i];
                    new_simplex->vertices[k]=j;

                    current->next=new_simplex;
                    current=current->next;
                }
                num_k_simplices++;
            }
        }

        previous_simplex = previous_simplex->next;
    }

    complex[k].size=num_k_simplices;
    return;
}

// Controlla se il vertice di indice j e' "vicino" ai k vertici di indici rows, ossia se il minore di righe rows e colonna j e' tutto non nullo.
// Restituisce 0 se j non e' un vicino, 1 se lo e'.
int check_neighbor_adjacency_matrix (int** M, int* rows, int k, int j) {
    for (int i=0; i<k; i++) {
        if (M[rows[i]][j]==0) return 0;
    }
    return 1;
}

// Controlla se a e b sono "vicini", ossia se la coppia (a,b) appartiene agli 1-simplessi. Restituisce 0 se non sono vicini, 1 se lo sono.
int check_neighbor (SimplicialComplex* complex, int a, int b) {
    Simplex* current=complex[1].simplices;
    while (current!=NULL) { // scorro lungo gli 1-simplessi
        if ((current->vertices[0]==a && current->vertices[1]==b) || (current->vertices[0]==b && current->vertices[1]==a)) return 1;
        current=current->next;
    }
    return 0;
}
