import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
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

# Validate expected columns
expected_cols = {"ring_dim", "matrix_size", "time_ms"}
if not expected_cols.issubset(df.columns):
    raise ValueError(f"CSV must contain columns: {expected_cols}\nFound: {list(df.columns)}")

df["ring_dim"] = df["ring_dim"].astype(str)
df["matrix_size"] = df["matrix_size"].astype(int)

# ---------- Style Setup ----------
colors = [
    "#E69F00", "#56B4E9", "#009E73", "#D55E00",
    "#CC79A7", "#F0E442", "#0072B2", "#999999"
]
linestyles = ["solid", "dashed", "dashdot", (0, (1, 1)), (0, (3, 5, 1, 5))]
markers = ["o", "s", "^", "D", "P", "X", "*", "v"]

ring_dims = sorted(df["ring_dim"].unique(), key=lambda x: int(x))
style_map = {
    dim: (colors[i % len(colors)],
          linestyles[i % len(linestyles)],
          markers[i % len(markers)])
    for i, dim in enumerate(ring_dims)
}

# ---------- Plot ----------
plt.figure(figsize=(12, 7))

handles = []
labels = []

for ring_dim in ring_dims:
    group = df[df["ring_dim"] == ring_dim].sort_values("matrix_size")
    color, linestyle, marker = style_map[ring_dim]
    line, = plt.plot(group["matrix_size"], group["time_ms"],
                     label=f"ring_dim = {ring_dim}",
                     color=color,
                     linestyle=linestyle,
                     marker=marker,
                     markersize=6,
                     linewidth=2)
    handles.append(line)
    labels.append(f"ring_dim = {ring_dim}")

# ---------- Axes & Labels ----------
plt.xlabel("Matrix Size (log2 scale)", fontsize=13)
plt.ylabel("Time (ms, log scale)", fontsize=13)
plt.title("Matrix-Vector Multiplication Time by Matrix Size and Ring Dimension", fontsize=15)

plt.xscale("log", base=2)
plt.yscale("log")

def is_power_of_two(n):
    return n > 0 and (n & (n - 1)) == 0

def log_label(x, pos):
    try:
        val = int(x)
        if is_power_of_two(val):
            return f"$2^{{{val.bit_length() - 1}}}$"
        else:
            return f"{val}"
    except Exception:
        return str(x)

plt.gca().xaxis.set_major_formatter(ticker.FuncFormatter(log_label))

# ---------- Clean Gridlines ----------
plt.grid(False)

# Horizontal gridlines at powers of 10
for y in [1e2, 1e3, 1e4, 1e5]:
    plt.axhline(y=y, color="gray", linestyle="--", linewidth=0.5, alpha=0.6)

# Vertical gridlines at 2^2, 2^4, 2^6, 2^8
for x in [4, 16, 64, 256]:
    plt.axvline(x=x, color="gray", linestyle=":", linewidth=0.5, alpha=0.6)

# ---------- Sorted Legend ----------
sorted_items = sorted(zip(labels, handles), key=lambda x: int(re.search(r'\d+', x[0]).group()))
sorted_labels, sorted_handles = zip(*sorted_items)

plt.legend(sorted_handles, sorted_labels, title="Ring Dim", fontsize=9, title_fontsize=10, loc="lower right")
plt.tight_layout()
plt.savefig(output_path, dpi=600, bbox_inches="tight")
plt.show()

print(f"✅ MatVec plot saved to: {output_path}")
