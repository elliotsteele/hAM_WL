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