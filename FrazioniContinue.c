#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	int num = 0;
	long double n = 0, x = 0, y = 0, z = 0;
	printf("num =");
	scanf("%d", &num);
	printf("n =");
	scanf("%LF", &n);
	x = sqrtl(num);

	printf("Metodo Teorico\n");
	int a = 0, i = 0, az = floorl(x);
	while (i < n)
	{
		a = floorl(x);
		y = x - a;
		z = 1 / y;
		x = z;
		printf(" a[%d] = %d \n", i, a);
		i++;
	}

	printf("Metodo Pratico\n");
	i = 0;
	int beta = -az, gamma = 1, app = 0;
	a = az;

	while (i < n)
	{
		printf(" a[%d] = %d \n", i, a);
		app = (num - beta * beta) / gamma;
		gamma = app;
		app = 0;
		a = floorl((az - beta) / gamma);
		app = -(a * gamma + beta);
		beta = app;
		app = 0;
		i++;
	}
	return 0;
}