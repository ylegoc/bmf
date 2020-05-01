project(bmf)
cmake_minimum_required(VERSION 3.0)

### Options ###

SET(CMAKE_CXX_FLAGS "-g")
ADD_DEFINITIONS(-std=c++11)

INCLUDE_DIRECTORIES(
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/src"
	"/usr/local/include/mongocxx/v_noabi"
	"/usr/local/include/bsoncxx/v_noabi"
)

LINK_DIRECTORIES(
	"/usr/local/lib"
)

LINK_LIBRARIES(
	-lmongocxx -lbsoncxx
)

### Lib ###
ADD_LIBRARY(bmf SHARED
	src/BMF.cpp src/BMF.h
)

target_link_libraries(bmf

)


### Apps ###

ADD_EXECUTABLE(bmfinitproto
	src/test/InitProtoDB.cpp
)

TARGET_LINK_LIBRARIES(bmfinitproto PUBLIC
	bmf -lmongocxx -lbsoncxx
)


ADD_EXECUTABLE(bmfinittest
	src/test/InitTestDB.cpp
)

TARGET_LINK_LIBRARIES(bmfinittest PUBLIC
	bmf -lmongocxx -lbsoncxx
)


ADD_EXECUTABLE(bmftestmongo
	src/test/TestMongo.cpp
)

TARGET_LINK_LIBRARIES(bmftestmongo PUBLIC
	-lmongocxx -lbsoncxx -pthread
)


ADD_EXECUTABLE(bmftestmongodistance
	src/test/TestMongoDistance.cpp
)

TARGET_LINK_LIBRARIES(bmftestmongodistance PUBLIC
	bmf -lmongocxx -lbsoncxx -pthread
)


ADD_EXECUTABLE(bmftestsurface
	src/test/TestSurface.cpp
)

TARGET_LINK_LIBRARIES(bmftestsurface PUBLIC
	bmf
)


ADD_EXECUTABLE(bmftestsearch
	src/test/TestSearch.cpp
)

TARGET_LINK_LIBRARIES(bmftestsearch PUBLIC
	bmf -lmongocxx -lbsoncxx -pthread
)


### Installs ###

INSTALL(
	TARGETS bmf
	DESTINATION lib
)

INSTALL(DIRECTORY src/
	DESTINATION include
 	FILES_MATCHING PATTERN "BMF.h"
)