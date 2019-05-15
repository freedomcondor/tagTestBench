#include <stdio.h>
#include <cstddef>
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix3.h"


int main()
{
	printf("------------------------------\n");
	printf("       Vector3                \n");
	printf("------------------------------\n");

	Vector3 a(2,2,2);
	a.set(0,1,2);
	//Vector3 b(&a);
	Vector3 b(a);

	//b.set(0,1,2);
	printf("-------------- += test --------------\n");
	printf("a = %s\n",a.toStr());
	printf("b = %s\n",b.toStr());

	printf("a += b = %s\n",(a += b).toStr());
	printf("a -= b = %s\n",(a -= b).toStr());

	b.set(a);
	printf("a = %s\n",a.toStr());
	printf("b = %s\n",b.toStr());

	printf("-------------- = test --------------\n");
	Vector3 c;
	c = a;
	printf("a = %s\n",a.toStr());
	printf("c = %s\n",c.toStr());
	a.set(0,0,0);
	printf("a = %s\n",a.toStr());
	printf("c = %s\n",c.toStr());

	printf("-------------- + - test --------------\n");
	a.set(1,1,3);
	printf("a = %s\n",a.toStr());
	printf("-a = %s\n",(-a).toStr());
	printf("b = %s\n",b.toStr());
	printf("a+b = %s\n",(a+b).toStr());
	c = a + b;
	printf("c = a+b = %s\n",(c).toStr());
	c = a + Vector3(2,2,2);
	printf("c = a+(2,2,2) = %s\n",(c).toStr());

	printf("-------------- *= /= test --------------\n");
	printf("a = %s\n",a.toStr());
	printf("a*3 = %s\n",(a*=3).toStr());
	printf("a = %s\n",a.toStr());
	printf("a/2 = %s\n",(a/=2).toStr());
	printf("a = %s\n",a.toStr());
	printf("a/0 = %s\n",(a/=0).toStr());
	printf("a = %s\n",a.toStr());

	printf("-------------- * / test --------------\n");
	a.set(1,2,3);
	printf("a = %s\n",a.toStr());
	printf("a*3 = %s\n",(a*3).toStr());
	printf("3*a = %s\n",(3*a).toStr());
	c = a * 3;
	printf("c = %s\n",c.toStr());
	c = 3 * a;
	printf("c = %s\n",c.toStr());
	c = a / 3;
	printf("c = %s\n",c.toStr());
	printf("a*b = %s\n",(a*b).toStr());

	printf("-------------- ^ test --------------\n");
	printf("a = %s\n",a.toStr());
	double cc_d;
	cc_d = a ^ a;
	printf("a^a = %lf\n",a^a);
	printf("cc = %lf\n",cc_d);
	printf("a^2 = %lf\n",a^2);
	printf("a^3 = %lf\n",a^3);

	printf("-------------- == test --------------\n");
	printf("a = %s\n",a.toStr());
	printf("b = %s\n",b.toStr());
	printf("a == b %d\n",a == b);
	if (a == b) printf("true\n"); else printf("false\n");
	b = a;
	printf("b = %s\n",b.toStr());
	printf("a == b %d\n",a == b);
	if (a == b) printf("true\n"); else printf("false\n");

	printf("-------------- len/nor test --------------\n");
	printf("a = %s\n",a.toStr());
	printf("a.len = %lf\n",a.len());
	c = a.nor();
	printf("c = %s\n",c.toStr());
	printf("c = %lf\n",c.len());
	a.makenor();
	printf("a = %s\n",a.toStr());
	printf("a.len = %lf\n",a.len());


	printf("------------------------------\n");
	printf("       Quaternion             \n");
	printf("------------------------------\n");

	Quaternion aa(1,2,3,4);
	Quaternion bb(aa);
	Quaternion cc(a,5);
	printf("aa = %s\n",aa.toStr());
	printf("bb = %s\n",bb.toStr());
	printf("cc = %s\n",cc.toStr());

	printf("-------------- + - test --------------\n");
	printf("aa = %s\n",aa.toStr());
	printf("bb = %s\n",bb.toStr());
	aa += bb;
	printf("aa += bb = %s\n",aa.toStr());
	bb += Quaternion(3,2,1,0);
	printf("bb += (3,2,1,0) = %s\n",bb.toStr());

	printf("aa = %s\n",aa.toStr());
	printf("bb = %s\n",bb.toStr());
	printf("-aa = %s\n",(-aa).toStr());
	cc = -aa;
	printf("cc = -aa = %s\n",cc.toStr());
	cc = aa - bb;
	printf("cc = aa-bb = %s\n",cc.toStr());
	cc = aa + bb;
	printf("cc = aa+bb = %s\n",cc.toStr());

	printf("-------------- *= /= test --------------\n");
	printf("aa = %s\n",aa.toStr());
	aa *= 3;
	printf("aa*=3 = %s\n",aa.toStr());
	aa /= 4;
	printf("aa/=4 = %s\n",aa.toStr());
	aa /= 0;
	printf("aa/=0 = %s\n",aa.toStr());
	
	printf("-------------- * / test --------------\n");
	aa.set(1,2,3,4);
	printf("aa = %s\n",aa.toStr());
	printf("bb = %s\n",bb.toStr());
	cc = aa * bb;
	printf("cc = aa*bb = %s\n",cc.toStr());
	cc = aa / 3;
	printf("cc = aa/3 = %s\n",cc.toStr());
	cc = aa * 3;
	printf("cc = aa*3 = %s\n",cc.toStr());


	printf("-------------- len & inv test --------------\n");
	printf("aa = %s\n",aa.toStr());
	printf("aa.len() = %lf\n",aa.len());
	cc = aa.inv();
	printf("cc = aa.inv() = %s\n",cc.toStr());

	printf("-------------- rotate test --------------\n");
	aa.setFromRotation(Vector3(0,0,1),3.1415926);
	a.set(1,0,0);
	printf("a = %s\n",a.toStr());
	printf("aa = %s\n",aa.toStr());
	c = aa.toRotate(a);
	printf("c = %s\n",c.toStr());

	aa.setFromRotation(Vector3(0,0,1),3.1415926/2);
	c = aa.toRotate(a);
	printf("c = %s\n",c.toStr());

	printf("-------------- create from 4Vecs test --------------\n");
	aa.setFrom4Vecs(	Vector3(1,0,0),Vector3(0,0,1),
						Vector3(1,1,0),Vector3(0,0,1)	);
	a.set(1,0,0);
	printf("a = %s\n",a.toStr());
	printf("aa = %s\n",aa.toStr());
	c = aa.toRotate(a);
	printf("c = %s\n",c.toStr());

#ifdef AAA
	printf("AAA\n");
#else
	printf("BBB\n");
#endif
	
	printf("------------------------------\n");
	printf("       Matrix3                \n");
	printf("------------------------------\n");

	Matrix3 mata;
	printf("mata = %s\n", mata.toStr());

	return 0;
}
