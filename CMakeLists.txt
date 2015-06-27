cmake_minimum_required(VERSION 2.8.3)

set(ENV{PATH} "/home/projects/edison/toolchain/edison-sdk-linux64-ww18-15/sysroots/x86_64-pokysdk-linux/usr/bin:$ENV{PATH}")

include(${TOOLCHAIN_FILE})

set(CMAKE_C_COMPILER "${EDISON_C_COMPILER}")
set(CMAKE_CXX_COMPILER "${EDISON_CXX_COMPILER}")

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
