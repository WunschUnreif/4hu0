add_library(NeuralModel 
    NeuralModel.cpp
    DataLoader.cpp
    SelfPlay/AgentPureMCTS.cpp
    SelfPlay/SelfPlayData.cpp
    SelfPlay/Agent.cpp
)

target_link_libraries(NeuralModel ChessMechanism MCTS ${TORCH_LIBRARIES})
target_include_directories(NeuralModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(selfplay-datagen SelfPlay/DataGen.cpp)
target_link_libraries(selfplay-datagen NeuralModel)

add_subdirectory(UnitTests)
