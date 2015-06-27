cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86)

if (NOT PLATFORM_EMU)
	set(RELATIVE_SYSROOT_PATH sysroots/core2-32-poky-linux)
	set(RELATIVE_COMPILER_PATH sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux)
	set(CROSS_COMPILE i586-poky-linux-)

	set(COMPILER_PREFIX "${TOOLCHAIN_PATH}/${RELATIVE_COMPILER_PATH}/${CROSS_COMPILE}")

	set(EDISON_C_COMPILER "${COMPILER_PREFIX}gcc")
	set(EDISON_CXX_COMPILER "${COMPILER_PREFIX}g++")

	set(CMAKE_C_COMPILER "${EDISON_C_COMPILER}")
	set(CMAKE_CXX_COMPILER "${EDISON_CXX_COMPILER}")

	set(EDISON_SYSROOT_PATH "${TOOLCHAIN_PATH}/${RELATIVE_SYSROOT_PATH}")
	set(CMAKE_FIND_ROOT_PATH "${EDISON_SYSROOT_PATH}")
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

	set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer")
	set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -O2 -pipe -g -feliminate-unused-debug-types")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS} --sysroot=${EDISON_SYSROOT_PATH}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_C_FLAGS} --sysroot=${EDISON_SYSROOT_PATH}")

	set(COMMON_LD_FLAGS "${COMMON_LD_FLAGS} -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed --sysroot=${EDISON_SYSROOT_PATH}")

	include_directories("${EDISON_SYSROOT_PATH}/usr/include/c++")
	include_directories("${EDISON_SYSROOT_PATH}/usr/include/c++/i586-poky-linux")

	add_definitions(-DPLATFORM_EDISON)
else()
	add_definitions(-DPLATFORM_EMU)
endif ()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_LD_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COMMON_LD_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${COMMON_LD_FLAGS}")

cmake_policy(SET CMP0056 NEW)

project(edison)

if (DEFINED CMAKE_FIND_ROOT_PATH)
	set(PKG_CONFIG_PATH "")
	foreach(DIR ${CMAKE_FIND_ROOT_PATH})
		if (NOT PKG_CONFIG_PATH STREQUAL "")
			set(PKG_CONFIG_PATH "${PKG_CONFIG_PATH};")
		endif ()
		set(PKG_CONFIG_PATH "${PKG_CONFIG_PATH}${DIR}/usr/lib/pkgconfig")
	endforeach(DIR)
	set(ENV{PKG_CONFIG_PATH} ${PKG_CONFIG_PATH})
	message(STATUS "PKG_CONFIG_PATH: ${PKG_CONFIG_PATH}")
endif ()

set(CMAKE_BUILD_TYPE Debug)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
list(APPEND PLATFORM_LIBS ${ZLIB_LIBRARIES})

set(PLATFORM_LINUX TRUE)

add_subdirectory(stingraykit)
add_definitions(${STINGRAYKIT_DEFINITIONS})
include_directories(${STINGRAYKIT_INCLUDE_DIRECTORIES})

include_directories(.)
include_directories(stingraykit)
include_directories(Simple-FFT/include)

set(BIKEBRAIN_SOURCES
	bikebrain/App.cpp
	bikebrain/DistanceBasedCadenceReporter.cpp
	bikebrain/HttpStatsEngine.cpp
	bikebrain/WrappedCFont.cpp
	bikebrain/main.cpp

	bikebrain/platform/emu/EmuStatsEngine.cpp
)

if (NOT PLATFORM_EMU)
	list(APPEND BIKEBRAIN_SOURCES
		bikebrain/platform/edison/Button.cpp
	)
else ()
	list(APPEND BIKEBRAIN_SOURCES
		bikebrain/platform/emu/EmuButton.cpp
		bikebrain/platform/emu/EmuDisplay.cpp
		bikebrain/platform/emu/EmuDistanceSensor.cpp
		bikebrain/platform/emu/EmuTextDisplay.cpp
		bikebrain/platform/emu/StdinReader.cpp
	)
endif ()

set(EXECUTABLE_OUTPUT_PATH ./build)

add_executable(bikebrain ${BIKEBRAIN_SOURCES})
target_link_libraries(bikebrain stingraykit ${STINGRAYKIT_LIBS})
