add_executable(fhe_examples
        example.cpp
        matvec_demo.cpp
        polyeval_demo.cpp
)

target_include_directories(fhe_examples PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/utils
)

target_link_libraries(fhe_examples PRIVATE fhe_algorithms yaml-cpp::yaml-cpp)
target_compile_definitions(fhe_examples PRIVATE CONFIG_PATH="${CMAKE_SOURCE_DIR}/config.yaml")
