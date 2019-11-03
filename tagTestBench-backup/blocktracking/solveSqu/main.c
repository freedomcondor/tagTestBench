#include <stdio.h>
#include "solveQuad.c"

int main()
{
	double x, y;

	solveQuad(
		1, 2, 1, 1, 1, -1,
		2, 1, 0.1, 0.5, 2, -1,
	    0, 0,
	    &x, &y
	);

	printf("%lf, %lf\n", x, y);

	double X[3] = {1,0,0};
	double Y[3] = {0,1,0};
	double Z[3] = {0,0,1};
	double rotationZ[9];
	double rotationY[9];
	rotationMatrix(3.1415926/2, Z, rotationZ);
	rotationMatrix(3.1415926/4, Y, rotationY);
	double v[3];
	double rotation[9];
	MatrixMatrix(rotationZ, rotationY, rotation);
	MatrixVector(rotation, X, v);
	printf("%lf, %lf, %lf\n", v[0], v[1], v[2]);

	vectorCrossProduct(X, Y, v);
	printf("%lf, %lf, %lf\n", v[0], v[1], v[2]);

	double abc[3] = {1,1,0};
	double pqr[3] = {-1,1,0};
	solveOrientation(abc, pqr, rotation);
	MatrixVector(rotation, Z, v);
	printf("%lf, %lf, %lf\n", v[0], v[1], v[2]);
	return 0;
}
