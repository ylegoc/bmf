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

	mongocxx::database db = client["bmf"];
	mongocxx::collection coll = db["mentors"];
	mongocxx::cursor cursor = coll.find({});

	// Initialise with Grenoble Hotel de Ville
	BMF bmf(5.7387279, 45.1849524);

	// Iterate the documents
	for (bsoncxx::document::view document : cursor) {

		// Pseudo
		string pseudo = document["pseudo"].get_utf8().value.to_string();

		// Start
		double lng = document["start"].get_array().value[0].get_double();
		double lat = document["start"].get_array().value[1].get_double();

		cout << "Pseudo " << pseudo << " @ " << lng << ", " << lat << endl;
		cout << "Distance x, y = " << bmf.getEuclideanDistance(bmf.getPoint(lng, lat)) << endl << endl;
	}

	return 0;
}
