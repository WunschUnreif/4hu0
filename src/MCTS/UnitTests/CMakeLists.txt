add_executable(ut-mcts-1 test-1.cpp)
target_link_libraries(ut-mcts-1 MCTS ChessMechanism)
target_include_directories(ut-mcts-1 PUBLIC "./" "../")

