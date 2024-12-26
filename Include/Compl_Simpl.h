#pragma once
// MioFile.h
#ifndef Complesso_Simplciale
    #define Complesso_simpliciale

    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>   

    /*Il complesso simpliciale è formato da un vettore di liste di vettori
    Ogni cella del primo vettore conterrà i rispettivi simplessi cella 0 gli 0-simpelessi cella 1 gli 1-simplessi etc..
    a loro volta gli n-simplessi sono organizzati in liste di vettori così da distinguere i vertici che compongono il rispettivo simplesso.*/


    //Definzione delle strutture
    //DEfinzione degli n-simplessi
    typedef struct Simplex {
        int* vertices;  // Array di vertici del simplicio
        int position;   // Ordine del simplicio nella base  
        struct Simplex* next;
    } Simplex;

    // Definizione di un complesso simpliciale
    typedef struct {
        Simplex* simplices;  // Array di simplici
        int size;            // Numero degli n-simplessi esenziale per costruire le matrici di bordo e non scorrere tutta la lista 
    } SimplicialComplex;


    //Definizione delle funzioni
    Simplex* createSimplex(int, int);
    SimplicialComplex* readComplex(int);
    void printComplex(SimplicialComplex*, int);
    int* creasubs(int*, int, int);
    bool equal(int*, int*, int);
    bool isIn(Simplex*, Simplex*, int);
    bool isSimplicial(SimplicialComplex*, int);
	int** edge_Matrix(SimplicialComplex*, int);
    int base_number(Simplex*, int*, int);
	int matrix_rank(int**, int, int);

    //Esplicitare le funzioni

    #pragma region gestione dei simplessi e dei complessi simpliciali

        // funzione per leggere un complesso da input quindi sappiamo la misura del complesso più grande = size
        SimplicialComplex* readComplex(int size){
	        SimplicialComplex* complex = (SimplicialComplex*)malloc(size * sizeof(SimplicialComplex));
	        int n = 0;

	        //itero i vari n-simplessi su tutta la lunghezza del vettore
	        for (int i = 0; i < size; i++) {
		        printf("Inserisci il numero di %d-simplessi: ", i);
		        scanf("%d", &n);
                complex[i].size = n;
		        complex[i].simplices = createSimplex(i, n);
	        }
	        return complex;
        }

        // funzione per stampare un simplesso 
        void printComplex(SimplicialComplex* complex, int size){
	        Simplex* app;
		        for (int i = 0; i < size; i++) {
			        app = complex[i].simplices;
			        printf("{ ");
			        while (app) {
                        printf("( ");
				        for (int j = 0; j <= i; j++)
					        printf("%d ", app->vertices[j]);
                        printf(")");
				        app = app->next;
				        if(app)
					        printf(", ");
			        }
			        printf("}\n");
		        }
	        return;
        }

        // funzione per creare un sottoinsieme di un simplesso togliendo il vertice i-esimo
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
            for (int i = 0; i < n; i++) {
                if (v[i] != s[i])
                    return false;
            }
            return true;
        }

        //Funzione per controllare se i sottoinsiemi dell' n simplesso sono contenuti nell' n-1 simplesso
        bool isIn(Simplex* s, Simplex* s1, int sizeN) {
            int* v = (int*)malloc(sizeN * sizeof(int));
            Simplex* Nsimp = s, * Nmosimp = s1;
            bool isin = false;
	        //fino a quando non ho terminato la lista degli n-simplessi
            while (Nsimp) {
                for (int i = 0; i < sizeN + 1; i++) {
			        //creo i sottoinsiemi del n-simplesso
                    v = creasubs(Nsimp->vertices, i, sizeN+1);
                    isin = false;
			        //fino a quando non ho terminato la lista degli n-1-simplessi oppure ho trovato il sottoinsieme
                    while (Nmosimp && !isin) {
				        //confronto i due vettori
                        if (equal(Nmosimp->vertices, v, sizeN)) {
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
        bool isSimplicial(SimplicialComplex* sc, int size) {
	        //faccio il controllo per ogni n-simplesso partendo dall'ultimo
            for (int i = size - 1; i > 0; i--)
                if (!isIn(sc[i].simplices, sc[i - 1].simplices, i))
                    return false;
            return true;
        }

        // Funzione per creare un simplesso sapendo prima le dimensioni
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
	        simplex->position = 0;
	        //riempio i restanti n-1 simplessi utilizzando un simplex ausiliario
            for (int i = 1; i < n; i++) {
                app = (Simplex*)malloc(sizeof(Simplex));
                app->next = NULL;
                app->vertices = (int*)malloc((size + 1) * sizeof(int));
                for (int j = 0; j < size + 1; j++) {
                    scanf("%d", &app->vertices[j]);
                }
		        app->position = i;
                simplex->next = app;
                simplex = simplex->next;
            }
            return head;
        }

    #pragma endregion


    #pragma region edge_Matrix

		// Funzione per calcolare la matrice di bordo
		int** edge_Matrix(SimplicialComplex* sc, int n) {
            int i = 0, j = 0, k = -1, row = sc[n - 1].size, col = sc[n].size;
            int** matrix;
            matrix = calloc(row,sizeof(int*));
			Simplex* Nsimp, * Nmosimp;
			int* v = (int*)malloc(n * sizeof(int));
			Nsimp = sc[n].simplices;
			Nmosimp = sc[n - 1].simplices;
			//itero i vari n-simplessi
            for (int i = 0; i < row; i++)
                matrix[i] = calloc(col, sizeof(int));
            for (int i = 0; i < sc[n].size; i++) {
				//itero i vari n-1-simplessi
				//creo i sotto inisiemi di ciascun simplesso e vedo che posizione ha nella base
                for (int j = 0; j <= n; j++) {
					v = creasubs(Nsimp->vertices, j, n);
					k = base_number(Nmosimp, v, n);
					printf("k = %d\n", k);
                    if (k != -1) {
                        matrix[k][i] = pow(-1, j);
                    }
                }
				Nsimp = Nsimp->next;
			}
			return matrix;
		}

		int base_number(Simplex* sc, int* v, int n) {
			Simplex* app = sc;
			while (app) {
				if (equal(app->vertices, v, n))
					return app->position;
				app = app->next;
			}
			return -1;
		}
    #pragma endregion

    #pragma region matrix_rank
        void swap_rows(int** matrix, int row1, int row2, int cols) {
            for (int i = 0; i < cols; i++) {
                double temp = matrix[row1][i];
                matrix[row1][i] = matrix[row2][i];
                matrix[row2][i] = temp;
            }
        }

		// Funzione per calcolare il rango di una matrice
		int matrix_rank(int** matrix, int rows, int cols) {
            int rank = cols;  // Inizialmente assumiamo il rango massimo possibile

            for (int row = 0; row < rank; row++) {
                // Controlliamo se l'elemento diagonale è diverso da zero
                if (fabs(matrix[row][row]) > 0) {
                    // Eliminiamo gli elementi sotto l'elemento pivot
                    for (int i = 0; i < rows; i++) {
                        if (i != row) {
                            double factor = matrix[i][row] / matrix[row][row];
                            for (int j = 0; j < cols; j++) {
                                matrix[i][j] -= factor * matrix[row][j];
                            }
                        }
                    }
                }
                else {
                    // Se il pivot è zero, cerchiamo una riga non nulla da scambiare
                    int reduce = 1;
                    for (int i = row + 1; i < rows; i++) {
                        if (fabs(matrix[i][row]) > 0) {
                            swap_rows(matrix, row, i, cols);
                            reduce = 0;
                            break;
                        }
                    }

                    if (reduce) {
                        // Se non troviamo una riga valida, riduciamo il rango
                        rank--;

                        // Spostiamo l'ultima colonna a sinistra
                        for (int i = 0; i < rows; i++) {
                            matrix[i][row] = matrix[i][rank];
                        }
                    }
                    row--;
                }
            }

            return rank;
		}
#pragma endregion

#endif
