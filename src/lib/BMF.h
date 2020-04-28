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
	 * Sets the center.
	 */
	void setCenter(double centerLng, double centerLat);

	/**
	 * Gets the euclidean distance between two Point with length coordinates.
	 */
	double getEuclideanDistance(const Point & a, const Point & b = Point()) const;

	/**
	 * Gets the geodesic distance between two Point with geodesic coordinates (radians).
	 * a.x = lng, a.y = lat
	 */
	double getGeodesicDistance(const Point & a, const Point & b) const;

	/**
	 * Gets the local point from longitude and latitude.
	 */
	Point getLocalPoint(double lng, double lat) const;

private:
	Point m_center;

	static constexpr double R = 6371.0;
};

}

#endif
