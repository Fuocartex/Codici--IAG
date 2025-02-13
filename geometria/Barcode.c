#include "..\Include\Barcode.h"

int main()
{
    int** m = NULL;
	int* k = 0;
	double* l_max = malloc(sizeof(double));
	m = input_point(&k);
	int n = k;
    double** d = NULL;
	
    d = distance_matrix(m, n);
	
	printf("Matrice delle distanze:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%lf ", d[i][j]);
		}
		printf("\n");
	}
	matrici_persistenza* mp = NULL;
	mp = create_matrix_persistenza(d, n, l_max);

	//matrici_persistenza* mp = NULL;
	//mp = create_M_P();
	M_P* mod_p = create_Modulo_Persistenza(mp);
	//print1(mod_p);

	//int h = beta_i_j(mod_p, 11.5, 11.5, 0, 1);

	//printf("beta_i_j = %d\n", h);

	int** matrix = NULL;
	//printf("l_max = %lf\n", l_max[0]);
	int lambda = ceil(l_max[0]);
	//printf("l_max = %d\n", lambda);
	matrix = beta_matrix(0, lambda, 0.5, mod_p, 1, 1);
	int dim = lambda / 0.5 + 2;
	//print_matrix(matrix, dim, dim);

	int** mu = NULL;
	mu = mu_matrix(matrix, dim);
	//printf("mu\n");
	//print_matrix(mu, dim, dim);
	//printf("beta_i_j = %d\n", h);

	char* json_matrix = matrix_to_json(mu, dim);
    
	//printf("Matrice in formato JSON: %s\n", json_matrix);

    n = dim;  // Supponiamo che la matrice sia 100x100
    int matrix_size = n * n * 4; // Stima della memoria necessaria (interi in JSON)
    int command_size = matrix_size + 100; // Qualche byte extra per il comando

    char* command = malloc(command_size);
    if (command == NULL) {
        printf("Errore nell'allocazione della memoria!\n");
        return 1;
    }

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
    for (int i = 0; i < 24; i++) {
        free(matrix[i]);
    }
    free(matrix);*/
	
	return 0;
}