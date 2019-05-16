#include "stdio.h"
#include "solveQuad.c"

int main()
{
	double x, y;
	solveQuad(
		1, 2, 0, 0, 0, -1,
		2, 1, 0, 0, 0, -1,
		0.01, 0, 0.01,
		1, &x, &y
	);

	printf("%lf, %lf\n", x, y);
	return 0;
}
