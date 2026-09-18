"""Generate the Lab 2 result placeholders and PWM mapping figure."""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from matplotlib.patches import FancyBboxPatch


OUTPUT = Path(__file__).resolve().parent / "figures"
OUTPUT.mkdir(exist_ok=True)

sns.set_theme(style="whitegrid", context="notebook")
NAVY = "#173B57"
BLUE = "#2F6F9F"
ORANGE = "#D66A1F"
SOFT = "#F2F5F7"
INK = "#263238"


def terminal_panel(ax, title, transcript, note):
    """Draw a clean placeholder shaped like a serial-terminal capture."""
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.axis("off")
    window = FancyBboxPatch(
        (0.02, 0.05),
        0.96,
        0.89,
        boxstyle="round,pad=0.012,rounding_size=0.02",
        facecolor="#17212B",
        edgecolor=NAVY,
        linewidth=1.5,
    )
    ax.add_patch(window)
    ax.add_patch(
        FancyBboxPatch(
            (0.02, 0.83),
            0.96,
            0.11,
            boxstyle="round,pad=0.012,rounding_size=0.02",
            facecolor=NAVY,
            edgecolor=NAVY,
        )
    )
    ax.text(0.06, 0.875, title, color="white", weight="bold", fontsize=10, va="center")
    ax.text(
        0.07,
        0.75,
        transcript,
        color="#D8E6EF",
        family="monospace",
        fontsize=9.2,
        va="top",
        linespacing=1.45,
    )
    ax.text(
        0.5,
        0.105,
        note,
        color="#FFC46B",
        fontsize=8.5,
        ha="center",
        va="center",
        weight="bold",
    )


fig, axes = plt.subplots(1, 2, figsize=(10.4, 3.0))
terminal_panel(
    axes[0],
    "Addition result",
    "> 1\nEnter three 2-digit integers: 12 34 56\nOperation [A=add, M=multiply]: A\nResult: 102",
    "REPLACE WITH SERIAL-MONITOR CAPTURE",
)
terminal_panel(
    axes[1],
    "Multiplication result",
    "> 1\nEnter three 2-digit integers: 12 34 56\nOperation [A=add, M=multiply]: M\nResult: 22848",
    "REPLACE WITH SERIAL-MONITOR CAPTURE",
)
fig.tight_layout(pad=0.7)
fig.savefig(OUTPUT / "task1_uart_results.pdf", bbox_inches="tight")
fig.savefig(OUTPUT / "task1_uart_results.png", dpi=220, bbox_inches="tight")
plt.close(fig)


fig, axes = plt.subplots(1, 2, figsize=(10.4, 3.0))
terminal_panel(
    axes[0],
    "I2C register 0x00",
    "> 2\nEnter speed 0..3 and data 0..1: 2 0\nCommand 0x82, register 0x00,\nanalog value: 673",
    "REPLACE WITH 2-BYTE ANALOG CAPTURE",
)
terminal_panel(
    axes[1],
    "I2C register 0x01",
    "> 2\nEnter speed 0..3 and data 0..1: 2 1\nCommand 0x82, register 0x01,\ntext: RTCA",
    "REPLACE WITH 4-BYTE DATA CAPTURE",
)
fig.tight_layout(pad=0.7)
fig.savefig(OUTPUT / "task2_i2c_results.pdf", bbox_inches="tight")
fig.savefig(OUTPUT / "task2_i2c_results.png", dpi=220, bbox_inches="tight")
plt.close(fig)


adc = np.arange(0, 1024)
led_duty = np.floor(adc * 1000 / 1023) / 10
servo_pulse = 1000 + np.floor(adc * 1000 / 1023)

fig, axes = plt.subplots(1, 2, figsize=(10.4, 4.0))
sns.lineplot(x=adc, y=led_duty, ax=axes[0], color=BLUE, linewidth=2.4)
axes[0].set(xlabel="Arduino ADC sample", ylabel="LED duty cycle (%)", xlim=(0, 1023), ylim=(0, 100))
axes[0].set_title("Task 3: linear LED PWM mapping", color=NAVY, weight="bold")
axes[0].scatter([0, 512, 1023], [0, np.floor(512 * 1000 / 1023) / 10, 100], color=ORANGE, zorder=3)

sns.lineplot(x=adc, y=servo_pulse, ax=axes[1], color=ORANGE, linewidth=2.4)
axes[1].set(xlabel="Arduino ADC sample", ylabel="Servo pulse width (µs)", xlim=(0, 1023), ylim=(950, 2050))
axes[1].set_title("Task 4: linear servo command mapping", color=NAVY, weight="bold")
axes[1].scatter([0, 512, 1023], [1000, 1000 + np.floor(512 * 1000 / 1023), 2000], color=BLUE, zorder=3)

for ax in axes:
    ax.spines[["top", "right"]].set_visible(False)
    ax.grid(color="#D8E1E7", linewidth=0.7)

fig.tight_layout(pad=1.0)
fig.savefig(OUTPUT / "task34_pwm_mapping.pdf", bbox_inches="tight")
fig.savefig(OUTPUT / "task34_pwm_mapping.png", dpi=220, bbox_inches="tight")
plt.close(fig)
