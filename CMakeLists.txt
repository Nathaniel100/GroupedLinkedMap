cmake_minimum_required(VERSION 3.10)
project(GroupedLinkedMap)

set(CMAKE_CXX_STANDARD 11)

MESSAGE(${PROJECT_SOURCE_DIR})

set(GroupedLinkedMap_Headers ${PROJECT_SOURCE_DIR}/groupedlinkedmap.h)
add_library(GroupedLinkedMap INTERFACE)
target_include_directories(GroupedLinkedMap INTERFACE ${PROJECT_SOURCE_DIR})
target_sources(GroupedLinkedMap INTERFACE ${GroupedLinkedMap_Headers})

add_subdirectory(test)

