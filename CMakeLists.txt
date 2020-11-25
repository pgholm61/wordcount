cmake_minimum_required(VERSION 3.10)

# set the project name
project(wordcount VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

## add the executable
add_executable(wordcount main.cpp wordcount.cpp)


# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")
# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.
add_subdirectory(json)

##target_link_libraries(wordcount PRIVATE nlohmann_json::nlohmann_json crypto ssl boost_regex boost_iostreams boost_context )
target_link_libraries(wordcount PRIVATE nlohmann_json::nlohmann_json crypto ssl boost_system)

##find_package(nlohmann_json 3.2.0 REQUIRED)

