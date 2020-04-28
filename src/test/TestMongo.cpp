#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/cursor.hpp>

using namespace std;

int main(int, char**) {

	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongodb://mongo:mongo@localhost:27017");
	mongocxx::client client(uri);

	mongocxx::database db = client["bmf"];
	mongocxx::collection coll = db["mentors"];
	mongocxx::cursor cursor = coll.find({});

	// Iterate the documents
	for (bsoncxx::document::view document : cursor) {
		cout << bsoncxx::to_json(document) << endl;

		// pseudo
		{
			bsoncxx::document::element element = document["pseudo"];
			bsoncxx::types::b_utf8 v = element.get_utf8();
			bsoncxx::stdx::string_view sview = v.value;
			string pseudo = sview.to_string();

			cout << "pseudo = " << pseudo << endl;

			string pseudoShort = document["pseudo"].get_utf8().value.to_string();
			cout << "pseudo short = " << pseudoShort << endl;
		}

		// start
		{
			bsoncxx::document::element element = document["start"];
			bsoncxx::types::b_array a = element.get_array();
			bsoncxx::array::view aview = a.value;
			bsoncxx::array::element lng = aview[0];
			double l = lng.get_double();

			cout << "start lng = " << l << endl;

			double lShort = document["start"].get_array().value[0].get_double();
			cout << "start lng short = " << lShort << endl;
		}

	}

	return 0;
}
