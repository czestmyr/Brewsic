project(brewsic)
cmake_minimum_required(VERSION 2.8)

find_package(SDL)
if(NOT SDL_FOUND)
	message("SDL library not found, quitting cmake...")
	return()
endif(NOT SDL_FOUND)

find_package(SDL_ttf)
if(NOT SDLTTF_FOUND)
	message("SDL_ttf library not found, quitting cmake...")
	return()
endif(NOT SDLTTF_FOUND)

find_package(SDL_image)
if(NOT SDLIMAGE_FOUND)
	message("SDL_image library not found, quitting cmake...")
	return()
endif(NOT SDLIMAGE_FOUND)

file(GLOB_RECURSE CXX_SOURCES
	${CMAKE_SOURCE_DIR}/common/*.cpp
	${CMAKE_SOURCE_DIR}/draw/*.cpp
	${CMAKE_SOURCE_DIR}/filters/*.cpp
	${CMAKE_SOURCE_DIR}/generators/*.cpp
	${CMAKE_SOURCE_DIR}/gui/*.cpp
	${CMAKE_SOURCE_DIR}/synths/*.cpp
	${CMAKE_SOURCE_DIR}/mixer/*.cpp
	${CMAKE_SOURCE_DIR}/main.cpp
	${CMAKE_SOURCE_DIR}/maintest.cpp
)

file(GLOB_RECURSE IMG_DATA
	${CMAKE_SOURCE_DIR}/data/images/*
)

file(GLOB_RECURSE FONT_DATA
	${CMAKE_SOURCE_DIR}/data/*.ttf
)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLTTF_INCLUDE_DIR})
include_directories(${SDLIMAGE_INCLUDE_DIR})

add_executable(
	brewsic
	${CXX_SOURCES}
	${C_SOURCES}
)

target_link_libraries(
	brewsic
	${SDL_LIBRARY}
	${SDLTTF_LIBRARY}
	${SDLIMAGE_LIBRARY}
)

install(FILES ${IMG_DATA} DESTINATION ${CMAKE_BINARY_DIR}/data/images)
install(FILES ${FONT_DATA} DESTINATION ${CMAKE_BINARY_DIR}/data)

