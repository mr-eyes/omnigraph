cmake_minimum_required (VERSION 3.4)
project (omnigraph C CXX)
set(default_build_type "Release")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -fPIC -lgomp -lrt -fopenmp -W -Wall -pedantic -O3 -Ofast -march=native")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")


# ----------------------------------------------------------------------------
# kProcessor Setup
# ----------------------------------------------------------------------------

add_subdirectory(lib/kProcessor)
set(kProcessor_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/lib/kProcessor/include")

# Add include directories.
include_directories("${PROJECT_SOURCE_DIR}/lib/kProcessor/ThirdParty/MQF/include")
include_directories (${kProcessor_INCLUDE_PATH})


# ----------------------------------------------------------------------------
# END kProcessor Setup
# ----------------------------------------------------------------------------


include_directories("${PROJECT_SOURCE_DIR}/includes")
include_directories("${PROJECT_SOURCE_DIR}/lib/progressbar")
include_directories("${PROJECT_SOURCE_DIR}/lib/inih")
include_directories(${PROJECT_SOURCE_DIR}/lib/sqlite3pp/)
include_directories(lib/gzstream)


add_executable (query_1 first_query.cpp src/omnigraph.cpp src/sqliteManager.cpp)
target_link_libraries (query_1 kProcessor pthread z sqlite3)
target_include_directories(query_1 INTERFACE ${kProcessor_INCLUDE_PATH})

add_executable (query_2 second_query.cpp src/omnigraph.cpp src/sqliteManager.cpp)
target_link_libraries (query_2 kProcessor pthread z sqlite3)
target_include_directories(query_2 INTERFACE ${kProcessor_INCLUDE_PATH})

#add_executable (singleQuery single_query.cpp src/omnigraph.cpp)
#target_link_libraries (singleQuery kProcessor pthread z sqlite3)
#target_include_directories(singleQuery INTERFACE ${kProcessor_INCLUDE_PATH})

add_executable (cDBG_labeling cDBG_labeling.cpp src/omnigraph.cpp)
target_link_libraries (cDBG_labeling kProcessor pthread z)
target_include_directories(cDBG_labeling INTERFACE ${kProcessor_INCLUDE_PATH})

#add_executable (cDBG75MappingStats cDBG_labeling.cpp src/omnigraph.cpp)
#target_link_libraries (cDBG75MappingStats kProcessor pthread z)
#target_include_directories(cDBG75MappingStats INTERFACE ${kProcessor_INCLUDE_PATH})