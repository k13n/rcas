cmake_minimum_required (VERSION 3.0)
project (app)

# enable compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
# set(CMAKE_CXX_CLANG_TIDY clang-tidy)


# set the C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# reference tlx library
add_subdirectory(extern/tlx)

# # show make output
# set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(src)
add_subdirectory(tests)

# app
add_executable(app ${CMAKE_CURRENT_SOURCE_DIR}/apps/app.cpp)
target_link_libraries(app cas tlx)


# benchmarks
add_executable(bm_structure ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/structure.cpp)
target_link_libraries(bm_structure cas tlx)

add_executable(bm_query_runtime ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/query_runtime.cpp)
target_link_libraries(bm_query_runtime cas tlx)

add_executable(bm_space ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/space.cpp)
target_link_libraries(bm_space cas tlx)

add_executable(bm_skew ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/skew.cpp)
target_link_libraries(bm_skew cas tlx)

add_executable(bm_scalability ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/scalability.cpp)
target_link_libraries(bm_scalability cas tlx)

add_executable(bm_cost_model ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/cost_model.cpp)
target_link_libraries(bm_cost_model cas tlx)
