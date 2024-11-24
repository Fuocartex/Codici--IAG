#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Compl_Simpl.h"

bool isSimplex = false;

// Definizione di un simplicio


int main() {
    int size;
    int  n = 0;

    // Chiediamo all'utente qual'è il simplesso più grande per inizializzare lo spazio
    printf("Inserisci la grandezza del simplesso piu' grande nel complesso: ");
    scanf("%d", &size);

    // Inizializziamo lo spazio per i simplici
    SimplicialComplex* complex = readComplex(size);
    
    if (isSimplicial(complex, size))
        printf("L'isnieme e' simpliciale\n");
    else
        printf("L'insieme non e' simpliciale\n");


    // Stampiamo il complesso simpliciale
    printComplex(complex, size);
    
    return 0;
}
