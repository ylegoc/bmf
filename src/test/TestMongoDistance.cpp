#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/cursor.hpp>

#include "lib/BMF.h"

using namespace std;
using namespace bmf;

int main(int, char**) {

	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongodb://mongo:mongo@localhost:27017");
	mongocxx::client client(uri);

	mongocxx::database db = client["bmf-proto"];

	// Initialise with Grenoble Hotel de Ville
	BMF bmfAPI;

	mongocxx::collection settings = db["settings"];

	mongocxx::cursor cursor = settings.find({});
	bsoncxx::document::view document = *cursor.begin();

	double lng = document["center"].get_array().value[0].get_double();
	double lat = document["center"].get_array().value[1].get_double();

	bmfAPI.setCenter(lng, lat);


	mongocxx::collection mentors = db["mentors"];
	cursor = mentors.find({});

	// Iterate the documents
	for (bsoncxx::document::view document : cursor) {

		// Pseudo
		string pseudo = document["pseudo"].get_utf8().value.to_string();

		// Start
		double lng = document["start"].get_array().value[0].get_double();
		double lat = document["start"].get_array().value[1].get_double();

		cout << "Pseudo " << pseudo << " @ " << lng << ", " << lat << endl;
		cout << "Distance x, y = " << bmfAPI.getEuclideanDistance(bmfAPI.getLocalPoint(lng, lat)) << endl << endl;
	}

	return 0;
}
