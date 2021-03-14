add_executable(xboard-adversary 
    xboard.cpp
)
target_link_libraries(xboard-adversary MCTS ChessMechanism)

