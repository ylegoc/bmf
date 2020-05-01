#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/cursor.hpp>
#include <iostream>
#include <string>
#include <random>

#include "BMF.h"

using namespace std;
using namespace bmf;

BMF bmfAPI;

random_device rdLng{};
mt19937 genLng{rdLng()};
normal_distribution<> dLng{5.71, 0.02};

random_device rdLat{};
mt19937 genLat{rdLat()};
normal_distribution<> dLat{45.19, 0.0125};

int m = 0;

void createMentor(
		const std::string& pseudo,
		double startLng,
		double startLat,
		double endLng,
		double endLat,
		mongocxx::collection& mentors) {

	bmf::Point start(bmf::toRadians(startLng), bmf::toRadians(startLat));
	bmf::Point end(bmf::toRadians(endLng), bmf::toRadians(endLat));
	double distance = bmfAPI.getGeodesicDistance(start, end);

	bsoncxx::builder::stream::document document{};
    document
		<< "pseudo" << pseudo
    	<< "mail" << "test@bmf.org"
		<< "start" << bsoncxx::builder::stream::open_array
			<< startLng << startLat
			<< bsoncxx::builder::stream::close_array
		<< "end" << bsoncxx::builder::stream::open_array
			<< endLng << endLat
			<< bsoncxx::builder::stream::close_array
		<< "dist" << distance;

	mentors.insert_one(document.view());
}

void fillDatabase(mongocxx::collection& mentors, int M) {

	for (int i = 1; i < M + 1; ++i) {
		string pseudo = "M" + to_string(i);

		double startLng = dLng(genLng);
		double startLat = dLat(genLat);

		double endLng = dLng(genLng);
		double endLat = dLat(genLat);

		createMentor(pseudo, startLng, startLat, endLng, endLat, mentors);
	}
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		cout << "arguments: M" << endl;
		return 0;
	}

	int M = stoi(argv[1]);

	// Access database.
	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongodb://mongo:mongo@localhost:27017");
	mongocxx::client client(uri);

	mongocxx::database db = client["bmf-test"];
	mongocxx::collection mentors = db["mentors"];
	mentors.drop();

	fillDatabase(mentors, M);

	return 0;
}
