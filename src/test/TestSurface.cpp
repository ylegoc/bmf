#include <iostream>
#include "lib/BMF.h"

using namespace std;
using namespace bmf;

using namespace std;

BMF bmfAPI;
double x = 0.0;

void testSurface(double a, double b, double c, double d) {

	double s = bmfAPI.calculateMaxSurface(a, b, c, d, x, 10000);
	cout << "s = " << s << " @ x = " << x << endl;

	for (int i = 1; i < 10; ++i) {
		s = bmfAPI.calculateMaxSurfaceOpt(a, b, c, d, x, i);
		cout << i << " s = " << s << " @ x = " << x << endl;
	}
}

int main(int, char**) {

	cout << "Test 1" << endl;
	double a = 1;
	double b = 2;
	double c = 2;
	double d = sqrt(3);

	testSurface(a, b, c, d);

	cout << "Test 2" << endl;
	a = sqrt(2);
	b = 6;
	c = sqrt(3);
	d = 5;

	testSurface(a, b, c, d);

	return 0;
}
