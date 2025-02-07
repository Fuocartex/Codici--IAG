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
	typedef struct Modulo_Persistenza{
		int l_min;
		int l_max;
		int** matrix_d;
		struct Modulo_Persistenza* next;
		struct Modulo_Persistenza* prev;
	} M_P;

	M_P* create_M_P(void);

	M_P* create_M_P(void) {
		M_P* mp1 = (M_P*)malloc(sizeof(M_P));
		M_P* mp2 = (M_P*)malloc(sizeof(M_P));
		M_P* mp3 = (M_P*)malloc(sizeof(M_P));
		mp1->l_min = 0.5;
		mp1->l_max = 1;
		int** m1 = input
		return mp;
	}
	
	
	
#endif