#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/cursor.hpp>

#include "lib/BMF.h"

using namespace std;

bmf::BMF bmfAPI;

void createMentor(
		const std::string& pseudo,
		const std::string& mail,
		double startLng,
		double startLat,
		double endLng,
		double endLat,
		mongocxx::collection& mentors) {


	// Calculate local start and local end.
	bmf::Point localStart = bmfAPI.getLocalPoint(startLng, startLat);
	bmf::Point localEnd = bmfAPI.getLocalPoint(endLng, endLat);

	bsoncxx::builder::stream::document document{};
    document
		<< "pseudo" << pseudo
    	<< "mail" << mail
		<< "start" << bsoncxx::builder::stream::open_array
			<< startLng << startLat
			<< bsoncxx::builder::stream::close_array
		<< "end" << bsoncxx::builder::stream::open_array
			<< endLng << endLat
			<< bsoncxx::builder::stream::close_array
		<< "ls" << bsoncxx::builder::stream::open_array
			<< localStart.x << localStart.y
			<< bsoncxx::builder::stream::close_array
		<< "le" << bsoncxx::builder::stream::open_array
			<< localEnd.x << localEnd.y
			<< bsoncxx::builder::stream::close_array;

	mentors.insert_one(document.view());
}

int main(int, char**) {

	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongodb://mongo:mongo@localhost:27017");
	mongocxx::client client(uri);

	mongocxx::database db = client["bmf-proto"];

	mongocxx::collection settings = db["settings"];

	mongocxx::cursor cursor = settings.find({});
	bsoncxx::document::view document = *cursor.begin();

	double lng = document["center"].get_array().value[0].get_double();
	double lat = document["center"].get_array().value[1].get_double();

	bmfAPI.setCenter(lng, lat);

	mongocxx::collection mentors = db["mentors"];
	mentors.drop();

	createMentor("yanyan", "ylegoc@gmail.com", 5.7131486, 45.1798167, 5.6913905, 45.2066135, mentors);
	createMentor("esrf", "mail@esrf.org", 5.7300543, 45.1906798, 5.7031652, 45.2045723, mentors);
	createMentor("campus", "mail@campus.org", 5.716966, 45.1806586, 5.7650806, 45.1900485, mentors);

	return 0;
}
