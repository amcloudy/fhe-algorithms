import os
import warnings
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import FuncFormatter

# ---------- Configuration ----------
CSV_FILENAME = "matvec_ring_bench.csv"
OUTPUT_PNG = "matvec_equal_linear.png"
COLORS = {
    "helib": "#E69F00",
    "parallel": "#56B4E9",
    "custom": "#009E73",
    "openfhe": "#D55E00"
}
LINESTYLES = ["solid", "dashed", "dashdot", (0, (1, 1)), (0, (3, 5, 1, 5))]
MARKERS = ["o", "s", "^", "D", "P", "X", "*", "v"]
REQUIRED_COLUMNS = {"ring_dim", "matrix_size", "function", "time_ms"}

# ---------- Setup ----------
warnings.filterwarnings("ignore")
HERE = os.path.dirname(__file__)
CSV_PATH = os.path.join(HERE, CSV_FILENAME)
OUTPUT_PATH = os.path.join(HERE, OUTPUT_PNG)

# ---------- Load Data ----------
def load_and_prepare_data(csv_path):
    df = pd.read_csv(csv_path)
    df.columns = df.columns.str.strip()

    if not REQUIRED_COLUMNS.issubset(df.columns):
        raise ValueError(f"CSV must contain columns {REQUIRED_COLUMNS}")

    df["ring_dim"] = df["ring_dim"].astype(str)
    df["matrix_size"] = df["matrix_size"].astype(int)
    df["time_s"] = df["time_ms"].astype(float) / 1000.0
    return df

# ---------- Style Mapping ----------
def get_style_map(unique_keys):
    return {
        key: (LINESTYLES[i % len(LINESTYLES)], MARKERS[i % len(MARKERS)])
        for i, key in enumerate(unique_keys)
    }

# ---------- Plotting ----------
def plot_equal_linear(df, output_path):
    fig, ax = plt.subplots(figsize=(9, 9))  # Square canvas

    ring_dims = sorted(df["ring_dim"].unique(), key=int)
    style_map = get_style_map(ring_dims)

    for (ring_dim, func), group in df.groupby(["ring_dim", "function"]):
        group = group.sort_values("matrix_size")
        linestyle, marker = style_map[ring_dim]
        ax.plot(
            group["matrix_size"], group["time_s"],
            label=f"{func} @ N={ring_dim}",
            linestyle=linestyle,
            marker=marker,
            linewidth=2,
            color=COLORS.get(func, "#999999"),
            markersize=6
        )

    # Equal linear axis scaling
    max_val = int(max(df["matrix_size"].max(), df["time_s"].max()) // 100 + 1) * 100
    ticks = np.arange(0, max_val + 100, 100)
    ax.set_xlim(0, max_val)
    ax.set_ylim(0, max_val)


    # ax.set_xticks(ticks)
    # ax.set_yticks(ticks)
    # ax.xaxis.set_major_formatter(FuncFormatter(lambda x, _: f"{int(x)}"))
    # ax.yaxis.set_major_formatter(FuncFormatter(lambda y, _: f"{int(y)}"))
    # ax.grid(which="major", linestyle="--", linewidth=0.5, alpha=0.7)

    # Labels and legend
    ax.set_xlabel("Matrix Size", fontsize=13)
    ax.set_ylabel("Time (s)", fontsize=13)
    ax.set_title(
        "Matrix–Vector Multiplication (linear, equal axes, 100‑unit grid)",
        fontsize=15
    )
    ax.legend(title="Function@RingDim", loc="upper left")
    ax.format_coord = lambda x, y: f"(x,y)=({x:.0f},{y:.0f})"

    # Save and show
    plt.tight_layout()
    plt.savefig(output_path, dpi=300, bbox_inches="tight")
    plt.show()
    print(f"✅ Plot saved → {output_path}")

# ---------- Main ----------
if __name__ == "__main__":
    df = load_and_prepare_data(CSV_PATH)
    plot_equal_linear(df, OUTPUT_PATH)
