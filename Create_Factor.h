#pragma one
#ifndef Create_Factor
#define Create_Factor

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int* cont_frac_th(int, int);
int* cont_frac_impl(int, int);

int* cont_frac_th(int r, int n)
{	
	int* a = NULL;
	long double x = 0, y = 0, z = 0;
	a = (int*)calloc(n, sizeof(int));
	if(!a)
	{
		printf("Errore di allocazione della memoria");
		return NULL;
	}
	
	x = sqrtl(r);

	int i = 0;
	while (i < n)
	{
		a[i] = floorl(x);
		y = x - a[i];
		z = 1 / y;
		x = z;
		i++;
	}
	return a;
}

int* cont_frac_impl(int r, int n)
{	
	int* a = NULL;
	a = (int*)calloc(n, sizeof(int));
	if(!a)
	{
		printf("Errore di allocazione della memoria");
		return NULL;
	}
	
	long double x = sqrtl(r);
	a[0] = floorl(x);
	int i = 1, beta = -a[0], gamma = 1, app = 0;

	while (i < n)
	{
		app = (r - beta * beta) / gamma;
		gamma = app;
		app = 0;
		a[i] = floorl((a[0] - beta) / gamma);
		app = -(a[i] * gamma + beta);
		beta = app;
		app = 0;
		i++;
	}
	return a;
}


#endif
