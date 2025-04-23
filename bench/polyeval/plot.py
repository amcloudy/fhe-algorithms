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
csv_path = os.path.join(script_dir, "polyeval_ring_bench.csv")
output_path = os.path.join(script_dir, "polyeval_plot.png")

# ---------- Load Data ----------
df = pd.read_csv(csv_path)
df.columns = df.columns.str.strip()

# Rename if needed
df.rename(columns={"vecSize": "vector_size", "functionType": "function"}, inplace=True)

# Validate
expected_cols = {"ring_dim", "vector_size", "function", "time_ms"}
if not expected_cols.issubset(df.columns):
    raise ValueError(f"CSV must contain columns: {expected_cols}\nFound: {list(df.columns)}")

df["ring_dim"] = df["ring_dim"].astype(str)
df["vector_size"] = df["vector_size"].astype(int)

# ---------- Color & Line Style Setup ----------
function_colors = {
    "openfhe": "#E69F00",
    "custom": "#56B4E9",
    "lagrange": "#009E73",
    "odd_powers": "#D55E00"
}

linestyles = ["solid", "dashed", "dashdot", (0, (1, 1)), (0, (3, 5, 1, 5))]
markers = ["o", "s", "^", "D", "P"]
ring_dims = sorted(df["ring_dim"].unique())
ring_styles = {dim: (linestyles[i % len(linestyles)], markers[i % len(markers)]) for i, dim in enumerate(ring_dims)}

# ---------- Plot ----------
plt.figure(figsize=(12, 7))
plotted_handles = []
plotted_labels = []

for (ring_dim, function), group in df.groupby(["ring_dim", "function"]):
    group_sorted = group.sort_values("vector_size")
    label = f"{function} @ N={ring_dim}"
    linestyle, marker = ring_styles.get(ring_dim, ("solid", "o"))
    color = function_colors.get(function, "gray")

    line, = plt.plot(group_sorted["vector_size"], group_sorted["time_ms"],
                     label=label,
                     color=color,
                     linestyle=linestyle,
                     marker=marker,
                     markersize=6,
                     linewidth=2)
    plotted_handles.append(line)
    plotted_labels.append(label)

# ---------- Axes & Labels ----------
plt.xlabel("Vector Size (log2 scale)", fontsize=13)
plt.ylabel("Time (ms, log scale)", fontsize=13)
plt.title("Polynomial Evaluation Time by Vector Size, Ring Dimension, and Function", fontsize=15)

plt.xscale("log", base=2)
plt.yscale("log")

def is_power_of_two(n):
    return n > 0 and (n & (n - 1)) == 0

def log_label(x, pos):
    try:
        val = int(x)
        return f"$2^{{{val.bit_length() - 1}}}$" if is_power_of_two(val) else f"{val}"
    except Exception:
        return str(x)

plt.gca().xaxis.set_major_formatter(ticker.FuncFormatter(log_label))

# ---------- Y-Ticks: 10^1 to 10^5 ----------
y_decades = [10**i for i in range(1, 6)]
plt.yticks(y_decades, [f"$10^{int(np.log10(y))}$" for y in y_decades])
plt.gca().yaxis.set_major_locator(ticker.FixedLocator(y_decades))

# ---------- Grid ----------
plt.grid(axis="y", which="major", linestyle="--", linewidth=0.5, alpha=0.7)
plt.minorticks_off()

# Vertical gridlines at powers of 2: 2^1 to 2^10
for x in [2**i for i in range(1, 11)]:
    plt.axvline(x=x, color="gray", linestyle=":", linewidth=0.5, alpha=0.5)

# ---------- Legend ----------
def parse_label(label):
    match = re.search(r'(\w+)\s*@\s*N=(\d+)', label)
    if match:
        func = match.group(1)
        n = int(match.group(2))
        return (n, func)
    return (float('inf'), 'zzz')

sorted_pairs = sorted(zip(plotted_labels, plotted_handles), key=lambda x: parse_label(x[0]))
sorted_labels, sorted_handles = zip(*sorted_pairs)

plt.legend(sorted_handles, sorted_labels, title="Function @ RingDim", fontsize=9, title_fontsize=10, loc="lower right")

# ---------- Save ----------
plt.tight_layout()
plt.savefig(output_path, dpi=600, bbox_inches="tight")
plt.show()
print(f"✅ PolyEval Plot saved to: {output_path}")
