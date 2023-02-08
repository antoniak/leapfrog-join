set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "-I ${PROJECT_SOURCE_DIR}/lib/boost/")

find_package(Threads REQUIRED)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

target_link_libraries(${BINARY}_run PRIVATE Threads::Threads -L/${PROJECT_SOURCE_DIR}/lib/boost/libs)