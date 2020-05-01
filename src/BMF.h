#ifndef BMF_H
#define BMF_H

#include <cmath>
#include <string>
#include <memory>
#include <deque>

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
	 * Initialises the API.
	 */
	BMF();

	/**
	 * Sets the number of calculations per surface.
	 * Prefer an odd value.
	 */
	void setNumberOfCalculations(int N);

	/**
	 * Gets the geodesic distance between two Point with geodesic coordinates (radians).
	 * a.x = lng, a.y = lat
	 */
	double getGeodesicDistance(const Point & a, const Point & b) const;

	/**
	 * Calculates the surface of a triangle.
	 */
	double calculateTriangleSurface(double a, double b, double c) const;

	/**
	 * Calculates the maximum surface for the quadrileter with sides a, b, c, d. The value x is the diagonal found for the maximum surface.
	 */
	double calculateMaxSurface(double a, double b, double c, double d, double& x, int N = -1) const;

	/**
	 * Calculates the maximum surface for the quadrilateral with sides a, b, c, d. The value x is the diagonal found for the maximum surface.
	 * Prefer an odd value for N. Value of 1, 3 or 5 should be enough for our purpose.
	 */
	double calculateMaxSurfaceOpt(double a, double b, double c, double d, double& x, int N = -1) const;

	/**
	 * Evaluates the distance between two trajectories.
	 */
	double eval(const Point& start1, const Point& end1, double distance1, const Point& start2, const Point& end2, double distance2) const;

	/**
	 * Evaluates the distance between two trajectories.
	 */
	double evalOpt(const Point& start1, const Point& end1, double distance1, const Point& start2, const Point& end2, double distance2) const;

private:
	int m_N;

	static constexpr double R = 6371.0;
};

/**
 * Search type.
 */
enum class SearchType {START, END, TRAJECTORY};

class DatabaseImpl;

/**
 * Class database defining a connection to the mongo database.
 */
class Database {

public:
	/**
	 * Initialises the connection.
	 */
	Database(const std::string& url, const std::string& db);

	/**
	 * Destructor.
	 */
	~Database();

	/**
	 * Finds the best mentors for the trajectory start - end, using search type and limiting to numberOfResults.
	 */
	std::deque<std::string> findBestMentors(const Point& start, const Point& end, SearchType searchType, int numberOfResults);

private:
	std::unique_ptr<DatabaseImpl> m_impl;
};

}

#endif
