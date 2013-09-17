#include <iostream>
#include "delegate.h"

using namespace std;


class C
{
public:
	int v;
};

class A
{
public:
	float funcRVoid(void)
	{	
		return 10;
	}
	void funcVoidVoid(void)
	{	
	}
	float funcR1(float f)
	{
		return f;
	}
	void funcVoid1(float f)
	{
		
	}
	float funcR2(int d1, float f)
	{
		return d1+f;
	}
	void funcVoid2(int d1, float f)
	{
		
	}
	float funcR3(int d1, int d2, float f)
	{
		return d1+d2+f;
	}
	void funcVoid3(int d1, int d2, float f)
	{
		
	}
	float funcR4(int d1, int d2, int d3, float f)
	{
		return d1+d2+d3+f;
	}
	void funcVoid4(int d1, int d2, int d3, float f)
	{
		
	}
	float funcR5(int d1, int d2, int d3, int d4, float f)
	{
		return d1+d2+d3+d4+f;
	}
	void funcVoid5(int d1, int d2, int d3, int d4, float f)
	{
		
	}
	float funcR6(int d1, int d2, int d3, int d4, int d5, float f)
	{
		return d1+d2+d3+d4+d5+f;
	}
	void funcVoid6(int d1, int d2, int d3, int d4, int d5, float f)
	{
		
	}
	float funcR7(int d1, int d2, int d3, int d4, int d5, int d6, float f)
	{
		return d1+d2+d3+d4+d5+d6+f;
	}
	void funcVoid7(int d1, int d2, int d3, int d4, int d5, int d6, float f)
	{
		
	}
	float funcR8(int d1, int d2, int d3, int d4, int d5, int d6, int d7, float f)
	{
		return d1+d2+d3+d4+d5+d6+d7+f;
	}
	void funcVoid8(int d1, int d2, int d3, int d4, int d5, int d6, int d7, float f)
	{
		
	}
	float funcR9(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, float f)
	{
		return d1+d2+d3+d4+d5+d6+d7+d8+f;
	}
	void funcVoid9(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, float f)
	{
		
	}
	float funcR10(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, int d9, float f)
	{
		return d1+d2+d3+d4+d5+d6+d7+d8+d9+f;
	}
	void funcVoid10(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, int d9, float f)
	{
		
	}
	C* func_c_1C(C& c)
	{
		C* nc = new C;
		nc->v = c.v;
		return nc;
	}
	void func_Void_1C(C& sc)
	{
		C* nc = new C;
		nc->v = sc.v;
	}
};

bool test(int fn)
{
	A a,a1;
	ICallObject* b,*b1;
	float res;
	C sc;
	sc.v = 199;
	C* cres;
	

	switch (fn)
	{
		case 1:
			b = createCallObject(&A::funcRVoid, &a);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 10);
		case 2:
			b = createCallObject(&A::funcVoidVoid, &a);
			b->call();
			delete b;
			return true;
		case 3:
			b = createCallObject(&A::funcR1, &a, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 19);
		case 4:
			b = createCallObject(&A::funcVoid1, &a, (float)19);
			b->call();
			delete b;
			return true;
		case 5:
			b = createCallObject(&A::funcR2, &a, 10, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 29);
		case 6:
			b = createCallObject(&A::funcVoid2, &a, 10, (float)19);
			b->call();
			delete b;
			return true;
		case 7:
			b = createCallObject(&A::funcR3, &a, 10, 11, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 40);
		case 8:
			b = createCallObject(&A::funcVoid3, &a, 10, 11, (float)19);
			b->call();
			delete b;
			return true;
		case 9:
			b = createCallObject(&A::funcR4, &a, 10, 11, 12, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 52);
		case 10:
			b = createCallObject(&A::funcVoid4, &a, 10, 11, 12, (float)19);
			b->call();
			delete b;
			return true;
		case 11:
			b = createCallObject(&A::funcR5, &a, 10, 11, 12, 13, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 65);
		case 12:
			b = createCallObject(&A::funcVoid5, &a, 10, 11, 12, 13, (float)19);
			b->call();
			delete b;
			return true;
		case 13:
			b = createCallObject(&A::funcR6, &a, 10, 11, 12, 13, 14, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 79);
		case 14:
			b = createCallObject(&A::funcVoid6, &a, 10, 11, 12, 13, 14, (float)19);
			b->call();
			delete b;
			return true;
		case 15:
			b = createCallObject(&A::funcR7, &a, 10, 11, 12, 13, 14, 15, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 94);
		case 16:
			b = createCallObject(&A::funcVoid7, &a, 10, 11, 12, 13, 14, 15, (float)19);
			b->call();
			delete b;
			return true;
		case 17:
			b = createCallObject(&A::funcR8, &a, 10, 11, 12, 13, 14, 15, 16, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 110);
		case 18:
			b = createCallObject(&A::funcVoid8, &a, 10, 11, 12, 13, 14, 15, 16, (float)19);
			b->call();
			delete b;
			return true;
		case 19:
			b = createCallObject(&A::funcR9, &a, 10, 11, 12, 13, 14, 15, 16, 17, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 127);
		case 20:
			b = createCallObject(&A::funcVoid9, &a, 10, 11, 12, 13, 14, 15, 16, 17, (float)19);
			b->call();
			delete b;
			return true;
		case 21:
			b = createCallObject(&A::funcR10, &a, 10, 11, 12, 13, 14, 15, 16, 17, 18, (float)19);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				res = b->getResult();
			#else
				res = *(static_cast<float*>(b->getResult()));
			#endif
			delete b;
			return (res == 145);
		case 22:
			b = createCallObject(&A::funcVoid10, &a, 10, 11, 12, 13, 14, 15, 16, 17, 18, (float)19);
			b->call();
			delete b;
			return true;
		/*case 30:
			b = createCallObject(&A::func_c_1C, &a, c);
			b->call();
			#ifdef ICALLOBJECT_VARIANT_RESULT
				cres = b->getResult();
			#else
				cres = *(static_cast<C*>(b->getResult()));
			#endif
			delete b;
			return (cres->v == c.v);
		case 31:
			b = createCallObject(&A::func_Void_1C, &a, sc);
			b->call();
			delete b;
			return true;*/
		case 32:
			b = createCallObject(&A::funcVoid10, &a, 10, 11, 12, 13, 14, 15, 16, 17, 18, (float)19);
			b->call();
			b->setReceiver(&a1);
			b->call();
			delete b;
			return true;
		case 33:
			b = createCallObject(&A::funcVoid10, &a, 10, 11, 12, 13, 14, 15, 16, 17, 18, (float)19);
			b1 = b->clone();
			b1->call();
			delete b;
			delete b1;
			return true;
	}
}

int main()
{
	cout << "Test func_R_Void: " << test(1) << endl;
	cout << "Test func_Void_Void: " << test(2) << endl;
	cout << "Test func_R_1: " << test(3) << endl;
	cout << "Test func_void_1: " << test(4) << endl;
	cout << "Test func_R_2: " << test(5) << endl;
	cout << "Test func_void_2: " << test(6) << endl;
	cout << "Test func_R_3: " << test(7) << endl;
	cout << "Test func_void_3: " << test(8) << endl;
	cout << "Test func_R_4: " << test(9) << endl;
	cout << "Test func_void_4: " << test(10) << endl;
	cout << "Test func_R_5: " << test(11) << endl;
	cout << "Test func_void_5: " << test(12) << endl;
	cout << "Test func_R_6: " << test(13) << endl;
	cout << "Test func_void_6: " << test(14) << endl;
	cout << "Test func_R_7: " << test(15) << endl;
	cout << "Test func_void_7: " << test(16) << endl;
	cout << "Test func_R_8: " << test(17) << endl;
	cout << "Test func_void_8: " << test(18) << endl;
	cout << "Test func_R_9: " << test(19) << endl;
	cout << "Test func_void_9: " << test(20) << endl;
	cout << "Test func_R_10: " << test(21) << endl;
	cout << "Test func_void_10: " << test(22) << endl;
//	cout << "Test func_Void_1C: " << test(31) << endl;
	cout << "Test SetReceiver: " << test(32) << endl;
	cout << "Test Clone: " << test(33) << endl;

	return 0;
}


