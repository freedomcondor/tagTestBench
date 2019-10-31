#include <math.h> // for sqrt  use -lm if use gcc to compile
void solveQuad(double a1, double b1, double c1, double d1, double e1, double f1,
               double a2, double b2, double c2, double d2, double e2, double f2,
               double x0, double y0,
               double* pansx, double* pansy)
{
	double x = x0;
	double y = y0;
	double lastx = x;
	double lasty = y;
	double F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
	double F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;

	double F1x, F1y, F2x, F2y, Fx, Fy, L, dx, dy, dF, rate;

	double threshold = 0.0000000001;
	int count = 0;

	while (! ((F1 >= -threshold) && (F1 <= threshold) &&
	          (F2 >= -threshold) && (F2 <= threshold)    )
	      )
	{
		// find the next (x,y) based on gradient of F1 and F2
		double stepx = 0;
		double stepy = 0;
		// F1
		Fx = 2*a1*x+c1*y+d1;
		Fy = 2*b1*y+c1*x+e1;
		L = 1/sqrt(Fx*Fx + Fy*Fy);
		dx = Fx * L;
		dy = Fy * L;
		dF = Fx * dx + Fy * dy;
		rate = (0-F1) / dF;
		stepx += rate * dx;
		stepy += rate * dy;
		// F2
		Fx = 2*a2*x+c2*y+d2;
		Fy = 2*b2*y+c2*x+e2;
		L = 1/sqrt(Fx*Fx + Fy*Fy);
		dx = Fx * L;
		dy = Fy * L;
		dF = Fx * dx + Fy * dy;
		rate = (0-F2) / dF;
		stepx += rate * dx;
		stepy += rate * dy;
		// check and add to (x,y)
		if ((x + stepx == lastx) && (y + stepy == lasty)) break;
		lastx = x;
		lasty = y;
		x += stepx;
		y += stepy;
		F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
		F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;

		count++;
		if (count > 1000) break;
	}


	if ((F1 >= -threshold) && (F1 <= threshold) &&
	    (F2 >= -threshold) && (F2 <= threshold)    )
	{
		*pansx = x; *pansy = y; return;
	}

	// if not finish, then it means (x,y) goes into an oscillation, use F=F1^2 + F2^2 to approach
	threshold *= threshold;
	double F = F1*F1 + F2*F2;

	while (F > threshold)
	{
		// find the next (x,y) based on gradient of F = F1^2 + F^2
		F1x = 2*a1*x+c1*y+d1;
		F1y = 2*b1*y+c1*x+e1;

		F2x = 2*a2*x+c2*y+d2;
		F2y = 2*b2*y+c2*x+e2;

		Fx = 2*F1 * F1x + 2*F2 * F2x;
		Fy = 2*F1 * F1y + 2*F2 * F2y;

		L = 1/sqrt(Fx*Fx + Fy*Fy);
		dx = Fx * L;
		dy = Fy * L;
		dF = Fx * dx + Fy * dy;
		rate = (0-F) / dF;
		x += rate * dx;
		y += rate * dy;

		F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
		F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;
		F = F1*F1 + F2*F2;

		count++;
		if (count > 2000) break;
	}

	*pansx = x; *pansy = y;
}



void solveSquare(double points[], double camera[], double L,
                 double* p_x, double* p_y, double* p_z,
                 double* p_a, double* p_b, double* p_c,
                 double* p_p, double* p_q, double* p_r)
{
	/*              u                / z
	 *          ---------           /
	 *        v |       |          ------------- x
	 *          |       |          |
	 *          ---------          | y
	 *
	 *             abc
	 *          ---------
	 *          |   |   |       abc -> (0,1,0)
	 *          |   ----|pqr    pqr -> (1,0,0)
	 *          |  xyz  |
	 *          ---------
	 */

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
		    (u1*v2 - u2*v1 - u1*v4 + u2*v3 - u3*v2 + u4*v1 + u3*v4 - u4*v3);
	double r0 = -(	u1*v3 - u3*v1 - u1*v4 - u2*v3 + u3*v2 + u4*v1 + u2*v4 - u4*v2)/
		    (	u1*v2 - u2*v1 - u1*v4 + u2*v3 - u3*v2 + u4*v1 + u3*v4 - u4*v3);
		// got a c/z and r/z proximately, for quadric solving later as a starting point

	// a b p q x y c r z
	// 6 rows left
	//double Ks[6] = {-ku-ku, -kv-kv, ku+ku, kv+kv, -ku-ku, -kv-kv};
	double Ks[6] = {-0.5/ku,-0.5/kv,0.5/ku,0.5/kv,-0.5/ku,-0.5/kv};
	//double Cs[6] = { u3+u2-u0-u0,   v3+v2-v0-v0,
	double Cs[6] = { u3+u2-u0*2,v3+v2-v0*2,
	                 u2-u1,         v2-v1,
	                -u3+u1,        -v3+v1
	              };
	double Rs[6] = { u3-u2,         v3-v2,
	                //-u2-u1+u0+u0,  -v2-v1+v0+v0,
	                -u2-u1+u0*2,-v2-v1+v0*2,
	                -u3+u1,        -v3+v1
	              };
	double Zs[6] = {-u3+u2,        -v3+v2,
	                u2-u1,         v2-v1,
	               // u3+u1-u0-u0,   v3+v1-v0-v0,
	                u3+u1-u0*2,v3+v1-v0*2
	              };
	double ac,ar,az,bc,br,bz,xc,xr,xz;
	double pc,pr,pz,qc,qr,qz,yc,yr,yz;

	ac = -Cs[0]*Ks[0];	ar = -Rs[0]*Ks[0];	az = -Zs[0]*Ks[0];
	bc = -Cs[1]*Ks[1];	br = -Rs[1]*Ks[1];	bz = -Zs[1]*Ks[1];
	pc = -Cs[2]*Ks[2];	pr = -Rs[2]*Ks[2];	pz = -Zs[2]*Ks[2];
	qc = -Cs[3]*Ks[3];	qr = -Rs[3]*Ks[3];	qz = -Zs[3]*Ks[3];
	xc = -Cs[4]*Ks[4];	xr = -Rs[4]*Ks[4];	xz = -Zs[4]*Ks[4];
	yc = -Cs[5]*Ks[5];	yr = -Rs[5]*Ks[5];	yz = -Zs[5]*Ks[5];

	/* now we have x,y,a,b,p,q to c r z
	 * solve double quadric equation next
	 * we have two constrains:
	 * 		ap + bq + cr == 0 and
	 * 		a^2 + b^2 + c^2 == p^2 + q^2 + r^2 == hL^2
	 * expressed by c r z, we have two equations like:
	 * 		Ac^2 + Br^2 + Ccr + Dcz + Erz + Fz^2 == 0
	 * have c = cz * z,  r = rz * z, and eliminate z
	 * we have two:
	 * 		A1c^2 + B1r^2 + C1cr + D1c + E1r + F1 == 0
	 * 		A2c^2 + B2r^2 + C2cr + D2c + E2r + F2 == 0
	 */
	double a1, b1, c1, d1, e1, f1;
	double a2, b2, c2, d2, e2, f2;
	//c^2 ap            bq              cr
	a1 =  ac*pc         +bc*qc             ;
	//r^2
	b1 =  ar*pr         +br*qr             ;
	//cr
	c1 =  ac*pr+ar*pc   +bc*qr+br*qc    +1 ;
	//cz
	d1 =  ac*pz+az*pc   +bc*qz+bz*qc       ;
	//rz
	e1 =  ar*pz+az*pr   +br*qz+bz*qr       ;
	//z^2
	f1 =  az*pz         +bz*qz             ;

	double a3, b3, c3, d3, e3, f3;
	double a4, b4, c4, d4, e4, f4;
	//c^2 aa            bb              cc
	a3 = ac*ac          +bc*bc          +1 ;
	//r^2
	b3 = ar*ar          +br*br             ;
	//cr
	c3 = ac*ar+ar*ac    +bc*br+br*bc       ;
	//cz
	d3 = ac*az+az*ac    +bc*bz+bz*bc       ;
	//rz
	e3 = ar*az+az*ar    +br*bz+bz*br       ;
	//z^2
	f3 = az*az          +bz*bz             ;

	//c^2 pp            qq              rr
	a4 = pc*pc          +qc*qc             ;
	//r^2
	b4 = pr*pr          +qr*qr          +1 ;
	//cr
	c4 = pc*pr+pr*pc    +qc*qr+qr*qc       ;
	//cz
	d4 = pc*pz+pz*pc    +qc*qz+qz*qc       ;
	//rz
	e4 = pr*pz+pz*pr    +qr*qz+qz*qr       ;
	//z^2
	f4 = pz*pz          +qz*qz             ;

	a2 = a3-a4;
	b2 = b3-b4;
	c2 = c3-c4;
	d2 = d3-d4;
	e2 = e3-e4;
	f2 = f3-f4;

	double cz, rz;
	/*
	solveQuad_curve(a1, b1, c1, d1, e1, f1,
	          a2, b2, c2, d2, e2, f2,
			  0.0000000001, c0, r0,
			  1, &cz, &rz
			 );
	*/

	solveQuad(a1, b1, c1, d1, e1, f1,
	          a2, b2, c2, d2, e2, f2,
			  c0, r0,
			  &cz, &rz
			 );

	double a5,b5,c5,d5,e5,f5;
	//c^2
	a5 = a3 * cz * cz;
	//r^2
	b5 = b3 * rz * rz;
	//cr
	c5 = c3 * cz * rz;
	//cz
	d5 = d3 * cz;
	//rz
	e5 = e3 * rz;
	//z^2
	f5 = f3;

	double x,y,z,a,b,c,p,q,r;
	double hL = L / 2;
	z = sqrt(hL * hL / (a5+b5+c5+d5+e5+f5));
	c = cz * z;
	r = rz * z;
	x = xc * c + xr * r + xz * z;
	y = yc * c + yr * r + yz * z;
	a = ac * c + ar * r + az * z;
	b = bc * c + br * r + bz * z;
	p = pc * c + pr * r + pz * z;
	q = qc * c + qr * r + qz * z;

	// got x y z a b c p q r
	*p_x = x;
	*p_y = y;
	*p_z = z;
	*p_a = a;
	*p_b = b;
	*p_c = c;
	*p_p = p;
	*p_q = q;
	*p_r = r;
}

//-------------------------------------------------------------------------------------

double vectorLength(double A[])
{
	return sqrt(A[0]*A[0] + A[1]*A[1] + A[2]*A[2]);
}

void vectorNormalize(double A[])
{
	if (vectorLength(A) == 0) return;
	double L = 1/vectorLength(A);
	A[0] *= L;
	A[1] *= L;
	A[2] *= L;
}

double vectorDistance(double A[], double B[])
{
	return sqrt(
		(A[0] - B[0]) * (A[0] - B[0]) + 
		(A[1] - B[1]) * (A[1] - B[1]) + 
		(A[2] - B[2]) * (A[2] - B[2])
	);
}

double vectorDotProduct(double A[], double B[])
{
	return A[0]*B[0] + A[1]*B[1] + A[2]*B[2];
}

void vectorCrossProduct(double A[], double B[], double C[])
{
	C[0] = A[1] * B[2] - A[2] * B[1];
	C[1] = A[2] * B[0] - A[0] * B[2];
	C[2] = A[0] * B[1] - A[1] * B[0];
}

void rotationMatrix(double th, double axis[], double matrix[])
{
	double x = axis[0];
	double y = axis[1];
	double z = axis[2];

	matrix[0] = cos(th) + (1 - cos(th))*x*x;
	matrix[1] = (1 - cos(th))*x*y - sin(th)*z;
	matrix[2] = (1 - cos(th))*x*z + sin(th)*y;

	matrix[3] = (1 - cos(th))*y*x + sin(th)*z;
	matrix[4] = cos(th) + (1 - cos(th))*y*y;
	matrix[5] = (1 - cos(th))*y*z - sin(th)*x;

	matrix[6] = (1 - cos(th))*z*x - sin(th)*y;
	matrix[7] = (1 - cos(th))*z*y + sin(th)*x;
	matrix[8] = cos(th) + (1 - cos(th))*z*z;
}

void MatrixVector(double m[], double v[], double res[])
{
	res[0] = m[0]*v[0] + m[1]*v[1] + m[2]*v[2];
	res[1] = m[3]*v[0] + m[4]*v[1] + m[5]*v[2];
	res[2] = m[6]*v[0] + m[7]*v[1] + m[8]*v[2];
}

void MatrixMatrix(double m1[], double m2[], double m3[])
{
	m3[0] = m1[0]*m2[0] + m1[1]*m2[3] + m1[2]*m2[6];
	m3[1] = m1[0]*m2[1] + m1[1]*m2[4] + m1[2]*m2[7];
	m3[2] = m1[0]*m2[2] + m1[1]*m2[5] + m1[2]*m2[8];

	m3[3] = m1[3]*m2[0] + m1[4]*m2[3] + m1[5]*m2[6];
	m3[4] = m1[3]*m2[1] + m1[4]*m2[4] + m1[5]*m2[7];
	m3[5] = m1[3]*m2[2] + m1[4]*m2[5] + m1[5]*m2[8];

	m3[6] = m1[6]*m2[0] + m1[7]*m2[3] + m1[8]*m2[6];
	m3[7] = m1[6]*m2[1] + m1[7]*m2[4] + m1[8]*m2[7];
	m3[8] = m1[6]*m2[2] + m1[7]*m2[5] + m1[8]*m2[8];
}

void solveOrientation(double abc[], double pqr[], double orientation[])
{
	double unitX[3] = {1,0,0};
	double unitY[3] = {0,1,0};
	vectorNormalize(abc);
	vectorNormalize(pqr);

	double axis1[3];
	vectorCrossProduct(unitX, abc, axis1);
	vectorNormalize(axis1);
	double th1 = acos( vectorDotProduct(unitX,abc) );
	double matrix1[9];
	if (vectorLength(axis1) == 0)
		rotationMatrix(0, unitX, matrix1);
	else
		rotationMatrix(th1, axis1, matrix1);

	double pqr_m[3];
	MatrixVector(matrix1, unitY, pqr_m);
	double axis2[3];
	vectorCrossProduct(pqr_m, pqr, axis2);
	vectorNormalize(axis2);

	double th2 = acos( vectorDotProduct(pqr_m,pqr) );
	double matrix2[9];

	if (vectorDistance(axis2, abc) > 1) unitX[0] = -1;
	rotationMatrix(th2, unitX, matrix2);

	MatrixMatrix(matrix1, matrix2, orientation);
}

void solveTag(double points[], double camera[], double L, 
              double position[], double orientation[])
{
	double x, y, z, a, b, c, p, q, r;
	solveSquare(points, camera, L, 
	            &x, &y, &z, &a, &b, &c, &p, &q, &r);
	position[0] = x;
	position[1] = y;
	position[2] = z;

	double abc[3] = {a,b,c};
	double pqr[3] = {p,q,r};
	solveOrientation(pqr, abc, orientation);
}
