cmake_minimum_required( VERSION 2.8 )
project( wyrm-client )

find_package( SFML 2.0 REQUIRED SYSTEM NETWORK WINDOW GRAPHICS AUDIO )
find_package( SFGUI )
find_package( Boost 1.46 REQUIRED )

if( WINDOWS )
	set( BIN_EXTENSION ".exe" )
endif()

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )

add_definitions( -D SFML_EXPORTS )
add_definitions( -D SFGUI_EXPORTS )

set(
	SOURCES
	${SRC_DIR}/backdrop.cpp
	${SRC_DIR}/backdrop.h
	${SRC_DIR}/events.cpp
	${SRC_DIR}/events.h
	${SRC_DIR}/game.cpp
	${SRC_DIR}/game.h
	${SRC_DIR}/gui.cpp
	${SRC_DIR}/gui.h
	${SRC_DIR}/guievents.cpp
	${SRC_DIR}/guievents.h
	${SRC_DIR}/inventory.cpp
	${SRC_DIR}/inventory.h
	${SRC_DIR}/main.cpp
	${SRC_DIR}/network.cpp
	${SRC_DIR}/network.h
	${SRC_DIR}/objectmanager.cpp
	${SRC_DIR}/objectmanager.h
	${SRC_DIR}/perlin.cpp
	${SRC_DIR}/perlin.h
	${SRC_DIR}/player.cpp
	${SRC_DIR}/player.h
	${SRC_DIR}/resourcemanager.cpp
	${SRC_DIR}/resourcemanager.h
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
	${SRC_DIR}/particlesystem/particle.cpp
	${SRC_DIR}/particlesystem/particle.h
	${SRC_DIR}/particlesystem/particleemitter.cpp
	${SRC_DIR}/particlesystem/particleemitter.h
	${SRC_DIR}/particlesystem/particlesystem.cpp
	${SRC_DIR}/particlesystem/particlesystem.h
)

include_directories( ${SFML_INCLUDE_DIR} )
include_directories( ${Boost_INCLUDE_DIR} )

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

target_link_libraries( "wyrm-client" ${LIBRARIES}  )
