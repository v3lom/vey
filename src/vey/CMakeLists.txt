add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
        argparse::argparse
        ${CMAKE_PROJECT_NAME}_inet
        ${CMAKE_PROJECT_NAME}_injec
)