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

file(GLOB OMNIGRAPH_SRC "src/*.cpp")

add_executable (query_1 first_query.cpp ${OMNIGRAPH_SRC})
target_link_libraries (query_1 kProcessor pthread z)
target_include_directories(query_1 INTERFACE ${kProcessor_INCLUDE_PATH} ${FQFeeder_INCLUDE_PATH})

#add_executable (kmersToSeq kmers_to_seq.cpp)
#add_executable (slice slice.cpp)