#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isSimplex = false;

// Definizione di un simplicio
typedef struct Simplex {
    int* vertices;  // Array di vertici del simplicio
    int size;       // Numero di vertici nel simplicio
    struct Simplex* next;
} Simplex;

// Definizione di un complesso simpliciale
typedef struct {
    Simplex* simplices;  // Array di simplici
    int size;            // Numero del simplesso 
} SimplicialComplex;

//Funzione per controllare se l'insieme è simplicale

int* creasubs(int* v, int i, int n) {
    int* s = (int*)malloc((n) * sizeof(int));
    int k = 0;
    for (int j = 0; j <= n; j++) {
        //printf("\n\n\n j = %d\ni = %d\n n = %d \n", j, i, n);
        if (j != i) {
            s[k] = v[j];
            //if (v[j] == 0)
            k++;
        }
    }
    return s;
}

//controlla se due vettori sono uguali
bool equal(int* v, int* s, int n) {
    for (int i = 0; i <= n; i++) {
        //printf("\n\n\n v[i] = %d\ns[i]=%d\n", v[i], s[i]);
        if (v[i] != s[i])
            return false;
    }
    return true;
}

//Funzione per controllare se i sottoinsiemi dell' n simplesso sono contenuti nell' n-1 simplesso
bool isIn(Simplex* s, Simplex* s1, int sizeN) {
    int* v = (int*)malloc((sizeN - 1) * sizeof(int));
    Simplex* Nsimp = s, * Nmosimp = s1;
    bool isin = false;
    while (Nsimp) {
        //printf("isin = %d", isin);
        for (int i = 0; i <= sizeN; i++) {
            v = creasubs(Nsimp->vertices, i, sizeN);
            //printf("i = %d \nv = %d\n",i, v[0]);
            isin = false;
            while (Nmosimp && !isin) {
                //printf("sizeN = %d\n", sizeN);
                if (equal(Nmosimp->vertices, v, sizeN - 1)) {
                    isin = true;
                }
                else
                    isin = false;
                Nmosimp = Nmosimp->next;
                //printf("2\tisin = %d\n i = %d\n", isin, i);
            }
            if (!isin)
                return false;
            Nmosimp = s1;
        }
        Nsimp = Nsimp->next;
    }
    return true;
}

bool isSimmplicial(SimplicialComplex* sc, int size) {
    for (int i = size - 1; i > 0; i--)
        if (!isIn(sc[i].simplices, sc[i - 1].simplices, sc[i].size))
            return false;
    return true;
}

// Funzione per creare un simplesso
Simplex* createSimplex(int size, int n) {
    Simplex* simplex, * head, * app;      // Definiamo la testa del simplicio + due simplex ausiliari
    simplex = (Simplex*)malloc(sizeof(Simplex));
    head = simplex;
    simplex->next = NULL;
    simplex->vertices = (int*)malloc((size + 1) * sizeof(int));
    printf("Inserisci i %d-simplessi scrivendo i vertici in ordine crescente separati da uno spazio es: (1 2 3)\n", size);
    for (int j = 0; j < size + 1; j++) {
        scanf("%d", &simplex->vertices[j]);
    }
    for (int i = 1; i < n; i++) {
        app = (Simplex*)malloc(sizeof(Simplex));
        app->next = NULL;
        app->vertices = (int*)malloc((size + 1) * sizeof(int));
        for (int j = 0; j < size + 1; j++) {
            scanf("%d", &app->vertices[j]);
        }
        simplex->next = app;
        simplex = simplex->next;
    }
    return head; //Tolgo il primo elemento della lista perchè vuoto
}

int main() {
    SimplicialComplex* complex;
    int size;
    int  n = 0;

    printf("ciao");

    // Chiediamo all'utente qual'è il simplesso più grande per inizializzare lo spazio
    printf("Inserisci la grandezza del simplesso più grande nel complesso: ");
    scanf("%d", &size);

    // Inizializziamo lo spazio per i simplici
    complex = (SimplicialComplex*)malloc(size * sizeof(SimplicialComplex));

    for (int i = 0; i < size; i++) {
        complex[i].size = i;
        printf("Inserisci il numero di %d-simplessi: ", i);
        scanf("%d", &n);
        complex[i].simplices = createSimplex(i, n);
    }
    //printf("size = %d", complex[1].simplices->vertices[0]);
    if (isSimmplicial(complex, size))
        printf("L'isnieme e' simpliciale\n");
    else
        printf("L'insieme non e' simpliciale\n");


    // Stampiamo il complesso simpliciale
    Simplex* app;
    for (int i = 0; i < size; i++) {
        app = complex[i].simplices;
        printf("{ ");
        while (app) {
            for (int j = 0; j <= complex[i].size; j++)
                printf("%d ", app->vertices[j]);
            app = app->next;
            if(app)
                printf(", ");
        }
        printf("}\n");
    }
    return 0;
}