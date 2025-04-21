# 🔐 FHE Algorithm Benchmarking with OpenFHE

This C++ framework provides a lightweight and extensible setup for benchmarking algorithms built with **Fully Homomorphic Encryption (FHE)** using the [OpenFHE](https://github.com/openfheorg/openfhe-development) library. 
It ensures trustworthy results by verifying output correctness before measuring performance, with all parameters configurable via a central `config.yaml`. 
Benchmarks run through an interactive CLI, log results to CSV, and can be visualized using Python scripts. 
The codebase includes minimal C++ examples to demonstrate usage, and Docker support for reproducibility. 
While current benchmarks cover matrix–vector multiplication and polynomial evaluation, the framework is designed for easy extension.

## 📚 Table of Contents

- [Directory Structure](#directory-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Configuration](#configuration)
- [Benchmarking](#benchmarking)
- [Plotting Results](#plotting-results)
- [Examples](#examples)
- [Docker Support](#docker-support)
- [Contributing](#contributing)
- [Get Help](#get-help)

## 📁 Directory Structure

```bash
.
├── CMakeLists.txt
├── config.yaml
├── README.md
├── run.sh
├── bench
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── matvec
│   │   ├── bench.cpp
│   │   ├── matvec_plot.png
│   │   ├── matvec_ring_bench.csv
│   │   ├── plot.py
│   │   └── plot.sh
│   └── polyeval
│       ├── bench.cpp
│       ├── plot.log
│       ├── plot.py
│       ├── plot.sh
│       ├── polyeval_plot.png
│       └── polyeval_ring_bench.csv
├── docker-files
│   ├── docker-compose.yaml
│   └── Dockerfile
├── examples
│   ├── CMakeLists.txt
│   ├── example.cpp
│   ├── matvec_demo.cpp
│   └── polyeval_demo.cpp
└── src
    ├── CMakeLists.txt
    ├── context.cpp
    ├── context.h
    ├── matvec.cpp
    ├── matvec.h
    ├── poly_eval.cpp
    ├── poly_eval.h
    └── utils
        ├── CMakeLists.txt
        ├── config_loader.cpp
        ├── config_loader.h
        ├── helper_utils.cpp
        ├── helper_utils.h
        └── scheme_defs.h

8 directories, 36 files
```

## 🚀 Getting Started

Everything is automated via a single script and Docker. You don’t need to install compilers or dependencies manually.

### Requirements

- Docker & Docker Compose
- Bash-compatible shell (Linux/macOS/WSL)

---

## 🔧 Using `run.sh`

The `run.sh` script builds, runs, and shuts down everything for you.

```bash
./run.sh [1] [example] [bench]
```

### ✅ Available Options

| Option       | Description                                      |
|--------------|--------------------------------------------------|
| `1`          | 🔁 Rebuild the project from scratch              |
| `example`    | ▶️ Run the example/demo programs                 |
| `bench`      | 📊 Launch the benchmark menu                     |

You can pass these in **any order**. The script will:
- Start the Docker container
- Rebuild (if `1` is passed)
- Run examples or benchmarks
- Shut everything down after execution

### 🔧 Example Usages

```bash
# Rebuild and run everything
./run.sh 1 example bench

# Just run benchmarks
./run.sh bench

# Run both examples and benchmarks (no rebuild)
./run.sh example bench

# Just rebuild the project (no execution)
./run.sh 1
```

---

## ⚙️ Configuration

All runtime parameters are controlled by `config.yaml`.

```yaml
matvec:
  depth: 1
  ringDims: [32768, 65536]
  matrixSizes: [512, 1024]
  max_matrix_value: 5.0
  max_vector_value: 3.0
  rotation_indices: [1]

polyeval:
  ringDims: [ 32768, 65536, 131072 ]
  depth: 5
  vectorSizes: [512, 1024, 2048]
  max_vector_value: 3.0
```

You can tweak ring dimension, input sizes, or functions here before running the benchmarks.

---

## 🧮 Benchmarking

Once inside the benchmark menu (`./run.sh bench`), you'll see:

```
🔬 FHE Algorithm Benchmarks
 [1] Matrix–Vector Multiplication
 [2] Polynomial Evaluation
 [0] Exit
Choose an option:
```

Results will be stored in the corresponding `.csv` files for plotting and analysis.

---

## 📊 Plotting Results

Each benchmark folder includes a script to visualize the results using `matplotlib`.

```bash
./bench/matvec/plot.sh

./bench/polyeval/plot.sh

```

This will generate `matvec_plot.png` and `polyeval_plot.png` and you can customize the plotting logic in `plot.py`.

---

## 💡 Examples

To run minimal working examples:

```bash
./run.sh example
```

This will execute demo programs such as:
- `examples/matvec_demo.cpp`
- `examples/polyeval_demo.cpp`

Useful for validating correctness without benchmarking.

---

## 🐳 Docker Support

The project is containerized for consistency across systems.

- Uses `docker-compose.yaml` and `Dockerfile` under `docker-files/`
- The `run.sh` script automatically handles starting and stopping containers

You **don’t need to run Docker commands manually** — just use `./run.sh`.

---

## 🤝 Contributing

Contributions are welcome! You can:
- Add new benchmarks or examples
- Improve plotting or CLI interfaces
- Enhance Docker support

Feel free to open an issue or pull request.

## 📬 Get Help
For questions or bug reports, we encourage you to write an email to pradeep.fhe@gmail.com