cmake_minimum_required(VERSION 4.3)
project(vey LANGUAGES C CXX VERSION 0.5)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_definitions(-DVEY_PROJECT_NAME="${CMAKE_PROJECT_NAME}")
add_definitions(-DVEY_VERSION="${CMAKE_PROJECT_VERSION}")


option(VEY_OPTIMIZE_ON_RELEASE "compiler flag optimizer" OFF)
option(VEY_STATIC_ON_RELEASE "include static libs without dynamic libs" OFF)

if (VEY_OPTIMIZE_ON_RELEASE)
    add_compile_options(
            $<$<CONFIG:Release>:-O3>
    )
    add_link_options(
            $<$<CONFIG:Release>:-s>
            $<$<CONFIG:Release>:-Wl,--exclude-all-symbols>
    )
endif ()

if (VEY_STATIC_ON_RELEASE)
    add_link_options(
            $<$<CONFIG:Release>:-static>
    )
endif ()


add_subdirectory(src vey)
add_subdirectory(libs)