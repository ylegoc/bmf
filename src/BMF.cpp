#include "BMF.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <iostream>

using namespace std;

namespace bmf {

BMF::BMF() : m_N(7) {}

void BMF::setNumberOfCalculations(int N) {
	m_N = N;
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


double BMF::calculateTriangleSurface(double a, double b, double c) const {

	double s = (a + b + c) / 2;
	double a2 = s * (s - a) * (s - b) * (s - c);

	if (a2 > 0.0) {
		return sqrt(a2);
	}

	return 0.0;
}


double BMF::calculateMaxSurface(double a, double b, double c, double d, double& x, int N) const {

	// Set the number of calculations.
	if (N == -1) {
		N = m_N;
	}

	// Calculate the min x.
	double m1 = abs(b - a);
	double m2 = abs(d - c);
	double xMin = max(m1, m2);

	// Calculate the max x.
	double xMax = min(a + b, c + d);

	double delta = (xMax - xMin) / (N - 1);

	double xCurrent = 0.0;
	double maxSurface = 0.0;

	// Discretize the points.
	for (int i = 0; i < N + 1; ++i) {

		// Update current x.
		xCurrent = xMin + delta * i;

		// Calculate a1 and a2.
		double a1 = calculateTriangleSurface(a, b, xCurrent);
		double a2 = calculateTriangleSurface(c, d, xCurrent);
		double surface = a1 + a2;

		//cout << "current a, b, c, d, x: " << a << ", " << b << ", " << c << ", " << d << ", " << xCurrent << " => " << a1 << ", " << a2 << endl;

		if (surface > maxSurface) {
			x = xCurrent;
			maxSurface = surface;
		}
	}

	return maxSurface;
}

double BMF::calculateMaxSurfaceOpt(double a, double b, double c, double d, double& x, int N) const {

	// Set the number of calculations.
	if (N == -1) {
		N = m_N;
	}

	// Calculate the min x.
	double m1 = abs(b - a);
	double m2 = abs(d - c);
	double xMin = max(m1, m2);

	// Calculate the max x.
	double xMax = min(a + b, c + d);
	double xCurrent = 0.0;
	double maxSurface = 0.0;

	// Calculate the maximum surface for triangle 1 and triangle 2.
	double x1 = sqrt(a * a + b * b);
	double x2 = sqrt(c * c + d * d);

	double xMinOpt = min(x1, x2);
	double xMaxOpt = max(x1, x2);

	xMin = max(xMin, xMinOpt);
	xMax = min(xMax, xMaxOpt);

	// The maximum x is in [x1, x2] (or [x2, x1] because functions of x for triangle are maximum when angle is PI/2
	// and functions are increasing then decreasing.

	double delta = (xMax - xMin) / (N - 1);

	// Discretize the points.
	for (int i = 0; i < N + 1; ++i) {

		// Update current x.
		xCurrent = xMin + delta * i;

		// Calculate a1 and a2.
		double a1 = calculateTriangleSurface(a, b, xCurrent);
		double a2 = calculateTriangleSurface(c, d, xCurrent);
		double surface = a1 + a2;

		//cout << "current a, b, c, d, x: " << a << ", " << b << ", " << c << ", " << d << ", " << xCurrent << " => " << a1 << ", " << a2 << endl;

		if (surface > maxSurface) {
			x = xCurrent;
			maxSurface = surface;
		}
	}

	return maxSurface;
}

double BMF::eval(Point start1, Point end1, double distance1, Point start2, Point end2, double distance2) const {

	// Calculate missing distances of the quadrilateral.
	double a = getGeodesicDistance(start1, start2);
	double b = distance1;
	double c = getGeodesicDistance(end1, end2);
	double d = distance2;
	double x;

	return calculateMaxSurface(a, b, c, d, x);
}

double BMF::evalOpt(Point start1, Point end1, double distance1, Point start2, Point end2, double distance2) const {

	// Calculate missing distances of the quadrilateral.
	double a = getGeodesicDistance(start1, start2);
	double b = distance1;
	double c = getGeodesicDistance(end1, end2);
	double d = distance2;
	double x;

	return calculateMaxSurfaceOpt(a, b, c, d, x);
}

class DatabaseImpl {

public:
	DatabaseImpl(const std::string& url, const std::string& db);

	mongocxx::instance m_instance;
	mongocxx::client m_client;

	mongocxx::database m_db;
	mongocxx::collection m_settings;
	mongocxx::collection m_mentors;
};

DatabaseImpl::DatabaseImpl(const std::string& url, const std::string& db) {

	mongocxx::uri uri(url);
	m_client = mongocxx::client(uri);

	m_db = mongocxx::database(m_client[db]);
	m_settings = m_db["settings"];
	m_mentors = m_db["mentors"];
}

Database::Database(const std::string& url, const std::string& db) :
	m_impl(new DatabaseImpl(url, db)) {
}

Database::~Database() {
}

}
