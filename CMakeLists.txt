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


### Apps ###

ADD_EXECUTABLE(bmftest
	src/test/Test.cpp
)

TARGET_LINK_LIBRARIES(bmftest PUBLIC
	-lmongocxx -lbsoncxx -pthread
)


### Installs ###


INSTALL(
	TARGETS bmftest
	DESTINATION bin
)
