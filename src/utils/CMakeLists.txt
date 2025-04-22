add_library(fhe_utils
        config_loader.cpp
        helper_utils.cpp
        serialization_utils.cpp
)

target_include_directories(fhe_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fhe_utils PUBLIC yaml-cpp::yaml-cpp)

