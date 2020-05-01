#include <node.h>
#include <iostream>
#include <unistd.h>
#include <uv.h>
#include <thread>
#include <sstream>
#include <functional>
#include <string>
#include "BMF.h"

using namespace std;
using namespace std::placeholders;

namespace bmf {

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Context;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;
using v8::Integer;
using v8::Boolean;
using v8::Array;
using v8::Persistent;

Isolate * v8Isolate;

bmf::BMF bmfAPI;
std::unique_ptr<bmf::Database> database;

/**
 * Init function to initialise the connection.
 */
void Init(const FunctionCallbackInfo<Value>& args) {

	cout << "initialising bmf addon" << endl;

	// Get the V8 isolate.
	v8Isolate = args.GetIsolate();

	// Get the args.
	v8::String::Utf8Value param0(args[0]->ToString());
	string url(*param0);

    v8::String::Utf8Value param1(args[1]->ToString());
	string db(*param1);

    cout << "url = " << url << endl;
    cout << "db = " << db << endl;

    database.reset(new bmf::Database(url, db));

	args.GetReturnValue().Set(Undefined(v8Isolate));
}

void GetDistance(const FunctionCallbackInfo<Value>& args) {

    double startLng = Local<Number>::Cast(args[0])->Value();
    double startLat = Local<Number>::Cast(args[1])->Value();
    double endLng = Local<Number>::Cast(args[2])->Value();
    double endLat = Local<Number>::Cast(args[3])->Value();

    bmf::Point start(bmf::toRadians(startLng), bmf::toRadians(startLat));
    bmf::Point end(bmf::toRadians(endLng), bmf::toRadians(endLat));

    double distance = bmfAPI.getGeodesicDistance(start, end);
	
    args.GetReturnValue().Set(Number::New(args.GetIsolate(), distance));
}

void FindBestMentors(const FunctionCallbackInfo<Value>& args) {

    double startLng = Local<Number>::Cast(args[0])->Value();
    double startLat = Local<Number>::Cast(args[1])->Value();
    double endLng = Local<Number>::Cast(args[2])->Value();
    double endLat = Local<Number>::Cast(args[3])->Value();

    bmf::Point start(bmf::toRadians(startLng), bmf::toRadians(startLat));
    bmf::Point end(bmf::toRadians(endLng), bmf::toRadians(endLat));

    v8::String::Utf8Value param(args[4]->ToString());
	string searchTypeParam(*param);

    bmf::SearchType searchType = bmf::SearchType::TRAJECTORY;

    if (searchTypeParam == "START") {
        searchType = bmf::SearchType::START;
    }
    else if (searchTypeParam == "END") {
        searchType = bmf::SearchType::END;
    }
    if (searchTypeParam == "TRAJECTORY") {
        searchType = bmf::SearchType::TRAJECTORY;
    }

    int numberOfResults = Local<Integer>::Cast(args[5])->Value();

    deque<MentorResult> results = database->findBestMentors(start, end, searchType, numberOfResults);

    Local<Array> resultArray = Array::New(v8Isolate, results.size());

    int i = 0;
    for (MentorResult m : results) {
        //cout << "mentor " << m.mentor << " @ " << m.score << endl;

        Local<Object> result = Object::New(v8Isolate);
        result->Set(String::NewFromUtf8(v8Isolate, "mentor"), String::NewFromUtf8(v8Isolate, m.mentor.c_str()));
        result->Set(String::NewFromUtf8(v8Isolate, "score"), Number::New(v8Isolate, m.score));

        resultArray->Set(i, result);
        ++i;
    }

    args.GetReturnValue().Set(resultArray);
}

/**
 * The init function declares what we will make visible to node.
 */
void init(Local<Object> exports) {

	// Register the functions.
	NODE_SET_METHOD(exports, "init", Init);
	NODE_SET_METHOD(exports, "getDistance", GetDistance);
    NODE_SET_METHOD(exports, "findBestMentors", FindBestMentors);
}

NODE_MODULE(addonbmf, init)

}
