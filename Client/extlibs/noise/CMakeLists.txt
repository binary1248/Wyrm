cmake_minimum_required( VERSION 2.8 )

set( NOISE_SRC_DIR "${PROJECT_SOURCE_DIR}/extlibs/noise/src" )
set( NOISE_INC_DIR "${PROJECT_SOURCE_DIR}/extlibs/noise/src" )

set(
	SOURCES
	${NOISE_SRC_DIR}/noise/latlon.cpp
	${NOISE_SRC_DIR}/noise/noisegen.cpp
	${NOISE_SRC_DIR}/noise/noiseutils.cpp
  ${NOISE_SRC_DIR}/noise/model/cylinder.cpp
	${NOISE_SRC_DIR}/noise/model/line.cpp
	${NOISE_SRC_DIR}/noise/model/plane.cpp
	${NOISE_SRC_DIR}/noise/model/sphere.cpp
  ${NOISE_SRC_DIR}/noise/module/abs.cpp
	${NOISE_SRC_DIR}/noise/module/add.cpp
	${NOISE_SRC_DIR}/noise/module/billow.cpp
	${NOISE_SRC_DIR}/noise/module/blend.cpp
	${NOISE_SRC_DIR}/noise/module/cache.cpp
	${NOISE_SRC_DIR}/noise/module/checkerboard.cpp
	${NOISE_SRC_DIR}/noise/module/clamp.cpp
	${NOISE_SRC_DIR}/noise/module/const.cpp
	${NOISE_SRC_DIR}/noise/module/curve.cpp
	${NOISE_SRC_DIR}/noise/module/cylinders.cpp
	${NOISE_SRC_DIR}/noise/module/displace.cpp
	${NOISE_SRC_DIR}/noise/module/exponent.cpp
	${NOISE_SRC_DIR}/noise/module/invert.cpp
	${NOISE_SRC_DIR}/noise/module/max.cpp
	${NOISE_SRC_DIR}/noise/module/min.cpp
	${NOISE_SRC_DIR}/noise/module/modulebase.cpp
	${NOISE_SRC_DIR}/noise/module/multiply.cpp
	${NOISE_SRC_DIR}/noise/module/perlin.cpp
	${NOISE_SRC_DIR}/noise/module/power.cpp
	${NOISE_SRC_DIR}/noise/module/ridgedmulti.cpp
	${NOISE_SRC_DIR}/noise/module/rotatepoint.cpp
	${NOISE_SRC_DIR}/noise/module/scalebias.cpp
	${NOISE_SRC_DIR}/noise/module/scalepoint.cpp
	${NOISE_SRC_DIR}/noise/module/select.cpp
	${NOISE_SRC_DIR}/noise/module/spheres.cpp
	${NOISE_SRC_DIR}/noise/module/terrace.cpp
	${NOISE_SRC_DIR}/noise/module/translatepoint.cpp
	${NOISE_SRC_DIR}/noise/module/turbulence.cpp
	${NOISE_SRC_DIR}/noise/module/voronoi.cpp
	${NOISE_INC_DIR}/noise/basictypes.h
	${NOISE_INC_DIR}/noise/exception.h
	${NOISE_INC_DIR}/noise/interp.h
	${NOISE_INC_DIR}/noise/latlon.h
	${NOISE_INC_DIR}/noise/mathconsts.h
	${NOISE_INC_DIR}/noise/misc.h
	${NOISE_INC_DIR}/noise/noisegen.h
	${NOISE_INC_DIR}/noise/noise.h
	${NOISE_INC_DIR}/noise/vectortable.h
	${NOISE_INC_DIR}/noise/noiseutils.h
  ${NOISE_INC_DIR}/noise/model/cylinder.h
	${NOISE_INC_DIR}/noise/model/model.h
	${NOISE_INC_DIR}/noise/model/sphere.h
	${NOISE_INC_DIR}/noise/model/line.h
	${NOISE_INC_DIR}/noise/model/plane.h
  ${NOISE_INC_DIR}/noise/module/abs.h
	${NOISE_INC_DIR}/noise/module/add.h
	${NOISE_INC_DIR}/noise/module/billow.h
	${NOISE_INC_DIR}/noise/module/blend.h
	${NOISE_INC_DIR}/noise/module/cache.h
	${NOISE_INC_DIR}/noise/module/checkerboard.h
	${NOISE_INC_DIR}/noise/module/clamp.h
	${NOISE_INC_DIR}/noise/module/const.h
	${NOISE_INC_DIR}/noise/module/curve.h
	${NOISE_INC_DIR}/noise/module/cylinders.h
	${NOISE_INC_DIR}/noise/module/displace.h
	${NOISE_INC_DIR}/noise/module/exponent.h
	${NOISE_INC_DIR}/noise/module/invert.h
	${NOISE_INC_DIR}/noise/module/max.h
	${NOISE_INC_DIR}/noise/module/min.h
	${NOISE_INC_DIR}/noise/module/module.h
	${NOISE_INC_DIR}/noise/module/modulebase.h
	${NOISE_INC_DIR}/noise/module/multiply.h
	${NOISE_INC_DIR}/noise/module/perlin.h
	${NOISE_INC_DIR}/noise/module/power.h
	${NOISE_INC_DIR}/noise/module/ridgedmulti.h
	${NOISE_INC_DIR}/noise/module/rotatepoint.h
	${NOISE_INC_DIR}/noise/module/scalebias.h
	${NOISE_INC_DIR}/noise/module/scalepoint.h
	${NOISE_INC_DIR}/noise/module/select.h
	${NOISE_INC_DIR}/noise/module/spheres.h
	${NOISE_INC_DIR}/noise/module/terrace.h
	${NOISE_INC_DIR}/noise/module/translatepoint.h
	${NOISE_INC_DIR}/noise/module/turbulence.h
	${NOISE_INC_DIR}/noise/module/voronoi.h
)

include_directories( ${NOISE_INC_DIR}/noise )

add_library( noise ${SOURCES} )

