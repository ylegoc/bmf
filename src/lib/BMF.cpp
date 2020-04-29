#include "BMF.h"
#include <iostream>

using namespace std;

namespace bmf {

BMF::BMF() {}

// Lat = y
// Lng = x
double BMF::getGeodesicDistance(const Point & a, const Point & b) const {

	double dLat = b.y - a.y;
	double dLon = b.x - a.x;

	double l = sin(dLat / 2.0) * sin(dLat / 2.0) + cos(a.y) * cos(b.y) * sin(dLon / 2.0) * sin(dLon / 2.0);
	double c = 2 * asin(min(1.0, sqrt(l)));
	double d = R * c;

	return d;
}

}
