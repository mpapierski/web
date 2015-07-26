CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (web)

SET (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Force C++11 mode.
IF (CMAKE_COMPILER_IS_GNUCXX)
	EXECUTE_PROCESS (COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		MESSAGE (STATUS "C++11 activated.")
		ADD_DEFINITIONS ("-std=gnu++11")
	ELSEIF (GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
		MESSAGE (WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
		ADD_DEFINITIONS ("-std=gnu++0x")
	ELSE ()
		MESSAGE (FATAL_ERROR "C++11 needed. Therefore a gcc compiler with a version higher than 4.3 is needed.")
	ENDIF ()
ELSE (CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE (STATUS "C++11 activated.")
	# Clang++ does not recognize add_definitions ?
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

ADD_SUBDIRECTORY (deps)

INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/http-server/include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/http-server/deps/http-parser/)

ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (examples)

# ENABLE_TESTING ()
# ADD_SUBDIRECTORY (tests)
