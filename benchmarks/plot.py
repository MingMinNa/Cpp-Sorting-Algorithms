import os
import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

BENCHMARKS_FOLDER = os.path.dirname(os.path.abspath(__file__))
RESULTS_FOLDER = os.path.join(BENCHMARKS_FOLDER, "results")
IMAGES_FOLDER = os.path.join(BENCHMARKS_FOLDER, "images")
os.makedirs(IMAGES_FOLDER, exist_ok=True)

FAST_CMP_FILE = os.path.join(RESULTS_FOLDER, "fast-comparison.csv")
SLOW_CMP_FILE = os.path.join(RESULTS_FOLDER, "slow-comparison.csv")
NON_CMP_FILE = os.path.join(RESULTS_FOLDER, "non-comparison.csv")

ALGO_COLORS = [
    "#4C72B0",  # blue
    "#DD8452",  # orange
    "#55A868",  # green
    "#C44E52",  # red
    "#8172B3",  # purple
    "#937860",  # brown
    "#DA8BC3",  # pink
    "#8C8C8C",  # gray
    "#CCB974",  # tan/yellow
    "#64B5CD",  # light blue
    "#B8860B",  # dark goldenrod
    "#1F77B4",  # steel blue
    "#FF7F0E",  # bright orange
    "#2CA02C",  # bright green
    "#D62728",  # bright red
    "#9467BD",  # medium purple
    "#556B2F",  # dark olive
    "#B5651D",  # rust
    "#4B0082",  # indigo
    "#008080",  # teal
    "#C71585",  # medium violet red
    "#800000",  # maroon
    "#000080",  # navy
    "#FF6347",  # tomato
    "#20B2AA",  # light sea green
    "#9932CC",  # dark orchid
    "#5F9EA0",  # cadet blue
    "#A0522D",  # sienna
    "#6B8E23",  # olive drab
    "#DC143C",  # crimson
    "#00CED1",  # dark turquoise
    "#8B4513",  # saddle brown
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
        help="Input size to plot for the bar chart (default: largest size in CSV)",
    )

    parser.add_argument(
        "--mode",
        choices=["bar", "trend", "both"],
        default="both",
        help="bar = single-size ranking, trend = log-log growth across sizes, both = default",
    )

    return parser.parse_args()

def build_color_map(dfs: list[pd.DataFrame]) -> dict:

    all_algos = set()
    for df in dfs:
        all_algos.update(df["Algo"].str.replace(" Sort", "").unique())
    return {
        algo: ALGO_COLORS[i % len(ALGO_COLORS)]
        for i, algo in enumerate(sorted(all_algos))
    }

def best_time_unit(max_ns: float):
    if max_ns >= 1000000000:
        return 1000000000, "s"
    if max_ns >= 1000000:
        return 1000000, "ms"
    if max_ns >= 1000:
        return 1000, "μs"
    return 1, "ns"

def plot_bar(
    filename: str,
    df: pd.DataFrame,
    data_order: str,
    size: int,
    color_map: dict,
    log_y: bool,
) -> None:

    subset = df[(df["DataOrder"] == data_order) & (df["Size"] == size)].copy()
    if subset.empty:
        return

    subset["Algo"] = subset["Algo"].str.replace(" Sort", "")
    algos = sorted(
        subset["Algo"].unique(),
        key=lambda a: subset.loc[subset["Algo"] == a, "Time(ns)"].values[0],
    )

    unit_div, unit_label = best_time_unit(subset["Time(ns)"].max())
    times = [
        subset.loc[subset["Algo"] == a, "Time(ns)"].values[0] / unit_div for a in algos
    ]
    colors = [color_map[a] for a in algos]

    x_pos = np.arange(len(algos))
    bar_width = 0.6

    fig, ax = plt.subplots(figsize=(10, 5.5))
    fig.patch.set_facecolor("#F8F9FA")
    ax.set_facecolor("#F8F9FA")

    bars = ax.bar(x_pos, times, width=bar_width, color=colors, alpha=0.88)

    if log_y:
        ax.set_yscale("log")
        ax.bar_label(bars, fmt="%.2f", padding=3, fontsize=9)
    else:
        ax.bar_label(bars, fmt="%.1f", padding=3, fontsize=9)

    ax.set_xticks(x_pos)
    ax.set_xticklabels(algos, fontsize=10, rotation=30, ha="right")

    ax.yaxis.grid(
        True, which="both", linestyle="--", linewidth=0.6, alpha=0.5, color="#CCCCCC"
    )
    ax.set_axisbelow(True)
    ax.spines[["top", "right"]].set_visible(False)
    ax.spines[["left", "bottom"]].set_color("#CCCCCC")

    ax.set_xlabel("Algorithm", fontsize=11, labelpad=8)
    ax.set_ylabel(
        f"Time ({unit_label}){' (log scale)' if log_y else ''}", fontsize=11, labelpad=8
    )
    ax.set_title(
        f"{os.path.basename(filename)} {data_order} (size={size})",
        fontsize=13,
        fontweight="bold",
        pad=14,
    )

    plt.tight_layout()
    save_path = f"{filename}_bar_{data_order}_{size}.png"
    fig.savefig(save_path, dpi=150, bbox_inches="tight")
    plt.close(fig)

    print(f"[Output] Saved to {save_path}")

def plot_trend(
    filename: str,
    df: pd.DataFrame,
    data_order: str,
    color_map: dict,
    log_y: bool = True,
) -> None:

    subset = df[df["DataOrder"] == data_order].copy()
    if subset.empty:
        return

    subset["Algo"] = subset["Algo"].str.replace(" Sort", "")
    unit_div, unit_label = best_time_unit(subset["Time(ns)"].max())
    subset["Time"] = subset["Time(ns)"] / unit_div

    fig, ax = plt.subplots(figsize=(10, 6))
    fig.patch.set_facecolor("#F8F9FA")
    ax.set_facecolor("#F8F9FA")

    for algo in sorted(subset["Algo"].unique()):
        algo_df = subset[subset["Algo"] == algo].sort_values("Size")
        ax.plot(
            algo_df["Size"],
            algo_df["Time"],
            marker="o",
            markersize=4,
            linewidth=1.8,
            label=algo,
            color=color_map[algo],
            alpha=0.9,
        )

    ax.set_xscale("log", base=2)
    if log_y:
        ax.set_yscale("log")

    ax.grid(
        True, which="both", linestyle="--", linewidth=0.6, alpha=0.4, color="#CCCCCC"
    )
    ax.set_axisbelow(True)
    ax.spines[["top", "right"]].set_visible(False)
    ax.spines[["left", "bottom"]].set_color("#CCCCCC")

    ax.set_xlabel("Input size (log scale)", fontsize=11, labelpad=8)
    ax.set_ylabel(
        f"Time ({unit_label}){' (log scale)' if log_y else ''}", fontsize=11, labelpad=8
    )
    ax.set_title(
        f"{os.path.basename(filename)} {data_order} (growth trend)",
        fontsize=13,
        fontweight="bold",
        pad=14,
    )
    ax.legend(fontsize=8, ncol=2, frameon=False, loc="upper left")

    plt.tight_layout()
    save_path = f"{filename}_trend_{data_order}.png"
    fig.savefig(save_path, dpi=150, bbox_inches="tight")
    plt.close(fig)

    print(f"[Output] Saved to {save_path}")

def main() -> None:
    args = parse_args()

    fast_cmp_df = pd.read_csv(FAST_CMP_FILE)
    slow_cmp_df = pd.read_csv(SLOW_CMP_FILE)
    non_cmp_df = pd.read_csv(NON_CMP_FILE)

    # Build one global color map so e.g. "Quick Sort" is always the same
    # color whether it shows up in the fast, slow, or non chart.
    color_map = build_color_map([fast_cmp_df, slow_cmp_df, non_cmp_df])

    datasets = {
        "fast": ("fast-comp", fast_cmp_df),
        "slow": ("slow-comp", slow_cmp_df),
        "non": ("non-comp", non_cmp_df),
    }

    if args.comp == "all":
        selected = datasets.items()
    else:
        selected = [(args.comp, datasets[args.comp])]

    for comp_key, (filename, df) in selected:
        img_path = os.path.join(IMAGES_FOLDER, filename)

        if args.size is None:
            size = df["Size"].max()
        else:
            size = args.size

        # slow-comparison spans the widest range (O(n^2) algorithms blow up
        # at large sizes) so log y-scale is used there; fast/non stay linear
        # on the bar chart since their spread is much smaller.
        bar_log_y = comp_key == "slow"

        for order in df["DataOrder"].unique():
            if args.mode in ("bar", "both"):
                plot_bar(img_path, df, order, size, color_map, log_y=bar_log_y)
            if args.mode in ("trend", "both"):
                plot_trend(img_path, df, order, color_map, log_y=True)

if __name__ == "__main__":
    main()
