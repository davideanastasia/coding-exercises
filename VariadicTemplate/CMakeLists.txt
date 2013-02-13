FIND_PACKAGE(Boost COMPONENTS chrono system REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11") # -stdlib=libc++

add_executable(Polynomial main.cpp polynomial.h stopwatch.h)
target_link_libraries(Polynomial ${TEST_LIBRARIES} ${Boost_CHRONO_LIBRARY} ${Boost_SYSTEM_LIBRARY})
