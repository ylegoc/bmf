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
	src/lib/BMF.cpp src/lib/BMF.h
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


### Installs ###

INSTALL(
	TARGETS bmf
	DESTINATION lib
)

#INSTALL(
#	TARGETS bmftestmongo
#	DESTINATION bin
#)
