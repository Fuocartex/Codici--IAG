#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Compl_Simpl.h"

bool isSimplex = false;

// Definizione di un simplicio


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