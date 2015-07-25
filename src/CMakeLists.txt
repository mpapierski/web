CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (web)

SET (SOURCES
	request.cpp
	response.cpp
	application.cpp
	exceptions.cpp
	server.cpp
)

ADD_LIBRARY (web
	${SOURCES}
)

TARGET_LINK_LIBRARIES (web
	http_server)