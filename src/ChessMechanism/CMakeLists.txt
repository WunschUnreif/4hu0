add_library(ChessMechanism
    Chessman.cpp
    Chessboard.cpp
    ChessMove.cpp
    Configuration.cpp
    Game.cpp
)

target_include_directories(ChessMechanism PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(UnitTests)
