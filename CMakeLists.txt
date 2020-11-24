cmake_minimum_required(VERSION 3.10)
##cmake_minimum_required(VERSION 2.0)

# set the project name
project(wordcount VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

## add the executable
add_executable(wordcount main.cpp wordcount.cpp)

## I../includes
#include_directories(.)

target_link_libraries(wordcount crypto ssl boost_regex boost_iostreams boost_context)

find_package(nlohmann_json 3.2.0 REQUIRED)

## creates the variable EXEC and sets it to hello
#set(EXEC wordcount)
## puts all .cpp files inside src to the SOURCES variable
#file(GLOB SOURCES *.cpp)

## compiles the files defined by SOURCES to generate the exec def by EXEC
#add_executable(${EXEC} ${SOURCES})
