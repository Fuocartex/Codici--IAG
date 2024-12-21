#include "Compl_Simpl.h"

int main(){
	
    	int size = 0;
	printf("Inserisci la grandezza del simplesso piu' grande nel complesso: ");
    	scanf("%d", &size);
    	
    	SimplicialComplex* complex = readComplex(size);
    	
    	if(!isSimplicial(complex, size)){
    		printf("Errore il simplesso non è simpliciale inserire un nuovo simplesso\n");
    		return 1;
	}
	
    	printComplex(complex, size);
	return 0;
}


