cmake_minimum_required(VERSION 3.10)
project(sffMOBL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

find_package(SFML 2.5 COMPONENTS system window graphics  REQUIRED)

option(ISCREEN "Use interface in game" ON)

if(ISCREEN)
	add_definitions(-DISCREEN)
endif()

add_executable(sffMOBL main.cpp menu.cpp iscr.cpp field.cpp ftools/ffg.cpp ftools/sfmath.cpp )

target_link_libraries(sffMOBL sfml-system sfml-window sfml-graphics )
