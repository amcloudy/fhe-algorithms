import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import os
import warnings
import re

warnings.filterwarnings("ignore")

# ---------- Paths ----------
script_dir = os.path.dirname(__file__)
csv_path = os.path.join(script_dir, "matvec_ring_bench.csv")
output_path = os.path.join(script_dir, "matvec_plot.png")

# ---------- Load Data ----------
df = pd.read_csv(csv_path)
df.columns = df.columns.str.strip()

expected_cols = {"ring_dim", "matrix_size", "function", "time_ms"}
if not expected_cols.issubset(df.columns):
    raise ValueError(f"CSV must contain columns: {expected_cols}\nFound: {list(df.columns)}")

df["ring_dim"] = df["ring_dim"].astype(str)
df["matrix_size"] = df["matrix_size"].astype(int)

# ---------- Style Setup ----------
function_colors = {
    "helib": "#E69F00",
    "parallel": "#56B4E9",
    "custom": "#009E73",
    "openfhe": "#D55E00",
    "default": "#999999"
}
linestyles = ["solid", "dashed", "dashdot", (0, (1, 1)), (0, (3, 5, 1, 5))]
markers = ["o", "s", "^", "D", "P", "X", "*", "v"]
ring_dims = sorted(df["ring_dim"].unique(), key=lambda x: int(x))
ring_styles = {
    dim: (linestyles[i % len(linestyles)], markers[i % len(markers)])
    for i, dim in enumerate(ring_dims)
}

# ---------- Plot ----------
fig, ax = plt.subplots(figsize=(12, 7))
handles, labels = [], []

for (ring_dim, function), group in df.groupby(["ring_dim", "function"]):
    group_sorted = group.sort_values("matrix_size")
    label = f"{function} @ N={ring_dim}"
    linestyle, marker = ring_styles[ring_dim]
    color = function_colors.get(function, function_colors["default"])

    line, = ax.plot(group_sorted["matrix_size"], group_sorted["time_ms"],
                    label=label,
                    linestyle=linestyle,
                    marker=marker,
                    color=color,
                    markersize=6,
                    linewidth=2)
    handles.append(line)
    labels.append(label)

# ---------- Axes ----------
ax.set_xscale("log", base=2)
ax.set_yscale("log")
ax.set_xlabel("Matrix Size (log2 scale)", fontsize=13)
ax.set_ylabel("Time (ms, log scale)", fontsize=13)
ax.set_title("Matrix-Vector Multiplication Time by Matrix Size, Ring Dimension, and Function", fontsize=15)

# ---------- Custom Tick Labels (with default format for (x,y) coordinate working) ----------
x_ticks = [2**i for i in range(1, 10)]
y_ticks = [10**i for i in range(1, 6)]

ax.set_xticks(x_ticks)
ax.set_xticklabels([f"$2^{{{int(np.log2(x))}}}$" for x in x_ticks])

ax.set_yticks(y_ticks)
ax.set_yticklabels([f"$10^{int(np.log10(y))}$" for y in y_ticks])

# ---------- Grid ----------
ax.grid(False)
for y in y_ticks:
    ax.axhline(y=y, color="gray", linestyle="--", linewidth=0.5, alpha=0.6)
for x in x_ticks:
    ax.axvline(x=x, color="gray", linestyle=":", linewidth=0.5, alpha=0.5)

# ---------- Legend ----------
def parse_label(label):
    match = re.search(r'(\w+)\s*@\s*N=(\d+)', label)
    return (int(match.group(2)), match.group(1)) if match else (float('inf'), 'zzz')

sorted_items = sorted(zip(labels, handles), key=lambda x: parse_label(x[0]))
sorted_labels, sorted_handles = zip(*sorted_items)
ax.legend(sorted_handles, sorted_labels, title="Function @ RingDim", fontsize=9, title_fontsize=10, loc="lower right")

# ---------- Save and Show ----------
plt.tight_layout()
plt.savefig(output_path, dpi=600, bbox_inches="tight")
plt.show()

print(f"✅ MatVec plot saved to: {output_path}")
