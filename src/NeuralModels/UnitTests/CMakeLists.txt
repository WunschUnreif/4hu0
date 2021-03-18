add_executable(ut-nnmodel-1 test-1.cpp)
target_link_libraries(ut-nnmodel-1 NeuralModel ChessMechanism)
target_include_directories(ut-nnmodel-1 PUBLIC "./" "../")

