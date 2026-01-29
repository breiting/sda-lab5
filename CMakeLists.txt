cmake_minimum_required(VERSION 3.16)
project(observer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include(CTest)
enable_testing()

# ------------------------------------------------------------
# GoogleTest
# ------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Prevent GoogleTest from overriding our compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ------------------------------------------------------------
# Library
# ------------------------------------------------------------
add_library(obslib
	src/AlarmObserver.cpp
	src/DisplayObserver.cpp
	src/Sensor.cpp
	src/TemperatureSensor.cpp
)

target_include_directories(obslib PUBLIC inc)
target_compile_features(obslib PUBLIC cxx_std_17)

add_executable(test_obs
	tests/test_observer.cpp
)

target_link_libraries(test_obs
	obslib
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_obs)
