cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(4hu0)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 14)


find_package(Torch REQUIRED PATHS ../third/libtorch/)

add_executable(main main.cpp)
target_link_libraries(main ${TORCH_LIBRARIES})
set_property(TARGET main PROPERTY CXX_STANDARD 14)

add_executable(xboard-test
    "XBoardComm/xboard.cpp"
)
set_property(TARGET xboard-test PROPERTY CXX_STANDARD 14)

add_subdirectory(ChessMechanism)
