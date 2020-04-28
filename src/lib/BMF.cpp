#include "BMF.h"
#include <iostream>

using namespace std;

namespace bmf {

BMF::BMF(double centerLng, double centerLat) :
	m_center(toRadians(centerLng), toRadians(centerLat)) {
}

double BMF::getEuclideanDistance(const Point & a, const Point & b) const {

	double x = b.x - a.x;
	double y = b.y - a.y;

	return sqrt(x * x + y * y);
}

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

Point BMF::getPoint(double lng, double lat) const {

	Point point(toRadians(lng), toRadians(lat));

	Point px(m_center.x, point.y);
	Point py(point.x, m_center.y);

	double dx = getGeodesicDistance(px, m_center);
	double dy = getGeodesicDistance(py, m_center);

	return Point(dx, dy);
}

}
