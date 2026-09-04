from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from matplotlib.patches import Rectangle


OUTPUT_DIR = Path(__file__).resolve().parent / "figures"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

sns.set_theme(
    style="whitegrid",
    context="paper",
    font_scale=1.15,
    rc={
        "axes.spines.top": False,
        "axes.spines.right": False,
        "figure.dpi": 150,
    },
)


def save_figure(fig: plt.Figure, name: str) -> None:
    fig.savefig(OUTPUT_DIR / f"{name}.png", dpi=300, bbox_inches="tight")
    fig.savefig(OUTPUT_DIR / f"{name}.pdf", bbox_inches="tight")
    plt.close(fig)


def register_field_figure() -> None:
    values = np.full((3, 32), np.nan)
    # Columns are displayed in descending bit order: 31 on the left, 0 on the right.
    pb5_columns = [31 - bit for bit in (23, 22, 21, 20)]
    values[1, pb5_columns] = [0, 0, 0, 0]
    values[2, pb5_columns] = [0, 0, 1, 1]

    fig, ax = plt.subplots(figsize=(12.0, 2.8))
    cmap = sns.color_palette(["#eef2f7", "#2f6f9f"], as_cmap=True)
    sns.heatmap(
        values,
        ax=ax,
        cmap=cmap,
        vmin=0,
        vmax=1,
        cbar=False,
        linewidths=0.35,
        linecolor="white",
        mask=np.isnan(values),
    )
    ax.set_facecolor("#d7dce2")
    ax.set_yticklabels(["Initial", "After clear", "After set"], rotation=0)
    ax.set_xticks(np.arange(32) + 0.5)
    ax.set_xticklabels(range(31, -1, -1), rotation=0, fontsize=7)
    ax.set_xlabel("GPIOB_CRL bit number")
    ax.set_ylabel("")
    ax.set_title("PB5 configuration field: CNF[1:0] MODE[1:0] = 0011")

    left = min(pb5_columns)
    ax.add_patch(Rectangle((left, 0), 4, 3, fill=False, edgecolor="#e07a2d", linewidth=2.2))
    for row, bits in ((1, "0000"), (2, "0011")):
        for col, bit in zip(pb5_columns, bits):
            ax.text(col + 0.5, row + 0.5, bit, ha="center", va="center", fontsize=9,
                    color="white" if bit == "1" else "#263238", fontweight="bold")

    fig.tight_layout()
    save_figure(fig, "task1_register_field")


def interrupt_timing_figure() -> None:
    first_press = 0.5
    second_press = 6.0
    toggle_times = [first_press, 2.0, 3.5, 5.0, second_press]
    led_levels = [0, 1, 0, 1, 0, 0]
    led_times = [0.0, *toggle_times, 8.0]
    timer_levels = [0, 1, 0]
    timer_times = [0.0, first_press, second_press, 8.0]

    fig, axes = plt.subplots(2, 1, figsize=(10.5, 4.8), sharex=True,
                             gridspec_kw={"height_ratios": [1, 1.25]})
    palette = sns.color_palette("deep")

    axes[0].stairs(timer_levels, timer_times, linewidth=2.6, color=palette[0])
    axes[0].set_yticks([0, 1], ["Stopped", "Running"])
    axes[0].set_ylabel("TIM2")

    axes[1].stairs(led_levels, led_times, linewidth=2.6, color=palette[2])
    axes[1].set_yticks([0, 1], ["Off", "On"])
    axes[1].set_ylabel("LD2")
    axes[1].set_xlabel("Time (s)")
    axes[1].set_xlim(0, 8)
    axes[1].set_ylim(-0.18, 1.3)

    for ax in axes:
        ax.axvline(first_press, color="#e07a2d", linestyle="--", linewidth=1.4)
        ax.axvline(second_press, color="#e07a2d", linestyle="--", linewidth=1.4)
    axes[0].text(first_press + 0.08, 1.12, "First B1 press: start", color="#a64f12")
    axes[0].text(second_press + 0.08, 1.12, "Second B1 press: stop", color="#a64f12")

    for event_time in (2.0, 3.5, 5.0):
        axes[1].annotate(
            "1.5 s update",
            xy=(event_time, 0.5),
            xytext=(event_time, 1.18),
            ha="center",
            fontsize=8,
            arrowprops={"arrowstyle": "-|>", "color": "#59636e", "lw": 0.8},
        )

    fig.suptitle("Interrupt-driven LED state sequence", fontsize=13, fontweight="bold")
    fig.tight_layout()
    save_figure(fig, "task4_timing")


if __name__ == "__main__":
    register_field_figure()
    interrupt_timing_figure()
