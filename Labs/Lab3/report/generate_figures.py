"""Plot measured Task 2 LED transitions and write the interval summary."""

from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns


ROOT = Path(__file__).resolve().parent
data = pd.read_csv(ROOT / "data/led_edges.csv")
data["interval_ms"] = data.groupby("command_ms")["time_s"].diff() * 1000
summary = data.groupby("command_ms")["interval_ms"].agg(["mean", "std", "count"])

sns.set_theme(style="whitegrid", context="paper", font_scale=1.15)
colors = sns.color_palette("colorblind", 3)
fig, axes = plt.subplots(3, 1, figsize=(6.8, 4.6), sharex=True)
for axis, interval, color in zip(axes, (1000, 500, 250), colors):
    trace = data[data.command_ms == interval]
    times = [0, *trace.time_s, trace.time_s.iloc[-1] + interval / 1000]
    levels = [1 - trace.level.iloc[0], *trace.level, trace.level.iloc[-1]]
    axis.step(times, levels, where="post", color=color, linewidth=1.8)
    axis.set(ylim=(-0.2, 1.5), yticks=[0, 1], yticklabels=["Off", "On"])
    axis.text(0.015, 0.9, f"Command: {interval} ms", transform=axis.transAxes,
              ha="left", va="top", color=color, weight="bold")
    axis.text(0.985, 0.9, f"Mean interval: {summary.loc[interval, 'mean']:.1f} ms",
              transform=axis.transAxes, ha="right", va="top", color="#39434d")
    axis.grid(axis="y", visible=False)
    sns.despine(ax=axis, left=True)
axes[-1].set(xlim=(0, 3.2), xlabel="Time after command (s)")
fig.supylabel("LD2 output state", x=0.005)
fig.tight_layout(pad=1.2)
(ROOT / "figures").mkdir(exist_ok=True)
fig.savefig(ROOT / "figures/led_timing.pdf", bbox_inches="tight")
fig.savefig(ROOT / "figures/led_timing.png", dpi=220, bbox_inches="tight")
plt.close(fig)

rows = [r"\begin{tabular}{@{}rrrr@{}}", r"\toprule",
        r"Command (ms) & Mean (ms) & SD (ms) & Intervals \\", r"\midrule"]
for interval in (1000, 500, 250):
    row = summary.loc[interval]
    rows.append(f"{interval} & {row['mean']:.1f} & {row['std']:.1f} & {int(row['count'])} " + r"\\")
rows += [r"\bottomrule", r"\end{tabular}"]
(ROOT / "measurement_table.tex").write_text("\n".join(rows) + "\n", encoding="utf-8")
