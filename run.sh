#!/bin/bash

# --- Flags ---
REBUILD=false
RUN_EXAMPLE=false
RUN_BENCH=false

# --- Parse args ---
for arg in "$@"; do
  case $arg in
    1)
      REBUILD=true
      ;;
    example)
      RUN_EXAMPLE=true
      ;;
    bench)
      RUN_BENCH=true
      ;;
    *)
      echo "⚠️ Unknown argument: $arg"
      ;;
  esac
done

# --- Start container ---
docker compose -f docker-files/docker-compose.yaml up --build -d

# --- Interactive Terminal Check ---
DOCKER_INTERACTIVE=""
if [ -t 1 ]; then
  DOCKER_INTERACTIVE="-it"
fi

# --- Shared Build Command ---
BUILD_COMMAND="
  cd /fhe-algorithms &&
  "

if $REBUILD; then
  BUILD_COMMAND+="
    echo '🧹 Rebuilding from scratch...' &&
    rm -rf build &&
    cmake -S . -B build \
      -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
      -DCMAKE_BUILD_TYPE=Release &&
  "
fi

BUILD_COMMAND+="
  echo '🛠️ Building the project...' &&
  cmake --build build --parallel;
"

# --- Run Examples ---
if $RUN_EXAMPLE; then
  docker exec $DOCKER_INTERACTIVE fhe-algorithms-dev bash -c "
    $BUILD_COMMAND
    echo '▶️ Running fhe_examples...' &&
    ./build/examples/fhe_examples"
fi

# --- Run Benchmarks ---
if $RUN_BENCH; then
  docker exec $DOCKER_INTERACTIVE fhe-algorithms-dev bash -c "
    $BUILD_COMMAND
    echo '▶️ Launching benchmark menu...' &&
    ./build/bench/bench_main"
fi

 --- Shut down ---
docker compose -f docker-files/docker-compose.yaml down
