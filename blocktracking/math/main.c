#include <stdio.h>
#include "solveQuad.c"

int main()
{
	double x, y;
	solveQuad(
		1, 2, 1, 1, 1, -1,
		2, 1, 0.1, 0.5, 2, -1,
		0.0001, 1, 1,
		0, &x, &y
	);

	printf("%lf, %lf\n", x, y);
	return 0;
}
