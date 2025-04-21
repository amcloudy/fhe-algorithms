# === bench/CMakeLists.txt ===

# Recursively find all benchmark .cpp files
file(GLOB_RECURSE BENCHMARK_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Exclude anything in the Python virtual environment directory
list(FILTER BENCHMARK_SOURCES EXCLUDE REGEX ".*/\\.venv/.*")

# Define the main benchmark binary
add_executable(bench_main ${BENCHMARK_SOURCES})

# Link to fhe_algorithms (your core lib) and yaml-cpp
target_link_libraries(bench_main
        PRIVATE
        fhe_algorithms
        yaml-cpp::yaml-cpp
)

# Inject the path to the config file (used by ConfigLoader)
target_compile_definitions(bench_main
        PRIVATE CONFIG_PATH="${CMAKE_SOURCE_DIR}/config.yaml"
)

# Optional: turn off some warnings you don't care about
# target_compile_options(bench_main PRIVATE -Wno-sign-compare)
