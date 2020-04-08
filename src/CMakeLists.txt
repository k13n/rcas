project (cas)

# reference tlx library
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../extern/tlx)

add_library(cas
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/bulk_load.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/index.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/key_decoder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/query_stats.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/search_key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/utils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/binary_key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/cas.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/csv_importer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/insertion.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/insertion_static.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/interleaved_key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/interleaver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/key_encoder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/locator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node0.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node16.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node256.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node4.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/node48.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/path_matcher.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/prefix_matcher.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/query.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/surrogate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/surrogate_cas.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cas/surrogate_path_matcher.cpp
  #
  ${CMAKE_CURRENT_SOURCE_DIR}/casxml/casxml.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/casxml/entry.cpp
  #
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/cost_experiment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/query_experiment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/scalability_experiment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/selectivity_computer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/skew_experiment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/space_experiment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/structure_experiment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/utils.cpp
)
