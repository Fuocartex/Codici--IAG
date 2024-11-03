#pragma once
// MioFile.h
#ifndef Complesso_Simplciale
#define Complesso_simpliciale

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


// funzione per creare un sottoinsieme di un simplesso
int* creasubs(int* v, int i, int n) {
    int* s = (int*)malloc((n) * sizeof(int));
    int k = 0;
	//porto il cilco fino a n perchè il vertice i-esimo non deve essere inserito quindi inserisco solo n-1 vertici
    for (int j = 0; j <= n; j++) {
        if (j != i) {
            s[k] = v[j];
            k++;
        }
    }
    return s;
}

//controlla se due vettori sono uguali
bool equal(int* v, int* s, int n) {
    for (int i = 0; i <= n; i++) {
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
	//fino a quando non ho terminato la lista degli n-simplessi
    while (Nsimp) {
        for (int i = 0; i <= sizeN; i++) {
			//creo i sottoinsiemi del n-simplesso
            v = creasubs(Nsimp->vertices, i, sizeN);
            isin = false;
			//fino a quando non ho terminato la lista degli n-1-simplessi oppure ho trovato il sottoinsieme
            while (Nmosimp && !isin) {
				//confronto i due vettori
                if (equal(Nmosimp->vertices, v, sizeN - 1)) {
                    isin = true;
                }
                else
                    isin = false;
                Nmosimp = Nmosimp->next;
            }
            if (!isin)
                return false;
            Nmosimp = s1;
        }
        Nsimp = Nsimp->next;
    }
    return true;
}

// Funzione per controllare se il complesso è simpliciale
bool isSimmplicial(SimplicialComplex* sc, int size) {
	//faccio il controllo per ogni n-simplesso partendo dall'ultimo
    for (int i = size - 1; i > 0; i--)
        if (!isIn(sc[i].simplices, sc[i - 1].simplices, sc[i].size))
            return false;
    return true;
}

// Funzione per creare un simplesso
Simplex* createSimplex(int size, int n) {
    Simplex* simplex, * head, * app;      // Definiamo la testa del simplicio + due simplex ausiliari
    simplex = (Simplex*)malloc(sizeof(Simplex));
	//mi salvo la testa del simplesso e la riempio
    head = simplex;
    simplex->next = NULL;
    simplex->vertices = (int*)malloc((size + 1) * sizeof(int));
    printf("Inserisci i %d-simplessi scrivendo i vertici in ordine crescente separati da uno spazio es: (1 2 3)\n", size);
    for (int j = 0; j < size + 1; j++) {
        scanf("%d", &simplex->vertices[j]);
    }
	//riempio i restanti n-1 simplessi utilizzando un simplex ausiliario
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
    return head;
}

#endif