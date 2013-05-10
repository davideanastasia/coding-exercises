project(SkipList)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# find_package(Boost COMPONENTS chrono REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    main.cpp
    skiplist.h)

add_executable("${PROJECT_NAME}PerfTest" perftest.cpp skiplist.h)
