cmake_minimum_required(VERSION 3.15)

project(fhe_algorithms LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "arm64-linux" CACHE STRING "")

# Toggle static vs shared linking for OpenFHE
option(BUILD_STATIC "Use static OpenFHE libraries" OFF)

# === Dependencies ===
find_package(OpenFHE REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# Compiler Flags from OpenFHE
set(CMAKE_CXX_FLAGS ${OpenFHE_CXX_FLAGS})

# Include OpenFHE headers
include_directories(
        ${OPENMP_INCLUDES}
        ${OpenFHE_INCLUDE}
        ${OpenFHE_INCLUDE}/third-party/include
        ${OpenFHE_INCLUDE}/core
        ${OpenFHE_INCLUDE}/pke
)

link_directories(
        ${OpenFHE_LIBDIR}
        ${OPENMP_LIBRARIES}
)

if(BUILD_STATIC)
    set(CMAKE_EXE_LINKER_FLAGS "${OpenFHE_EXE_LINKER_FLAGS} -static")
    link_libraries(${OpenFHE_STATIC_LIBRARIES})
else()
    set(CMAKE_EXE_LINKER_FLAGS ${OpenFHE_EXE_LINKER_FLAGS})
    link_libraries(${OpenFHE_SHARED_LIBRARIES})
endif()

# === Subdirectories ===
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(bench)
