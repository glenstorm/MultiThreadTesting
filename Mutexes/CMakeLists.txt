cmake_minimum_required( VERSION 3.1 )

set( PROJECT_NAME Mutexes )
project( ${PROJECT_NAME} )

include( CTest )

include_directories( ./ )

find_package( Boost COMPONENTS system thread chrono REQUIRED )
if( NOT Boost_FOUND )
	message(SEND_ERROR "Failed to find boost::{system}")
	return()
else()
	include_directories( ${Boost_INCLUDE_DIRS} )
endif()


set ( SOURCES
	main.cpp
)

add_executable( ${PROJECT_NAME} ${SOURCES} )

target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )

add_test( example_test ${PROJECT_NAME} )
