add_executable(ut-mech-1 test-1.cpp)
target_link_libraries(ut-mech-1 ChessMechanism)
target_include_directories(ut-mech-1 PUBLIC "./" "../")

add_executable(ut-mech-2-random-game test-2.cpp)
target_link_libraries(ut-mech-2-random-game ChessMechanism)
target_include_directories(ut-mech-2-random-game PUBLIC "./" "../")
