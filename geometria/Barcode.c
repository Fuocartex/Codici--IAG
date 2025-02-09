#include "..\Include\Barcode.h"

int main()
{
	
	matrici_persistenza* mp = NULL;
	mp = create_M_P();
	M_P* mod_p = create_Modulo_Persistenza(mp);
	print1(mod_p);

	int h = beta_i_j(mod_p, 11.5, 11.5, 0, 1);

	printf("beta_i_j = %d\n", h);

	int** matrix = NULL;
	matrix = beta_matrix(0, 11, 0.5, mod_p, 0, 1);

	//print_matrix(matrix, 24, 24);

	int** mu = NULL;
	mu = mu_matrix(matrix, 24);
	//printf("mu\n");
	print_matrix(mu, 24, 24);
	//printf("beta_i_j = %d\n", h);

	char* json_matrix = matrix_to_json(mu, 24);
    
	//printf("Matrice in formato JSON: %s\n", json_matrix);

    int n = 24;  // Supponiamo che la matrice sia 100x100
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
    free(json_matrix);
    for (int i = 0; i < 24; i++) {
        free(matrix[i]);
    }
    free(matrix);

	return 0;
}