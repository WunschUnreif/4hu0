add_executable(ut-nnmodel-1 test-1.cpp)
target_link_libraries(ut-nnmodel-1 NeuralModel)
target_include_directories(ut-nnmodel-1 PUBLIC "./" "../")

add_executable(ut-nnmodel-2 test-2.cpp)
target_link_libraries(ut-nnmodel-2 NeuralModel)
target_include_directories(ut-nnmodel-2 PUBLIC "./" "../")

add_executable(ut-nnmodel-3-self-play test-3.cpp)
target_link_libraries(ut-nnmodel-3-self-play NeuralModel)
target_include_directories(ut-nnmodel-3-self-play PUBLIC "./" "../")

add_executable(ut-nnmodel-4-data-loader-train test-4.cpp)
target_link_libraries(ut-nnmodel-4-data-loader-train NeuralModel)
target_include_directories(ut-nnmodel-4-data-loader-train PUBLIC "./" "../")

add_executable(ut-nnmodel-5-neural-selfplay test-5.cpp)
target_link_libraries(ut-nnmodel-5-neural-selfplay NeuralModel)
target_include_directories(ut-nnmodel-5-neural-selfplay PUBLIC "./" "../")

