#include <math.h> // for sqrt  use -lm if use gcc to compile
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

	if (count > 10000) 
		{ *pansx = x; *pansy = y; return; }

	solveQuad(a1,b1,c1,d1,e1,f1,
	          a2,b2,c2,d2,e2,f2,
	          threshold,x,y,
	          count + 1, pansx, pansy);
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
	 *          abc -> (1,0,0)
	 *          pqr -> (0,1,0)
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
	double Ks[6] = {-ku-ku, -kv-kv, ku+ku, kv+kv, -ku-ku, -kv-kv};
	double Cs[6] = { u3+u2-u0-u0,   v3+v2-v0-v0,
	                 u2-u1,         v2-v1,
	                -u3+u1,        -v3+v1
	              };
	double Rs[6] = { u3-u2,         v3-v2,
	                -u2-u1+u0+u0,  -v2-v1+v0+v0,
	                -u3+u1,        -v3+v1
	              };
	double Zs[6] = {-u3+u2,        -v3+v2,
	                u2-u1,         v2-v1,
	                u3+u1-u0-u0,   v3+v1-v0-v0,
	              };
	double ac,ar,az,bc,br,bz,xc,xr,xz;
	double pc,pr,pz,qc,qr,qz,yc,yr,yz;

	ac = -Cs[0]/Ks[0];	ar = -Rs[0]/Ks[0];	az = -Zs[0]/Ks[0];
	bc = -Cs[1]/Ks[1];	br = -Rs[1]/Ks[1];	bz = -Zs[1]/Ks[1];
	pc = -Cs[2]/Ks[2];	pr = -Rs[2]/Ks[2];	pz = -Zs[2]/Ks[2];
	qc = -Cs[3]/Ks[3];	qr = -Rs[3]/Ks[3];	qz = -Zs[3]/Ks[3];
	xc = -Cs[4]/Ks[4];	xr = -Rs[4]/Ks[4];	xz = -Zs[4]/Ks[4];
	yc = -Cs[5]/Ks[5];	yr = -Rs[5]/Ks[5];	yz = -Zs[5]/Ks[5];

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
	solveQuad(a1, b1, c1, d1, d1, f1,
	          a2, b2, c2, d2, d2, f2,
			  0.00000001, c0, r0,
			  0, &cz, &rz
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

void solveOrientation(double abc[], double pqr[], double orientation[])
{
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
}
