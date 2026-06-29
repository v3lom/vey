cmake_minimum_required(VERSION 4.1)
project(vey)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(${PROJECT_NAME}
        src/main.cpp
        src/wsock.cpp
        src/vey.hpp
        src/wsock.hpp
)

if (WIN32)
target_link_libraries(${PROJECT_NAME} PRIVATE
        Ws2_32
)
endif ()