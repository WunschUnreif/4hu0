add_library(NeuralModel 
    NeuralModel.cpp
)

target_link_libraries(NeuralModel ChessMechanism ${TORCH_LIBRARIES})
target_include_directories(NeuralModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(UnitTests)
