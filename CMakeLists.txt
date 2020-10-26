cmake_minimum_required(VERSION 3.18.0)

project(table VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC OR CMAKE_CXX_SIMULATE_ID MATCHES MSVC)
  message("Using MSVC...")
  add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
  add_compile_options(/utf-8)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

include_directories(include)

find_package(GTest CONFIG)

if (GTest_FOUND)
  set (TEST_PROJ ${PROJECT_NAME}_test)
  message("GoogleTest Found, will generate an tester named " ${TEST_PROJ}.)
  enable_testing()
  aux_source_directory(tests DIR_TESTS)
  add_executable(${TEST_PROJ} ${DIR_TESTS})
  set_target_properties(${TEST_PROJ} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin
  ) 
  target_link_libraries(${TEST_PROJ} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
  include(GoogleTest)
  gtest_discover_tests(${TEST_PROJ})
else()
  message("Can't find GoogleTest, please install it if you want to generate a tester.")
endif()