"""Draw the mutex interleaving and interrupt-to-task semaphore diagrams."""

from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch
import seaborn as sns


ROOT = Path(__file__).resolve().parent
FIGURES = ROOT / "figures"
FIGURES.mkdir(exist_ok=True)
sns.set_theme(style="white", context="paper", font_scale=1.1)
COLORS = sns.color_palette("colorblind", 3)


def save(figure, name):
    figure.savefig(FIGURES / f"{name}.pdf", bbox_inches="tight")
    figure.savefig(FIGURES / f"{name}.png", dpi=220, bbox_inches="tight")
    plt.close(figure)


fig, axes = plt.subplots(1, 2, figsize=(7.0, 2.8), sharey=True)
for axis in axes:
    for task in range(3):
        axis.hlines(2 - task, 0.5, 6.5, color="#dce1e6", linewidth=1, zorder=0)
    axis.set(xlim=(0.4, 6.6), ylim=(-0.45, 2.6), xticks=range(1, 7),
             yticks=[2, 1, 0], yticklabels=["Task 1", "Task 2", "Task 3"])
    axis.set_xlabel("Operation order (schematic)")
    sns.despine(ax=axis, left=True, bottom=True)

axes[0].set_title("Unprotected updates", weight="bold", pad=12)
axes[1].set_title("Mutex-protected updates", weight="bold", pad=12)
for task, color in enumerate(COLORS):
    y = 2 - task
    for step, label in ((task + 1, "Read 0"), (task + 4, "Write 1")):
        axes[0].scatter(step, y, s=38, color=color, zorder=3)
        axes[0].text(step, y + 0.18, label, ha="center", fontsize=8, color=color)
    left, right = 2 * task + 1, 2 * task + 2
    axes[1].add_patch(FancyBboxPatch(
        (left - 0.3, y - 0.22), 1.6, 0.66, boxstyle="round,pad=0.04",
        facecolor=(*color, 0.1), edgecolor=color, linewidth=1.1))
    for step, label in ((left, f"Read {task}"), (right, f"Write {task + 1}")):
        axes[1].scatter(step, y, s=38, color=color, zorder=3)
        axes[1].text(step, y + 0.18, label, ha="center", fontsize=8, color=color)
fig.subplots_adjust(left=0.09, right=0.98, top=0.83, bottom=0.29, wspace=0.22)
fig.text(0.29, 0.015, "Final counter: 1", ha="center", weight="bold", color="#a33d37")
fig.text(0.78, 0.015, "Final counter: 3", ha="center", weight="bold", color="#24734b")
save(fig, "mutex_interleaving")

fig, axis = plt.subplots(figsize=(7.0, 2.3))
axis.set(xlim=(0, 1), ylim=(0, 1))
axis.axis("off")
for x, name in ((0.08, "UART interrupt"), (0.51, "Binary semaphore"), (0.92, "Worker task")):
    axis.text(x, 0.95, name, ha="center", weight="bold", color="#173b57")
    axis.plot([x, x], [0.1, 0.89], color="#b3bcc5", linestyle="--", linewidth=1)

def arrow(start, end, y, text, color):
    axis.annotate("", xy=(end, y), xytext=(start, y),
                  arrowprops={"arrowstyle": "->", "color": color, "lw": 1.5})
    axis.text((start + end) / 2, y + 0.035, text, ha="center", fontsize=9, color=color)

arrow(0.92, 0.51, 0.77, "Acquire: count = 0", COLORS[0])
axis.text(0.92, 0.65, "Blocked", ha="center", fontsize=9, color="#5a646e")
axis.text(0.08, 0.60, "Frame complete", ha="center", fontsize=9, color=COLORS[1])
arrow(0.08, 0.51, 0.50, "Release completion token", COLORS[1])
arrow(0.51, 0.92, 0.35, "Wake waiting task", COLORS[2])
axis.text(0.92, 0.15, "Acquire token;\nprocess frame", ha="center", fontsize=9, color=COLORS[2])
fig.tight_layout(pad=1.0)
save(fig, "semaphore_handoff")
