cmake_minimum_required(VERSION 3.17)
project(hAM-WL)

set(CMAKE_CXX_STANDARD 20)

include_directories(lib)

file(GLOB LIB_SOURCES lib/*.cpp)
file(GLOB LIB_HEADERS lib/*.h)
add_library(hAM-WL-lib ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(single-cell-native Single_cell_native_main.cpp)
target_link_libraries(single-cell-native hAM-WL-lib)

add_executable(tissue-native Tissue_native_main.cpp)
target_link_libraries(tissue-native hAM-WL-lib)

add_executable(data-convert-binary-to-vtk-text-tissue Data_convert_binary_to_vtk_text_tissue.cpp)
target_link_libraries(data-convert-binary-to-vtk-text-tissue hAM-WL-lib)

IF (WIN32)
    #set(gtkmm_DIR ${PROJECT_SOURCE_DIR})
    #find_package(gtkmm)

    # TODO link to PLPLOT on windows

    set(GTKMM_INCLUDE_DIR "")
    set(PLPLOT_INCLUDE_DIR "")
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(GTKMM gtkmm-3.0)

    link_directories(${GTKMM_LIBRARY_DIRS})
    include_directories(${GTKMM_INCLUDE_DIRS})

    pkg_check_modules(PLPLOT plplot-c++)
ENDIF()

add_executable(hAMster-WheeL gui/gui-main.cpp gui/MainWindow.cpp gui/MainWindow.h gui/util.h)
target_include_directories(hAMster-WheeL PUBLIC ${GTKMM_INCLUDE_DIR} ${PLPLOT_INCLUDE_DIR})
target_link_libraries(hAMster-WheeL ${GTKMM_LIBRARIES} ${PLPLOT_LIBRARIES})
IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(hAMster-WheeL PUBLIC "-fconcepts")
ENDIF()
