#ifndef BMF_H
#define BMF_H

#include <cmath>

namespace bmf {

/**
 * Point 2D representing local projections.
 */
struct Point {

	double x;
	double y;

	Point() : x(0.0), y(0.0) {}
	Point(double cx, double cy) : x(cx), y(cy) {}
};

inline double toRadians(double angle) {
	return (M_PI / 180.0) * angle;
}

inline double sign(double value) {
	if (value > 0.0) {
		return 1.0;
	}
	return -1.0;
}

/**
 * Class providing the calculations of BMF.
 */
class BMF {

public:
	/**
	 * Initialises with the longitude and latitude of the center in degrees.
	 */
	BMF();

	/**
	 * Gets the geodesic distance between two Point with geodesic coordinates (radians).
	 * a.x = lng, a.y = lat
	 */
	double getGeodesicDistance(const Point & a, const Point & b) const;

private:
	static constexpr double R = 6371.0;
};

}

#endif
