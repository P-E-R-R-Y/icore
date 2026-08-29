cmake_minimum_required(VERSION 3.14)
project(icore CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# IAppModule fait d'une application un module chargeable : il lui faut le
# contrat commun a tous les modules.
find_package(imodule REQUIRED)

# Header-only : rien a compiler, juste un chemin d'include a propager.
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE imodule)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# Les exemples ne se construisent que si icore est le projet principal.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  foreach(sample cli app)
    add_executable(icore_${sample} examples/${sample}.cpp)
    target_link_libraries(icore_${sample} PRIVATE ${PROJECT_NAME})
  endforeach()
endif()

# ----------- test -----------

find_package(GoogleTest REQUIRED)

enable_testing()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/tests/includes
)

target_link_libraries(${PROJECT_NAME}_tests
	gtest_main
	${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
