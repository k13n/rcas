project (cas-test)

# reference tlx library
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../extern/tlx)

# reference helper classes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(castest
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/insertion_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/interleaver_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/key_encoder_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node0_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/path_matcher_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/prefix_matcher_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/query_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/surrogate_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)
target_link_libraries(castest
  cas
  tlx
)
