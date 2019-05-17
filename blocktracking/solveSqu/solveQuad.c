#include <math.h> // for sqrt
void solveQuad(double a1, double b1, double c1, 
               double d1, double e1, double f1,
               double a2, double b2, double c2,
               double d2, double e2, double f2,
               double threshold, double x0, double y0,
               int count, double* pansx, double* pansy)
{
	// solve equations:
	//   a1x^2 + b1y^2 + c1xy + d1x + e1y + f1 = 0
	//   a2x^2 + b2y^2 + c2xy + d2x + e2y + f2 = 0
	//   try starts from x0,y0,(0,0) default, until err < standard
	//   if count > XXXX (specified in line 50), considered no solution
	double x = x0;
	double y = y0;

	double F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
	double F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;
	if ((F1 >= -threshold) && (F1 <= threshold) &&
	    (F2 >= -threshold) && (F2 <= threshold)    )
	{
		*pansx = x; *pansy = y; return;
	}

	// partical derivative
	// climb up direction of F1 should be (F1_x,F1_y)
	// climb up direction of F2 should be (F2_x,F2_y)
	double F1_x = 2*a1*x+c1*y+d1;
	double F1_y = 2*b1*y+c1*x+e1;
	double F2_x = 2*a2*x+c2*y+d2;
	double F2_y = 2*b2*y+c2*x+e2;

	double F1_L = sqrt(F1_x*F1_x + F1_y*F1_y);
	double F2_L = sqrt(F2_x*F2_x + F2_y*F2_y);

	double F1_x_nor = F1_x / F1_L;
	double F1_y_nor = F1_y / F1_L;
	double F1_del = F1_x_nor * F1_x + F1_y_nor * F1_y;

	double F2_x_nor = F2_x / F2_L;
	double F2_y_nor = F2_y / F2_L;
	double F2_del = F2_x_nor * F2_x + F2_y_nor * F2_y;

	// for F1, (x0,y0 go (x_nor,y_nor) * err_F1)
	// for F2, (x0,y0 go (x_nor,y_nor) * err_F2)

	x += (0-F1)/F1_del * F1_x_nor + (0-F2)/F2_del * F2_x_nor;
	y += (0-F1)/F1_del * F1_y_nor + (0-F2)/F2_del * F2_y_nor;

	if (count > 100000) 
		{ *pansx = x; *pansy = y; return; }

	solveQuad(a1,b1,c1,d1,e1,f1,
	          a2,b2,c2,d2,e2,f2,
	          threshold,x,y,
	          count + 1, pansx, pansy);
}

void solveSquare(double points[], double camera[])
{
	double ku, kv, u0, v0;
	ku = camera[0];
	kv = camera[1];
	u0 = camera[2];
	v0 = camera[3];

	double u1, v1, u2, v2, u3, v3, u4, v4;
	u1 = points[0];
	v1 = points[1];
	u2 = points[2];
	v2 = points[3];
	u3 = points[4];
	v3 = points[5];
	u4 = points[6];
	v4 = points[7];

	// solve linar equation
	double c0 = -(u1*v2 - u2*v1 - u1*v3 + u3*v1 + u2*v4 - u4*v2 - u3*v4 + u4*v3)/
		    (u1*v2 - u2*v1 - u1*v4 + u2*v3 - u3*v2 + u4*v1 + u3*v4 - u4*v3)
	double r0 = -(	u1*v3 - u3*v1 - u1*v4 - u2*v3 + u3*v2 + u4*v1 + u2*v4 - u4*v2)/
		    (	u1*v2 - u2*v1 - u1*v4 + u2*v3 - u3*v2 + u4*v1 + u3*v4 - u4*v3)
		// got a c/z and r/z proximately, for quadric solving later as a starting point
	

}

