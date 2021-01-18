cmake_minimum_required(VERSION 3.17)
project(CODE)

set(CMAKE_CXX_STANDARD 20)

include_directories(lib)

add_executable(CODE
        lib/Arguments.cpp
        lib/Arguments.h
        lib/CRU.h
        lib/Initialisation.cpp
        lib/Initialisation.h
        lib/Model.cpp
        lib/Model.h
        lib/Model_hAM_CRN.cpp
        lib/Model_hAM_GB.cpp
        lib/Model_hAM_MT.cpp
        lib/Model_hAM_NG.cpp
        lib/Model_hAM_WL.cpp
        lib/Model_minimal.cpp
        lib/Model_TEMPLATE.cpp
        lib/Outputs.cpp
        lib/Outputs.h
        lib/Read_write_state.cpp
        lib/Read_write_state.h
        lib/Spatial_coupling.cpp
        lib/Spatial_coupling.h
        lib/Structs.h
        lib/Tissue.cpp
        lib/Tissue.h
        Single_cell_native_main.cpp)
