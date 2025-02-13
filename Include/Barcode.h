#pragma once
#ifndef Barcode
	#define Barcode
	#include "Complesso_da_1_simplessi.h"
	#include "Omo.h"

	/*Come creiamo il barcode
	come prima cosa creiamo il modulo di persistenza ovvero la collezione dell'evoluzione dei nostri complessi simplciali
	a questo punto ci chiediamo di quale H vogliamo calcolare il barcode e calcoliamo le rispettive matrici di bordo
	poi calcoliamo una base del nucleo
	applichiamo la trasformazione da H a H+t
	calcoliamo il rango di questa applicazione fusa con la matrice di bordo
	e sottraiamo il rango della matrice di bordo il risultato è l'elemento per il resto dobbiamo avere che j >= i e calcoliamo tutti gli altri elementi*/


	//Definizione modulo di persistenza
	typedef struct Matrici_Persistenza {
		double l_min;
		double l_max;
		int size;
		int** matrix_d;
		struct Matrici_Persistenza* next;
		struct Matrici_Persistenza* prev;
	} matrici_persistenza;

	typedef struct Modulo_Persistenza {
		double l_min;
		double l_max;
		int size;
		SimplicialComplex* sc;
		struct Modulo_Persistenza* next;
		struct Modulo_Persistenza* prev;
	}M_P;

	matrici_persistenza* create_M_P(void);
	M_P* create_Modulo_Persistenza(matrici_persistenza*);
	int** matrix_phi_l1_to_l2(Simplex*, int, Simplex*, int, int);
	int beta_i_j(M_P*, double, double, int, int);
	int** beta_matrix(int, int, double, M_P*, int, int);
	int** mu_matrix(int**, int);
	double** distance_matrix(int**, int);
	int** input_point(int*);
	int** zero_one_matrix(double**, int, double);
	double* find_lambda_value(double**, int);
	matrici_persistenza* create_matrix_persistenza(double**, int, double*);

	matrici_persistenza* create_M_P(void) {
		matrici_persistenza* mp1 = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
		matrici_persistenza* mp2 = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
		matrici_persistenza* mp3 = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
		matrici_persistenza* mp4 = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
		mp1->l_min = 0.5;
		mp1->l_max = 1;
		mp2->l_min = 1.5;
		mp2->l_max = 10;
		mp3->l_min = 10.5;
		mp3->l_max = 10.5;
		mp4->l_min = 11;
		mp4->l_max = 11; // per segnalare la fine
		mp1->size = 3;
		mp2->size = 3;
		mp3->size = 3;
		mp4->size = 3;
		int** m1 = NULL, ** m2 = NULL, ** m3 = NULL, ** m4 = NULL;
		m1 = input_null(m1, 3, 3);
		m2 = input_null(m2, 3, 3);
		m3 = input_null(m3, 3, 3);
		m4 = input_null(m4, 3, 3);
		m1[0][0] = 1; m1[0][1] = 0; m1[0][2] = 0;
		m1[1][0] = 0; m1[1][1] = 1; m1[1][2] = 0;
		m1[2][0] = 0; m1[2][1] = 0; m1[2][2] = 1;
		m2[0][0] = 1; m2[0][1] = 1; m2[0][2] = 0;
		m2[1][0] = 1; m2[1][1] = 1; m2[1][2] = 0;
		m2[2][0] = 0; m2[2][1] = 0; m2[2][2] = 1;
		m3[0][0] = 1; m3[0][1] = 1; m3[0][2] = 0;
		m3[1][0] = 1; m3[1][1] = 1; m3[1][2] = 1;
		m3[2][0] = 0; m3[2][1] = 1; m3[2][2] = 1;
		m4[0][0] = 1; m4[0][1] = 1; m4[0][2] = 1;
		m4[1][0] = 1; m4[1][1] = 1; m4[1][2] = 1;
		m4[2][0] = 1; m4[2][1] = 1; m4[2][2] = 1;
		mp1->matrix_d = m1;
		mp2->matrix_d = m2;
		mp3->matrix_d = m3;
		mp4->matrix_d = m4;
		mp1->next = mp2;
		mp2->next = mp3;
		mp3->next = mp4;
		mp4->next = NULL;
		mp1->prev = NULL;
		mp2->prev = mp1;
		mp3->prev = mp2;
		mp4->prev = mp3;
		return mp1;
	}

	M_P* create_Modulo_Persistenza(matrici_persistenza* mp) {
		M_P* mod_p = NULL, * app = NULL, * new = NULL;
		int size = mp->size;
		mod_p = (M_P*)malloc(sizeof(M_P));
		mod_p->l_min = mp->l_min;
		mod_p->l_max = mp->l_max;
		mod_p->size = size;
		mod_p->sc = complex_from_adjacency_matrix_complete(mp->matrix_d, size);
		mod_p->prev = NULL;
		app = mod_p;

		while (mp->next) {
			mp = mp->next;
			size = mp->size;
			new = (M_P*)malloc(sizeof(M_P));
			new->l_min = mp->l_min;
			new->l_max = mp->l_max;
			new->sc = complex_from_adjacency_matrix_complete(mp->matrix_d, size);
			
			new->prev = mod_p;
			new->next = NULL;
			mod_p->next = new;
			mod_p = new;
		}		
		
		return app;
	}

	int** matrix_phi_l1_to_l2(Simplex* s1, int sizeb1, Simplex* s2, int sizeb2, int size) {
		int** matrix = NULL;
		matrix = input_null(matrix, sizeb2, sizeb1);
		Simplex* current = s1;
		int col = 0;
		while (current)
		{
			int* v = current->vertices;
			int k = base_number(s2, v, size);
			if (k != -1) {
				matrix[k][col] = 1;
			}
			current = current->next;
			col++;
		}

		return matrix;
	}

	// n è il grado del complesso
	int beta_i_j(M_P* mp, double i, double j, int h, int n) {
		SimplicialComplex* sc1 = NULL, * sc2 = NULL;
		M_P* app = mp;

		if (i < app->l_min)
			return 0;
		if(h > n)
			return 0;

		while (app && !sc2) {
			if (app->l_min <= i && i <= app->l_max) {
				sc1 = app->sc;
			}
				
			if (app->l_min <= j && j <= app->l_max) {
				sc2 = app->sc;
				
			}
			app = app->next;
		}

		if (!sc2)
			return 0;

		printf("i = %lf, j = %lf\n", i, j);

		if (sc1[h].size == 0)
			return 0;

		//printf("sc1[h].size = %d\n", sc1[h].size);

		int** phi_i_j = matrix_phi_l1_to_l2(sc1[h].simplices, sc1[h].size, sc2[h].simplices, sc2[h].size, h + 1);
		//print_matrix(phi_i_j, sc2[h].size, sc1[h].size);

		int** edge1 = NULL;
		int** rank_edge1 = NULL;
		int colonne_base_rango = 0;
		if (h == 0) {
			//edge1 = input_id(edge1, sc1[h].size);
			edge1 = input_null(edge1, sc1[h].size, sc1[h].size);
			//print_matrix(edge1, sc1[h].size, sc1[h].size);
			rank_edge1 = input_id(rank_edge1, sc1[h].size);
			colonne_base_rango = sc1[h].size;
		}
		else {
			edge1 = edge_Matrix(sc1, h);
			rank_edge1 = rank_base(edge1, sc1[h].size, sc1[h].size, &colonne_base_rango);
		}

		if (!rank_edge1)
			return 0;

		int** matrix_j = NULL;
		
		matrix_j = mul_matrix(phi_i_j, sc2[h].size, sc1[h].size, rank_edge1, sc1[h].size, colonne_base_rango);
		
		int** edge2 = NULL;
		int rank2 = 0, row = 0, col = 0;
		
		if (h == n)
		{
			edge2 = input_null(edge2, sc2[h].size, sc2[h].size);
			row = sc2[h].size;
			col = sc2[h].size;
			rank2 = 0;
		}
		else if (!sc2[h + 1].size || sc2[h + 1].size == 0) {
			
			edge2 = input_null(edge2, sc2[h].size, sc2[h].size);
			row = sc2[h].size;
			col = sc2[h].size;
			//rank2 = rank_matrix(edge2, sc2[h].size, sc2[h + 1].size + 1);
			rank2 = 0;
			
		}
		else {
			edge2 = edge_Matrix(sc2, h + 1);
			rank2 = rank_matrix(edge2, sc2[h].size, sc2[h + 1].size);
			row = sc2[h].size;
			col = sc2[h + 1].size;
		}
		

		int** matrix_f = NULL;
		matrix_f = link2matrix_same_row(matrix_j, sc2[h].size, colonne_base_rango, edge2, row, col);
		
		

		int rank1 = rank_matrix(matrix_f, sc2[h].size, col + colonne_base_rango);
		
		

		int b = rank1 - rank2;

		return b;

	}

	void print1(M_P* a) {
		M_P* mp = a;
		while (mp) {
			printComplex(mp->sc, 3);
			mp = mp->next;
		}
	}
	
	//k è il grado del complesso
	int** beta_matrix(int min, int max, double passo, M_P* mp, int h, int k) {
		int** matrix = NULL;
		int n = ((max - min) / passo) + 1;
		matrix = input_null(matrix, n + 1, n + 1);
		int i = 0;
		int j = 0;
		for (i = 0; i <= n; i++) {
			for (j = i; j <= n; j++) {
				matrix[i][j] = beta_i_j(mp, min + i * passo, min + j * passo, h, k);
			}
		}
		return matrix;
	}
	
	int** mu_matrix(int** beta, int n) {
		int i = 0, j = 0;
		int** matrix = NULL;
		matrix = input_null(matrix, n , n);
		for (i = 1; i < n; i++)
			for (j = i + 1; j < n; j++)
				matrix[i][j] = beta[i][j - 1] - beta[i][j] + beta[i - 1][j] - beta[i - 1][j - 1];
		return matrix;
	}

	double** distance_matrix(int** point, int n) {
		int i = 0, j = 0;
		double** matrix = NULL;
		matrix = calloc(n, sizeof(double*));
		for (i = 0; i < n; i++) {
			matrix[i] = calloc(n, sizeof(double));
		}
		for (i = 0; i < n; i++) {
			for (j = i; j < n; j++) {
				matrix[i][j] = sqrt(pow(point[i][0] - point[j][0], 2) + pow(point[i][1] - point[j][1], 2));
				matrix[j][i] = matrix[i][j];
			}
		}
		return matrix;
	}

	int** input_point(int *k) {
		int** matrix = NULL;
		int n = 0;
		printf("Inserisci il numero di punti: n = ");
		scanf("%d", &n);
		*k = n;
		matrix = input_null(matrix, n, 2);
		int i = 0;
		for (i = 0; i < n; i++) {
			printf("Inserisci le coordinate del punto %d: ", i + 1);
			scanf("%d %d", &matrix[i][0], &matrix[i][1]);
		}
					
	return matrix;
	}

	int** zero_one_matrix(double** matrix, int n, double lambda) {
		int** m = NULL;
		m = input_null(m, n, n);
		int i = 0, j = 0;
		for (i = 0; i < n; i++) {
			for (j = i; j < n; j++) {
				if (matrix[i][j] < lambda) {
					m[i][j] = 1;
					m[j][i] = 1;
				}
				else
				{
					m[i][j] = 0;
					m[j][i] = 0;
				}
			}
		}

		return m;
	}

	double* find_lambda_value(double** matrix, int n) {
		int size = n * (n - 1) / 2;
		double* lambda = (double*)calloc(size, sizeof(double));
		int i = 0, j = 0, k = 0;
		
		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++) {
				lambda[k] = matrix[i][j];
				k++;
			}

		double l = 0;
		double app = 0;
		int iter = 0;
			
		qsort(lambda, size, sizeof(double), compare_double);
		

		while (l < lambda[size - 1] || iter < size - 1) {
			l += 0.5;
				
			if (lambda[iter] == app) {
				lambda[iter] = 0;
				iter++;
				l -= 0.5;
				continue;
			}
			else if(l > lambda[iter])
			{
				app = lambda[iter];
				lambda[iter] = l;
				iter++;
			}				
		}

		return lambda;
	}

	matrici_persistenza* create_matrix_persistenza(double** matrix, int n, double* l) {
		matrici_persistenza* mp = NULL, * app = NULL, * new = NULL;
		int size = (n*(n-1)/2);
		double* lambda = find_lambda_value(matrix, n);
		printf("Lambda\n");
		for (int i = 0; i < size; i++)
			printf("%lf ", lambda[i]);

		int i = 0;
		while (lambda[i] == 0)
			i++;
		mp = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
		mp->l_min = 0.5;
		mp->l_max = lambda[i]-0.5;
		mp->size = n;
		mp->matrix_d = zero_one_matrix(matrix, n, mp->l_max);
		printf("l_min = %lf\n", mp->l_min);
		printf("mp->l_max = %lf\n", mp->l_max);

		mp->prev = NULL;
		mp->next = NULL;
		app = mp;
		i++;
		printf("i = %d\n", i);
		while (i < size)
		{
			new = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
			new->l_min = mp->l_max + 0.5;
			while (lambda[i] == 0)
			{
				if(i < size)
					i++;
				else
					break;
			}
			
			new->l_max = lambda[i] - 0.5;
			new->size = n;
			new->matrix_d = zero_one_matrix(matrix, n, new->l_max);
			
			new->prev = mp;
			new->next = NULL;
			mp->next = new;
			mp = new;
			i++;
			printf("l_min = %lf\n", mp->l_min);
			printf("mp->l_max = %lf\n", mp->l_max);
		}

		new = (matrici_persistenza*)malloc(sizeof(matrici_persistenza));
		new->l_min = mp->l_max + 0.5;
		new->l_max = new->l_min;
		new->size = n;
		new->matrix_d = zero_one_matrix(matrix, n, new->l_max);
		
		new->prev = mp;
		new->next = NULL;
		mp->next = new;
		mp = new;
		*l = mp->l_max;
		printf("l_min = %lf\n", mp->l_min);
		printf("l_max = %lf\n", *l);
		return app;
	}

#endif