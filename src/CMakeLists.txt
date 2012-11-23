CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (web)

SET (SOURCES
	request.cpp
	response.cpp
	application.cpp
)

ADD_LIBRARY (web
	${SOURCES}
)
