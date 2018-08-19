cmake_minimum_required(VERSION 2.8)
PROJECT(wuduo)
AUX_SOURCE_DIRECTORY(./src SRC_DIR)
INCLUDE_DIRECTORIES( include )
SET(EXECUTABLE_OUTPUT_PATH ./bin)
SET(LIBRARY_OUTPUT_PATH ./lib)


# add_subdirectory(./src/base)




ADD_EXECUTABLE(wuduo ${SRC_DIR} src/base/Condition.cpp src/base/Condition.h src/base/Mutex.cpp src/base/Mutex.h src/base/currentThread.h  src/base/Logging.cpp src/base/Logging.h src/Timestamp_unittest.cpp)

find_package(Threads)
target_link_libraries(wuduo ${CMAKE_THREAD_LIBS_INIT})
