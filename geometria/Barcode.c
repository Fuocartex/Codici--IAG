#include "..\Include\Barcode.h"

int main()
{
	
	matrici_persistenza* mp = NULL;
	mp = create_M_P();
	M_P* mod_p = create_Modulo_Persistenza(mp);
	//print1(mod_p);

	int h = beta_i_j(mod_p, 1, 1, 0);

	printf("beta_i_j = %d\n", h);
	
	return 0;
}