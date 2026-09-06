find_package(Boost REQUIRED)

add_library(${CMAKE_PROJECT_NAME}_inet STATIC
        inet.cpp
        inet.hpp
)

target_precompile_headers(${CMAKE_PROJECT_NAME}_inet PRIVATE
        <boost/asio.hpp>
)

target_include_directories(${CMAKE_PROJECT_NAME}_inet
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}_inet
        PRIVATE
        Boost::boost
        $<$<PLATFORM_ID:Windows>:Ws2_32>
        $<$<PLATFORM_ID:Windows>:Mswsock>
        PUBLIC
        argparse::argparse
)