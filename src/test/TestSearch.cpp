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

	if (argc < 3) {
		cout << "arguments: N M" << endl;
		return 0;
	}

	int N = stoi(argv[1]);
	int M = stoi(argv[2]);

	// Access database.
	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongodb://mongo:mongo@localhost:27017");
	mongocxx::client client(uri);

	mongocxx::database db = client["bmf-test"];
	mongocxx::collection mentors = db["mentors"];
	mentors.drop();

	fillDatabase(mentors, M);

	while (true) {

		double startLng, endLng, startLat, endLat;

		cout << "Trajectory : ";
		cin >> startLng;
		cin >> startLat;
		cin >> endLng;
		cin >> endLat;

		Point start(toRadians(startLng), toRadians(startLat));
		Point end(toRadians(endLng), toRadians(endLat));
		double distance = bmfAPI.getGeodesicDistance(start, end);

		mongocxx::cursor cursor = mentors.find({});

		string bestPseudo;
		double bestScore = 1000.0;
		double bestStartLng, bestStartLat, bestEndLng, bestEndLat;

		// Iterate the documents
		for (bsoncxx::document::view document : cursor) {

			// Pseudo.
			string pseudo = document["pseudo"].get_utf8().value.to_string();

			// Start.
			bsoncxx::types::b_array a = document["start"].get_array();
			startLng = a.value[0].get_double();
			startLat = a.value[1].get_double();
			Point startMentor(toRadians(startLng), toRadians(startLat));

			// End.
			a = document["end"].get_array();
			endLng = a.value[0].get_double();
			endLat = a.value[1].get_double();
			Point endMentor(toRadians(endLng), toRadians(endLat));

			double distanceMentor = document["dist"].get_double().value;

			// Evaluate the trajectories.
			double score = bmfAPI.eval(start, end, distance, startMentor, endMentor, distanceMentor);
			double scoreOpt = bmfAPI.evalOpt(start, end, distance, startMentor, endMentor, distanceMentor);

			cout << pseudo << " has " << score << ", " << scoreOpt << endl;
			if (scoreOpt < bestScore) {
				bestScore = scoreOpt;
				bestPseudo = pseudo;
				bestStartLng = startLng;
				bestStartLat = startLat;
				bestEndLng = endLng;
				bestEndLat = endLat;
			}
		}

		cout << bestPseudo << " has " << bestScore << " @ google " << bestStartLat << ", " << bestStartLng << " - " << bestEndLat << ", " << bestEndLng << endl;

	}

	return 0;
}
