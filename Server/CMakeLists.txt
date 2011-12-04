cmake_minimum_required( VERSION 2.8 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE )
endif()

project( wyrm-server )

find_package( SFML 2.0 REQUIRED SYSTEM NETWORK )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	set( WINDOWS 1 )
endif()

if( CMAKE_COMPILER_IS_GNUCXX )
	set( COMPILER_GCC 1 )
elseif( MSVC_VERSION )
	set( COMPILER_MSVC 1 )
endif()

if( WINDOWS )
	set( BIN_EXTENSION ".exe" )
	set( SFML_STATIC false CACHE BOOL "Was SFML built as a static library?" )
endif()

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )

if( SFML_STATIC )
	add_definitions( -D SFML_STATIC )
endif()

if( COMPILER_GCC )
	if( NOT CMAKE_CXX_FLAGS )
		set( CMAKE_CXX_FLAGS "--std=c++0x -Wall -Wextra -Wshadow -Wconversion -pedantic" CACHE STRING "C++ compiler flags" FORCE )
		set( CMAKE_C_FLAGS "--std=c++0x -Wall -Wextra -Wshadow -Wconversion -pedantic" CACHE STRING "C compiler flags" FORCE )
	endif()

	if( NOT CMAKE_CXX_FLAGS_DEBUG )
		set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "C++ compiler flags (debug)" FORCE )
		set( CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE STRING "C++ compiler flags (debug)" FORCE )
	endif()
endif()

set(
	SOURCES
	${SRC_DIR}/auth.cpp
	${SRC_DIR}/auth.hpp
	${SRC_DIR}/config.hpp
	${SRC_DIR}/game.cpp
	${SRC_DIR}/game.hpp
	${SRC_DIR}/inventory.cpp
	${SRC_DIR}/inventory.hpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/networkmanager.cpp
	${SRC_DIR}/networkmanager.hpp
	${SRC_DIR}/objectmanager.cpp
	${SRC_DIR}/objectmanager.hpp
	${SRC_DIR}/player.cpp
	${SRC_DIR}/player.hpp
	${SRC_DIR}/playermanager.cpp
	${SRC_DIR}/playermanager.hpp
	${SRC_DIR}/system.cpp
	${SRC_DIR}/system.hpp
	${SRC_DIR}/utility.cpp
	${SRC_DIR}/utility.hpp
	${SRC_DIR}/utility.inl
	${SRC_DIR}/objects/object.cpp
	${SRC_DIR}/objects/object.hpp
	${SRC_DIR}/objects/objects.hpp
	${SRC_DIR}/objects/planet.cpp
	${SRC_DIR}/objects/planet.hpp
	${SRC_DIR}/objects/ship.cpp
	${SRC_DIR}/objects/ship.hpp
	${SRC_DIR}/objects/star.cpp
	${SRC_DIR}/objects/star.hpp
	${SRC_DIR}/items/item.cpp
	${SRC_DIR}/items/item.hpp
)

include_directories( ${SRC_DIR} )
include_directories( ${SFML_INCLUDE_DIR} )

add_executable( "wyrm-server" ${SOURCES} )

target_link_libraries( "wyrm-server" ${SFML_SYSTEM_LIBRARY} ${SFML_NETWORK_LIBRARY} )
