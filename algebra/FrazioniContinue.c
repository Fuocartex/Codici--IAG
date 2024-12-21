#include "../Include/Create_Factor.h"

int main()
{
	int num = 0, n = 0, i= 0;
	printf("Inserire il numero di cui si vuole approssimare la radice x = ");
	scanf("%d", &num);
	printf("inseriro il numero di approssimazioni deisderate n = ");
	scanf("%d", &n);
	
	int* a = NULL;
	a = (int*)calloc(n, sizeof(int));
	if(!a)
	{
		printf("Errore di allocazione della memoria");
		return 1;
	}
	
	printf("Metodo Teorico\n");
	a = cont_frac_th(num, n);
	
	while(i < n){
		printf(" a[%d] = %d \n", i, a[i]);
		i++;
	}

	i = 0;
	
	printf("Metodo Pratico\n");
	a = cont_frac_impl(num, n);
	while(i < n){
		printf(" a[%d] = %d \n", i, a[i]);
		i++;
	}
	
	return 0;
}
