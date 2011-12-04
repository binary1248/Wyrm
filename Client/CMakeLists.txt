cmake_minimum_required( VERSION 2.8 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE )
endif()

project( wyrm-client )

find_package( SFML 2.0 REQUIRED SYSTEM NETWORK WINDOW GRAPHICS AUDIO )
find_package( SFGUI )

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
	set( SFML_STATIC true CACHE BOOL "Was SFML built as a static library?" )
	set( SFGUI_STATIC true CACHE BOOL "Was SFGUI built as a static library?" )
endif()

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )

if( SFGUI_STATIC )
	add_definitions( -D SFGUI_STATIC )
endif()

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
	${SRC_DIR}/backdrop.cpp
	${SRC_DIR}/backdrop.hpp
	${SRC_DIR}/config.hpp
	${SRC_DIR}/events.cpp
	${SRC_DIR}/events.hpp
	${SRC_DIR}/game.cpp
	${SRC_DIR}/game.hpp
	${SRC_DIR}/gui.cpp
	${SRC_DIR}/gui.hpp
	${SRC_DIR}/guievents.cpp
	${SRC_DIR}/guievents.hpp
	${SRC_DIR}/inventory.cpp
	${SRC_DIR}/inventory.hpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/network.cpp
	${SRC_DIR}/network.hpp
	${SRC_DIR}/objectmanager.cpp
	${SRC_DIR}/objectmanager.hpp
	${SRC_DIR}/perlin.cpp
	${SRC_DIR}/perlin.hpp
	${SRC_DIR}/player.cpp
	${SRC_DIR}/player.hpp
	${SRC_DIR}/utility.cpp
	${SRC_DIR}/utility.hpp
	${SRC_DIR}/utility.inl
	${SRC_DIR}/resourcemanager.cpp
	${SRC_DIR}/resourcemanager.hpp
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
	${SRC_DIR}/particlesystem/particle.cpp
	${SRC_DIR}/particlesystem/particle.hpp
	${SRC_DIR}/particlesystem/particleemitter.cpp
	${SRC_DIR}/particlesystem/particleemitter.hpp
	${SRC_DIR}/particlesystem/particlesystem.cpp
	${SRC_DIR}/particlesystem/particlesystem.hpp
)

include_directories( ${SRC_DIR} )
include_directories( ${SFML_INCLUDE_DIR} )
include_directories( "extlibs/noise/src" )

add_subdirectory( "extlibs/noise" )

add_executable( "wyrm-client" ${SOURCES} )

if( WINDOWS )
	set(
		LIBRARIES
		glu32
		opengl32
	)
else()
	set(
		LIBRARIES
		openal
		Xrandr
		GL
	)
endif()

set(
	LIBRARIES
	${LIBRARIES}
	pthread
	jpeg
	freetype
	GLEW
	${SFML_SYSTEM_LIBRARY}
	${SFML_GRAPHICS_LIBRARY}
	${SFML_WINDOW_LIBRARY}
	${SFML_NETWORK_LIBRARY}
	${SFML_AUDIO_LIBRARY}
	${SFGUI_LIBRARY}
)

target_link_libraries( "wyrm-client" ${LIBRARIES} noise )
