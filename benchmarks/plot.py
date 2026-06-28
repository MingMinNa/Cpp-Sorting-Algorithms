import os
import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

BENCHMARKS_FOLDER = os.path.dirname(os.path.abspath(__file__))
RESULTS_FOLDER = os.path.join(BENCHMARKS_FOLDER, "results")
IMAGES_FOLDER = os.path.join(BENCHMARKS_FOLDER, "images")
os.makedirs(IMAGES_FOLDER, exist_ok=True)

FAST_CMP_FILE = os.path.join(RESULTS_FOLDER, "fast-comparison.csv")
SLOW_CMP_FILE = os.path.join(RESULTS_FOLDER, "slow-comparison.csv")
NON_CMP_FILE = os.path.join(RESULTS_FOLDER, "non-comparison.csv")

ALGO_COLORS = [
    "#4C72B0",
    "#DD8452",
    "#55A868",
    "#C44E52",
    "#8172B3",
    "#937860",
    "#DA8BC3",
    "#8C8C8C",
    "#CCB974",
    "#64B5CD",
    "#FFEA00",
]

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Plot sorting benchmark results.")

    parser.add_argument(
        "--comp",
        choices=["fast", "slow", "non", "all"],
        default="all",
        help="Comparison type (default: all)",
    )

    parser.add_argument(
        "--size",
        type=int,
        default=None,
        help="Input size to plot (default: largest size in CSV)",
    )

    return parser.parse_args()

def plot(filename: str, df: pd.DataFrame, data_order: str, size: int) -> None:

    subset = df[(df["DataOrder"] == data_order) & (df["Size"] == size)].copy()
    if subset.empty:
        return

    # Convert ns → μs
    subset["Time(μs)"] = subset["Time(ns)"] / 1_000
    subset["Algo"] = subset["Algo"].apply(lambda x: x.replace(" Sort", ""))
    algos = sorted(subset["Algo"].unique())
    n_algos = len(algos)
    x_pos = np.arange(n_algos)
    bar_width = 0.6

    fig, ax = plt.subplots(figsize=(10, 5.5))
    fig.patch.set_facecolor("#F8F9FA")
    ax.set_facecolor("#F8F9FA")

    for i, algo in enumerate(algos):
        time_us = subset.loc[subset["Algo"] == algo, "Time(μs)"].values[0]
        color = ALGO_COLORS[i % len(ALGO_COLORS)]
        ax.bar(x_pos[i], time_us, width=bar_width, label=algo, color=color, alpha=0.88)

    bars = ax.bar(
        x_pos,
        [subset.loc[subset["Algo"] == a, "Time(μs)"].values[0] for a in algos],
        width=bar_width,
        color=[ALGO_COLORS[i % len(ALGO_COLORS)] for i in range(len(algos))],
        alpha=0.88,
    )
    ax.bar_label(bars, fmt="%d", padding=3, fontsize=9)

    ax.set_xticks(x_pos)
    ax.set_xticklabels(algos, fontsize=10)

    ax.yaxis.grid(True, linestyle="--", linewidth=0.6, alpha=0.5, color="#CCCCCC")
    ax.set_axisbelow(True)
    ax.spines[["top", "right"]].set_visible(False)
    ax.spines[["left", "bottom"]].set_color("#CCCCCC")

    ax.set_xlabel("Algorithm", fontsize=11, labelpad=8)
    ax.set_ylabel("Time (μs)", fontsize=11, labelpad=8)
    ax.set_title(
        f"{os.path.basename(filename)} {data_order} (size={size})",
        fontsize=13,
        fontweight="bold",
        pad=14,
    )
    ax.set_xticklabels(algos, fontsize=10, rotation=30, ha="right")

    plt.tight_layout()
    plt.subplots_adjust(bottom=0.2)

    save_path = f"{filename}_{data_order}_{size}.png"
    fig.savefig(save_path, dpi=150, bbox_inches="tight")
    plt.close(fig)

    print(f"[Output] Saved to {save_path}")

def main() -> None:
    args = parse_args()

    fast_cmp_df = pd.read_csv(FAST_CMP_FILE)
    slow_cmp_df = pd.read_csv(SLOW_CMP_FILE)
    non_cmp_df = pd.read_csv(NON_CMP_FILE)

    datasets = {
        "fast": ("fast-comp", fast_cmp_df),
        "slow": ("slow-comp", slow_cmp_df),
        "non": ("non-comp", non_cmp_df),
    }

    if args.comp == "all":
        selected = datasets.items()
    else:
        selected = [(args.comp, datasets[args.comp])]

    for _, (filename, df) in selected:

        if args.size is None:
            size = df["Size"].max()
        else:
            size = args.size

        for order in df["DataOrder"].unique():
            plot(
                os.path.join(IMAGES_FOLDER, filename),
                df,
                order,
                size,
            )

if __name__ == "__main__":
    main()
