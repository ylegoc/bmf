project(bmf)
cmake_minimum_required(VERSION 3.0)

### Options ###

SET(CMAKE_CXX_FLAGS "-g")
ADD_DEFINITIONS(-std=c++11)

INCLUDE_DIRECTORIES(
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/src"
)

LINK_DIRECTORIES(
	"/usr/local/lib"
)

LINK_LIBRARIES(
)


### Apps ###

ADD_EXECUTABLE(bmftest
	src/test/Test.cpp
)

TARGET_LINK_LIBRARIES(bmftest PUBLIC
	-pthread
)


### Installs ###


INSTALL(
	TARGETS bmftest
	DESTINATION bin
)
