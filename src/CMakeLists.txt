add_subdirectory(utils)

add_library(fhe_algorithms
        context.cpp
        matvec.cpp
        poly_eval.cpp
)

target_include_directories(fhe_algorithms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fhe_algorithms PUBLIC fhe_utils yaml-cpp::yaml-cpp)
