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
	int beta_i_j(M_P*, double, double, int);

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

	int beta_i_j(M_P* mp, double i, double j, int h) {
		SimplicialComplex* sc1 = NULL, * sc2 = NULL;
		M_P* app = mp;

		if (i < app->l_min)
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

		if (!sc1[h].size || sc1[h].size == 0)
			return 0;

		int** phi_i_j = matrix_phi_l1_to_l2(sc1[h].simplices, sc1[h].size, sc2[h].simplices, sc2[h].size, h + 1);
		//print_matrix(phi_i_j, sc2[h].size, sc1[h].size);
		int** edge1 = NULL;
		int** rank_edge1 = NULL;

		if (h == 0) {
			//edge1 = input_id(edge1, sc1[h].size);
			edge1 = input_null(edge1, sc1[h].size, sc1[h].size);
			//print_matrix(edge1, sc1[h].size, sc1[h].size);
			rank_edge1 = rank_base(edge1, sc1[h].size, sc1[h].size);
		}
		else {
			edge1 = edge_Matrix(sc1, h);
			rank_edge1 = rank_base(edge1, sc1[h].size, sc1[h].size);
		}

		int** matrix_j = NULL;
		matrix_j = mul_matrix(phi_i_j, sc2[h].size, sc1[h].size, rank_edge1, sc1[h].size, sc1[h].size);


		int** edge2 = NULL;
		
		if (!sc2[h + 1].size || sc2[h + 1].size == 0) {
			
			edge2 = input_null(edge2, sc2[h].size, sc2[h + 1].size + 1);
			
		}
		else {
			edge2 = edge_Matrix(sc2, h + 1);
		}

		int** matrix_f = NULL;
		matrix_f = link2matrix_same_row(matrix_j, sc2[h].size, sc1[h].size, edge2, sc2[h].size, sc2[h + 1].size);
			
		//print_matrix(matrix_f, sc2[h].size, sc2[h + 1].size + sc1[h].size);

		int rank1 = rank_matrix(matrix_f, sc2[h].size, sc2[h + 1].size + sc1[h].size);
		
		int rank2 = rank_matrix(edge2, sc2[h].size, sc2[h + 1].size + 1);

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
	
	
	
#endif