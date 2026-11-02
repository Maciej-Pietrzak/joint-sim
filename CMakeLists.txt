cmake_minimum_required(VERSION 3.20)
project(joint_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Biblioteka: cała logika projektu --------------------------------------
add_library(jointsim STATIC
  src/sim/motor_sim.cpp
)
target_include_directories(jointsim PUBLIC include)

# --- Aplikacja -------------------------------------------------------------
add_executable(jointsim_app app/main.cpp)

# --- Testy -----------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Na Windows: GoogleTest ma używać tej samej biblioteki runtime co nasz kod.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(jointsim_tests
  tests/smoke_test.cpp
  tests/interfaces_test.cpp
  tests/motor_sim_test.cpp
)
target_link_libraries(jointsim_tests PRIVATE jointsim GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(jointsim_tests)
