# 🔐 FHE Algorithm Benchmarking with OpenFHE

This C++ framework is focused on benchmarking algorithms built with **Fully Homomorphic Encryption (FHE)** using the [OpenFHE](https://github.com/openfheorg/openfhe-development) library. 
It enables accurate, reproducible performance evaluation under configurable parameters, with benchmarking performed only after verifying output correctness to ensure result integrity. 
An optional `examples/` directory includes minimal, self-contained C++ programs (e.g., `matvec`) that demonstrate how to run specific algorithms with configurable parameters.

---
## ✨ Key Features

- ✅ **Matrix-Vector Multiplication** as an example, with an extensible modular structure for adding future algorithms and extensible modular structure for future algorithms
- 🧩 **Configurable CryptoContext** — supports schemes, depth, scale, bootstrapping, and rotation keys
- 🧠 **Bootstrapping Support** — easily toggle settings via `config.yaml`
- 📈 **Comprehensive Benchmarking** — generate runtime CSVs and export performance graphs
- 📦 **YAML-Driven Design** — cleanly separates configuration parameters from code logic

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/pradeep-tii/fhe-algorithms.git
cd fhe-algorithms
```

### 2. Build & Run the Project

```bash
./run.sh
```

### 3. Run the CLI Demo

```bash
./fhe_cli
```

Or, run a matrix-vector multiplication demo:

```bash
./examples/matvec_demo
```

---

## 🧪 Benchmarking

Run benchmarks for various ring dimensions and matrix sizes as defined in `config.yaml`:

```bash
./bench/matvec_bench
```

Plot the results:

```bash
cd bench
./plot.sh
```

This will generate `matvec_plot.png` from the benchmark CSV output.

---

Absolutely — here's a clean, user-friendly **README section** for your Docker-based FHE project that covers:

- Docker setup
- How to build and run
- Using the `run.sh` script for `example`, `bench`, and rebuilds

---

### 📦 Docker-Based Development

This project uses a Docker container for consistent building and running across environments.

#### 🐳 1. Build the Docker Image

Make sure you have Docker installed. From the project root:

```bash
docker compose -f docker-files/docker-compose.yaml up --build
```

This will:
- Build the Docker image using the provided `Dockerfile`
- Mount your source code into the container
- Drop you into a container shell (if not overridden)

---

### 🚀 Using `run.sh` for Building and Running

A helper script `run.sh` is provided to:
- Start the container
- (Optionally) rebuild the project
- Run example and benchmark executables
- Cleanly shut down the container afterward

#### 📄 Usage

```bash
./run.sh [1] [example] [bench]
```

- `1`: (optional) Rebuild the project (removes and reconfigures `build/`)
- `example`: Run the `matvec_demo` example
- `bench`: Run the `matvec_bench` benchmark

Arguments can be given in any order.

#### ✅ Examples

```bash
./run.sh example bench
# → Runs both the example and benchmark (no rebuild)

./run.sh 1 bench
# → Rebuilds the project and runs the benchmark

./run.sh 1 example bench
# → Full rebuild and run both

./run.sh bench
# → Just run the benchmark
```

## 🧪 Dependencies

- [OpenFHE](https://github.com/openfheorg/openfhe-development)
- CMake (>= 3.16)
- A modern C++ compiler (GCC >= 9, Clang >= 10)
- Python 3.8+ for plotting:
    - `pandas`, `matplotlib` (installed via `plot.sh`)

---


## 🙌 Contributing

We welcome contributions and new algorithms! See [CONTRIBUTING.md](./CONTRIBUTING.md).

---