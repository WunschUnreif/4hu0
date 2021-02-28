add_library(MCTS
    GameTree.cpp
    UCBPolicy.cpp
)

target_link_libraries(MCTS ChessMechanism)
target_include_directories(MCTS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(UnitTests)
