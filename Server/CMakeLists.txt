cmake_minimum_required( VERSION 2.8 )
project( wyrm-server )

find_package( SFML 2.0 REQUIRED SYSTEM NETWORK )
find_package( Boost 1.46 REQUIRED )

if( WINDOWS )
	set( BIN_EXTENSION ".exe" )
endif()

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )

set(
	SOURCES
	${SRC_DIR}/auth.cpp
	${SRC_DIR}/auth.h
	${SRC_DIR}/game.cpp
	${SRC_DIR}/game.h
	${SRC_DIR}/inventory.cpp
	${SRC_DIR}/inventory.h
	${SRC_DIR}/main.cpp
	${SRC_DIR}/networkmanager.cpp
	${SRC_DIR}/networkmanager.h
	${SRC_DIR}/objectmanager.cpp
	${SRC_DIR}/objectmanager.h
	${SRC_DIR}/player.cpp
	${SRC_DIR}/player.h
	${SRC_DIR}/playermanager.cpp
	${SRC_DIR}/playermanager.h
	${SRC_DIR}/system.cpp
	${SRC_DIR}/system.h
	${SRC_DIR}/utility.cpp
	${SRC_DIR}/utility.h
	${SRC_DIR}/objects/object.cpp
	${SRC_DIR}/objects/object.h
	${SRC_DIR}/objects/objects.h
	${SRC_DIR}/objects/planet.cpp
	${SRC_DIR}/objects/planet.h
	${SRC_DIR}/objects/ship.cpp
	${SRC_DIR}/objects/ship.h
	${SRC_DIR}/objects/star.cpp
	${SRC_DIR}/objects/star.h
	${SRC_DIR}/items/item.cpp
	${SRC_DIR}/items/item.h
)

include_directories( ${SFML_INCLUDE_DIR} )
include_directories( ${Boost_INCLUDE_DIR} )

add_executable( "wyrm-server" ${SOURCES} )

target_link_libraries( "wyrm-server" ${SFML_SYSTEM_LIBRARY} ${SFML_NETWORK_LIBRARY} )
