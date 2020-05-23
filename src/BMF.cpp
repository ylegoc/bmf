#include "BMF.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
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

double BMF::eval(const Point& start1, const Point& end1, double distance1, const Point& start2, const Point& end2, double distance2) const {

	// Calculate missing distances of the quadrilateral.
	double a = getGeodesicDistance(start1, start2);
	double b = distance1;
	double c = getGeodesicDistance(end1, end2);
	double d = distance2;
	double x;

	return calculateMaxSurface(a, b, c, d, x);
}

double BMF::evalOpt(const Point& start1, const Point& end1, double distance1, const Point& start2, const Point& end2, double distance2) const {

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

	std::deque<MentorResult> findBestMentors(const Point& start, const Point& end, SearchType searchType, int numberOfResults);

	mongocxx::instance m_instance;
	mongocxx::client m_client;

	mongocxx::database m_db;
	mongocxx::collection m_settings;
	mongocxx::collection m_mentors;

	BMF m_bmf;
};

DatabaseImpl::DatabaseImpl(const std::string& url, const std::string& db) {

	mongocxx::uri uri(url);
	m_client = mongocxx::client(uri);

	m_db = mongocxx::database(m_client[db]);
	m_settings = m_db["settings"];
	m_mentors = m_db["mentors"];

	// TODO set N from settings.
	// m_bmf.setNumberOfCalculations(N);
}

void updateResult(std::deque<MentorResult>& result, double score, const std::string& mentor, int numberOfResults) {

	// Insert immediately result is empty.
	if (result.empty()) {
		result.push_back(MentorResult(mentor, score));
		return;
	}

	// Iterate the results.
	auto it = result.begin();

	while (it != result.end()) {

		if (score < it->score) {
			result.insert(it, MentorResult(mentor, score));

			// Resize the result if size is too large.
			if (result.size() > numberOfResults) {
				result.pop_back();
			}
			return;
		}

		++it;
	}

	// The mentor is not yet inserted.
	// Do it if the result is not full.
	if (result.size() < numberOfResults) {
		result.push_back(MentorResult(mentor, score));
	}
}

std::deque<MentorResult> DatabaseImpl::findBestMentors(const Point& start, const Point& end, SearchType searchType, int numberOfResults) {

	deque<MentorResult> result;

	// Calculate the distance of the trajectory.
	double distance = m_bmf.getGeodesicDistance(start, end);

	mongocxx::cursor cursor = m_mentors.find({});

	// Iterate the documents.
	for (bsoncxx::document::view document : cursor) {

		try {
			// Start.
			bsoncxx::types::b_array a = document["start"].get_array();
			double startLng = a.value[0].get_double();
			double startLat = a.value[1].get_double();
			Point startMentor(toRadians(startLng), toRadians(startLat));

			// End.
			a = document["end"].get_array();
			double endLng = a.value[0].get_double();
			double endLat = a.value[1].get_double();
			Point endMentor(toRadians(endLng), toRadians(endLat));

			double distanceMentor = document["dist"].get_double().value;

			// Evaluate the trajectories.
			double score = 0.0;

			if (searchType == SearchType::START) {
				score = m_bmf.getGeodesicDistance(start, startMentor);
			}
			else if (searchType == SearchType::END) {
				score = m_bmf.getGeodesicDistance(end, endMentor);
			}
			else {
				//score = sqrt(m_bmf.evalOpt(start, end, distance, startMentor, endMentor, distanceMentor));
				//score = sqrt(m_bmf.getGeodesicDistance(start, startMentor) * m_bmf.getGeodesicDistance(end, endMentor));
				score = 0.5 * (m_bmf.getGeodesicDistance(start, startMentor) + m_bmf.getGeodesicDistance(end, endMentor));
			}

			// Update the result.
			updateResult(result, score, bsoncxx::to_json(document), numberOfResults);
		}
		catch (...) {
			cerr << "findBestMentors: Unexpected exception with document with _id " << document["_id"].get_oid().value.to_string() << endl;
		}
	}

	return result;
}


Database::Database(const std::string& url, const std::string& db) :
	m_impl(new DatabaseImpl(url, db)) {
}

Database::~Database() {
}

std::deque<MentorResult> Database::findBestMentors(const Point& start, const Point& end, SearchType searchType, int numberOfResults) {
	return m_impl->findBestMentors(start, end, searchType, numberOfResults);
}

}
