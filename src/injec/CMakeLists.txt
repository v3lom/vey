add_library(${CMAKE_PROJECT_NAME}_injec STATIC
        injec.cpp
        injec.hpp
        AOBScanner.hpp
        WsaCleanup.cpp
        WsaCleanup.hpp
)

target_include_directories(${CMAKE_PROJECT_NAME}_injec
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}_injec
        PRIVATE
        $<$<PLATFORM_ID:Windows>:Ws2_32>
        $<$<PLATFORM_ID:Windows>:Mswsock>
        PUBLIC
        argparse::argparse
)