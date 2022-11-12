cmake_minimum_required(VERSION 3.14)

project(http_server LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(C_EXTENSIONS FALSE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
endif()

if (WRITE_LOG)
	message("${Cyan}LOGGING ENABLED, COMPILING SERVER AND CLIENT APP${ColourReset}")
	add_definitions(-DWRITE_LOG)
endif()

if (TESTMODE) 
	message("${Cyan}TEST ARE ENABLED, SERVER ALL WILL NOW RUN${ColourReset}")
	add_definitions(-DTESTMODE)
endif()


set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE) 
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -g")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(SOURCES_ROOT_DIR ${CMAKE_SOURCE_DIR}/sources/INCLUDE)

add_subdirectory(sources/accounts)
add_subdirectory(sources/logging)
add_subdirectory(sources/misc)
add_subdirectory(sources/block_data)
add_subdirectory(sources/hashing)
add_subdirectory(sources/serdes)
add_subdirectory(sources/tests)
add_subdirectory(sources/networking)
add_subdirectory(sources/client)
add_subdirectory(sources/main)

