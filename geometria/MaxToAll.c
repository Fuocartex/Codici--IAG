#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct simplex {
    int* vertices;              // Array dinamico dei vertici (già ordinato)
    int position;               // Numero di vertici (dimensione + 1)
    struct simplex* next;       // Puntatore al simplesso successivo (lista concatenata)
} simplex;

typedef struct {
    simplex* simplices;         // Puntatore alla lista concatenata dei simplessi
    int size;                   // Numero di simplessi presenti
} SimplicialComplex;

// Funzione di confronto per qsort (ordinamento crescente)
int cmpInt(const void *a, const void *b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_a - int_b;
}

// Aggiunge un simplesso alla struttura SimplicialComplex
void addSimplex(SimplicialComplex* complex, int* vertices, int numVertices) {
    simplex* newNode = (simplex*)malloc(sizeof(simplex));
    if(newNode == NULL) {
        fprintf(stderr, "Errore di allocazione della memoria.\n");
        exit(1);
    }
    newNode->vertices = vertices;
    newNode->position = numVertices;
    newNode->next = complex->simplices;
    complex->simplices = newNode;
    complex->size++;
}

// Confronta due simplessi (si assume che abbiano lo stesso numero di vertici)
int compareSimplex(const simplex* a, const simplex* b) {
    int i;
    if(a->position != b->position)
        return a->position - b->position;
    for(i = 0; i < a->position; i++) {
        if(a->vertices[i] != b->vertices[i])
            return a->vertices[i] - b->vertices[i];
    }
    return 0;
}

// Rimuove i duplicati all'interno di un SimplicialComplex (lista concatenata)
void removeDuplicates(SimplicialComplex* complex) {
    simplex *current, *runner, *prevRunner, *temp;
    current = complex->simplices;
    while(current != NULL) {
        prevRunner = current;
        runner = current->next;
        while(runner != NULL) {
            if(compareSimplex(current, runner) == 0) {
                prevRunner->next = runner->next;
                temp = runner;
                runner = runner->next;
                free(temp->vertices);
                free(temp);
                complex->size--;
            } else {
                prevRunner = runner;
                runner = runner->next;
            }
        }
        current = current->next;
    }
}

// Genera tutti i sottoinsiemi non vuoti di un simplesso massimale e li aggiunge
// alla struttura dei simplessi "allComplexes" in base alla loro dimensione.
void generateSubsetsForSimplex(const simplex* maxSimplex, SimplicialComplex* allComplexes) {
    int n = maxSimplex->position;
    int total = 1 << n; // 2^n
    int i, j;
    for(i = 1; i < total; i++) {
        int count = 0;
        for(j = 0; j < n; j++) {
            if(i & (1 << j))
                count++;
        }
        int* subset = (int*)malloc(count * sizeof(int));
        if(subset == NULL) {
            fprintf(stderr, "Errore di allocazione della memoria.\n");
            exit(1);
        }
        int index = 0;
        for(j = 0; j < n; j++) {
            if(i & (1 << j)) {
                subset[index] = maxSimplex->vertices[j];
                index++;
            }
        }
        // La dimensione del simplesso è count-1 (numero di vertici - 1)
        int subsetDim = count - 1;
        addSimplex(&allComplexes[subsetDim], subset, count);
    }
}

// Libera tutta la memoria allocata per un SimplicialComplex
void freeSimplicialComplex(SimplicialComplex* complex) {
    simplex* cur = complex->simplices;
    simplex* next;
    while(cur != NULL) {
        next = cur->next;
        free(cur->vertices);
        free(cur);
        cur = next;
    }
    complex->simplices = NULL;
    complex->size = 0;
}

int main(void) {
    int k;
    printf("Inserisci il numero massimo di dimensioni (k): ");
    if(scanf("%d", &k) != 1) {
        fprintf(stderr, "Input non valido.\n");
        return 1;
    }
    
    // Allocazione dinamica di due array di SimplicialComplex (uno per i simplessi massimali e uno per tutti quelli generati)
    SimplicialComplex* maxSimplicialComplexes = (SimplicialComplex*)malloc((k + 1) * sizeof(SimplicialComplex));
    SimplicialComplex* allSimplicialComplexes = (SimplicialComplex*)malloc((k + 1) * sizeof(SimplicialComplex));
    if(maxSimplicialComplexes == NULL || allSimplicialComplexes == NULL) {
        fprintf(stderr, "Errore di allocazione della memoria.\n");
        exit(1);
    }
    
    int dim, i, j;
    // Inizializzazione delle strutture per ogni dimensione
    for(dim = 0; dim <= k; dim++) {
        maxSimplicialComplexes[dim].simplices = NULL;
        maxSimplicialComplexes[dim].size = 0;
        allSimplicialComplexes[dim].simplices = NULL;
        allSimplicialComplexes[dim].size = 0;
    }
    
    // Input dei simplessi massimali per ogni dimensione
    for(dim = 0; dim <= k; dim++) {
        int numMax;
        printf("Inserisci il numero di %d-simplessi massimali: ", dim);
        if(scanf("%d", &numMax) != 1) {
            fprintf(stderr, "Input non valido.\n");
            exit(1);
        }
        for(i = 0; i < numMax; i++) {
            int m = dim + 1;
            int* vertices = (int*)malloc(m * sizeof(int));
            if(vertices == NULL) {
                fprintf(stderr, "Errore di allocazione della memoria.\n");
                exit(1);
            }
            printf("Inserisci gli elementi del %d-simplesso massimale %d: ", dim, i + 1);
            for(j = 0; j < m; j++) {
                if(scanf("%d", &vertices[j]) != 1) {
                    fprintf(stderr, "Input non valido.\n");
                    exit(1);
                }
            }
            qsort(vertices, m, sizeof(int), cmpInt);
            addSimplex(&maxSimplicialComplexes[dim], vertices, m);
        }
    }
    
    // Per ogni simplesso massimale, genera tutti i suoi sottoinsiemi
    simplex* current;
    for(dim = 0; dim <= k; dim++) {
        current = maxSimplicialComplexes[dim].simplices;
        while(current != NULL) {
            generateSubsetsForSimplex(current, allSimplicialComplexes);
            current = current->next;
        }
    }
    
    // Rimuove eventuali duplicati per ciascuna dimensione
    for(dim = 0; dim <= k; dim++) {
        removeDuplicates(&allSimplicialComplexes[dim]);
    }
    
    // Output di tutti i simplessi generati per ogni dimensione
    printf("Tutti i simplessi generati sono:\n");
    for(dim = 0; dim <= k; dim++) {
        if(allSimplicialComplexes[dim].simplices != NULL) {
            printf("%d-simplessi generati:\n", dim);
            current = allSimplicialComplexes[dim].simplices;
            while(current != NULL) {
                printf("{");
                for(i = 0; i < current->position; i++) {
                    if(i > 0)
                        printf(", ");
                    printf("%d", current->vertices[i]);
                }
                printf("}\n");
                current = current->next;
            }
        }
    }
    
    // Deallocazione della memoria utilizzata
    for(dim = 0; dim <= k; dim++) {
        freeSimplicialComplex(&maxSimplicialComplexes[dim]);
        freeSimplicialComplex(&allSimplicialComplexes[dim]);
    }
    free(maxSimplicialComplexes);
    free(allSimplicialComplexes);
    
    return 0;
}

