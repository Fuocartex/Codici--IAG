#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definizione della struttura "simplex"
typedef struct simplex {
    int* vertices;           // Array dinamico contenente i vertici (ordinato)
    int position;            // Numero di vertici (dimensione + 1)
    struct simplex* next;    // Puntatore al simplesso successivo nella lista concatenata
} simplex;

// Definizione della struttura "SimplicialComplex"
typedef struct {
    simplex* simplices;      // Puntatore alla lista concatenata di simplessi
    int size;                // Numero di simplessi presenti
} SimplicialComplex;

// Funzione di confronto per qsort (ordinamento crescente)
int cmpInt(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return int_a - int_b;
}

// Funzione che verifica se il simplesso "sub" è un sottoinsieme del simplesso "sup"
// Entrambi gli array di vertici devono essere ordinati in ordine crescente.
bool isSubset(simplex* sub, simplex* sup) {
    int i = 0, j = 0;
    while (i < sub->position && j < sup->position) {
        if (sub->vertices[i] == sup->vertices[j]) {
            i++;
            j++;
        } else if (sub->vertices[i] > sup->vertices[j]) {
            j++;
        } else { // sub->vertices[i] < sup->vertices[j]
            return false;
        }
    }
    return (i == sub->position);
}

// Funzione per aggiungere un simplesso alla struttura SimplicialComplex
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

int main(void) {
    int k;
    printf("Inserisci il numero massimo di dimensioni (k): ");
    if (scanf("%d", &k) != 1) {
        fprintf(stderr, "Input non valido.\n");
        return 1;
    }

    // Allocazione dinamica di un array di SimplicialComplex per dimensioni da 0 a k.
    SimplicialComplex* complexes = (SimplicialComplex*)malloc((k + 1) * sizeof(SimplicialComplex));
    if (complexes == NULL) {
        fprintf(stderr, "Errore di allocazione della memoria.\n");
        return 1;
    }
    
    int i, dim, s, j, supDim;
    // Inizializzazione dei complessi
    for (i = 0; i <= k; i++) {
        complexes[i].simplices = NULL;
        complexes[i].size = 0;
    }

    // Costruzione automatica degli 0-simplessi
    int numZeroSimplices;
    printf("Inserisci il numero di 0-simplessi: ");
    if (scanf("%d", &numZeroSimplices) != 1) {
        fprintf(stderr, "Input non valido.\n");
        free(complexes);
        return 1;
    }
    for (i = 0; i < numZeroSimplices; i++) {
        int* vertex = (int*)malloc(sizeof(int));
        if (vertex == NULL) {
            fprintf(stderr, "Errore di allocazione della memoria.\n");
            exit(1);
        }
        vertex[0] = i + 1;  // Gli 0-simplessi sono rappresentati da un singolo vertice
        addSimplex(&complexes[0], vertex, 1);
    }

    // Input e costruzione dei simplessi per dimensioni maggiori di 0.
    // Per ogni dimensione dim (da 1 a k), ogni simplesso ha (dim + 1) elementi.
    for (dim = 1; dim <= k; dim++) {
        int numSimplices;
        printf("Inserisci il numero di %d-simplessi: ", dim);
        if (scanf("%d", &numSimplices) != 1) {
            fprintf(stderr, "Input non valido.\n");
            exit(1);
        }
        for (s = 0; s < numSimplices; s++) {
            int m = dim + 1;
            int* vertices = (int*)malloc(m * sizeof(int));
            if (vertices == NULL) {
                fprintf(stderr, "Errore di allocazione della memoria.\n");
                exit(1);
            }
            printf("Inserisci gli elementi del %d-simplesso %d: ", dim, s + 1);
            for (j = 0; j < m; j++) {
                if (scanf("%d", &vertices[j]) != 1) {
                    fprintf(stderr, "Input non valido.\n");
                    exit(1);
                }
            }
            // Ordinamento degli elementi per il corretto funzionamento di isSubset
            qsort(vertices, m, sizeof(int), cmpInt);
            addSimplex(&complexes[dim], vertices, m);
        }
    }

    // Processo per trovare i simplessi massimali.
    // Per ogni simplesso di dimensione inferiore a k, si verifica se è contenuto in un simplesso di dimensione superiore.
    for (dim = k - 1; dim >= 0; dim--) {
        simplex* newList = NULL;
        int newSize = 0;
        simplex* current = complexes[dim].simplices;
        while (current != NULL) {
            bool isMaximal = true;
            for (supDim = dim + 1; supDim <= k && isMaximal; supDim++) {
                simplex* candidate = complexes[supDim].simplices;
                while (candidate != NULL && isMaximal) {
                    if (isSubset(current, candidate)) {
                        isMaximal = false;
                    }
                    candidate = candidate->next;
                }
            }
            simplex* nextNode = current->next;
            if (isMaximal) {
                current->next = newList;
                newList = current;
                newSize++;
            } else {
                free(current->vertices);
                free(current);
            }
            current = nextNode;
        }
        complexes[dim].simplices = newList;
        complexes[dim].size = newSize;
    }

    // Output dei complessi massimali
    printf("I complessi massimali sono:\n");
    for (dim = 0; dim <= k; dim++) {
        if (complexes[dim].simplices != NULL) {
            printf("%d-simplessi massimali:\n", dim);
            simplex* cur = complexes[dim].simplices;
            while (cur != NULL) {
                printf("{");
                for (j = 0; j < cur->position; j++) {
                    if (j > 0) {
                        printf(", ");
                    }
                    printf("%d", cur->vertices[j]);
                }
                printf("}\n");
                cur = cur->next;
            }
        }
    }

    // Deallocazione della memoria utilizzata
    for (dim = 0; dim <= k; dim++) {
        simplex* cur = complexes[dim].simplices;
        while (cur != NULL) {
            simplex* next = cur->next;
            free(cur->vertices);
            free(cur);
            cur = next;
        }
    }
    free(complexes);

    return 0;
}

