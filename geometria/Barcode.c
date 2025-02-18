#include "..\Include\Barcode.h"

//dati i punti su un piano cartesiano studiamone la topologia attraverrso il barcode

int main()
{
    int** m = NULL;
	int* k = 0;
	double* l_max = malloc(sizeof(double));
	//salvo in m i punti di un piano cartesiano di cui voglio studiare la topologia
	m = input_point(&k);
	int n = k, h = 0, max = 0;
    //double** d = NULL;
	
	//calcolo la matrice delle distanze
    //d = distance_matrix(m, n);
	
	double** d = NULL;
	d = input_null(d, 8, 8);
	int** c = NULL;
	c = input_null(c, 3, 8);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++)
		{
			
			scanf("%d", &c[i][j]);
		}
	}
	
		


	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			d[i][j] = 10 * sqrt(pow((c[0][i] - c[0][j]), 2) + pow((c[1][i] - c[1][j]), 2) + pow((c[2][i] - c[2][j]), 2));
	}
	print_matrix(c, 3, 8);
	
	n = 8;

	printf("Quale omologia vuoi calcolare? h = ");
	scanf("%d", &h);

	printf("Fino a quale k vuoi calcolare i complessi simpliciali? max = ");
	scanf("%d", &max);

	//stampo la matrice delle distanze
	printf("\n\nMatrice delle distanze:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%lf ", d[i][j]);
		}
		printf("\n");
	}

	//creo la successione di matrici partendo dalla matrice delle distanze
	matrici_persistenza* mp = NULL;
	mp = create_matrix_persistenza(d, n, l_max);

	//creo il modulo di persistenza ovvero l'evoluzione dei complessi simpliciali
	M_P* mod_p = create_Modulo_Persistenza(mp, max);
	print1(mod_p, max);

	// calcolo la matrice beta essendomi calcolato a mano la distanza pi� grande tra i punti e aggiungo 2
	int** matrix = NULL;
	int lambda = ceil(l_max[0]);
	matrix = beta_matrix(0, lambda, 0.5, mod_p, h, max);
	int dim = lambda / 0.5 + 2;

	//calcolo la matrice mu
	int** mu = NULL;
	mu = mu_matrix(matrix, dim);
	printf("mu\n");
	print_matrix(mu, dim, dim);
	//printf("beta_i_j = %d\n", h);

	//da qui passo la matrice ad un file python che mi stampa il barcode molto pi� efficiente di c in questo
	//gli passo la matrice come un file json ovvero di caratteri
	char* json_matrix = matrix_to_json(mu, dim);
    
    n = dim;  
    int matrix_size = n * n * 4; // Stima della memoria necessaria (interi in JSON)
    int command_size = matrix_size + 100; // Qualche byte extra per il comando

    char* command = malloc(command_size);
    if (command == NULL) {
        printf("Errore nell'allocazione della memoria!\n");
        return 1;
    }

	//scrivo il comando 
    snprintf(command, command_size, "python barcode1.py %s %lf %lf", json_matrix, 0, 0.5);

    //printf("Eseguo il comando: %s\n", command);

    // Lancia lo script Python
    int ret = system(command);
    if (ret == -1) {
        perror("Errore nell'esecuzione del comando");
        exit(EXIT_FAILURE);
    }

    // Pulizia: liberiamo la memoria allocata
    /*free(json_matrix);
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);*/
	
	return 0;
}