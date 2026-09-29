"""Generate the ADC-to-PWM mapping figure for Tasks 3 and 4."""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns


output = Path(__file__).resolve().parent / "figures"
output.mkdir(exist_ok=True)

sns.set_theme(style="whitegrid", context="notebook")
navy = "#173B57"
blue = "#2F6F9F"
orange = "#D66A1F"

adc = np.arange(1024)
led_duty = np.floor(adc * 1000 / 1023) / 10
servo_pulse = 1000 + np.floor(adc * 1000 / 1023)

fig, axes = plt.subplots(1, 2, figsize=(10.4, 4.0))
sns.lineplot(x=adc, y=led_duty, ax=axes[0], color=blue, linewidth=2.4)
axes[0].set(xlabel="Arduino ADC sample", ylabel="LED duty cycle (%)", xlim=(0, 1023), ylim=(0, 100))
axes[0].set_title("Task 3: linear LED PWM mapping", color=navy, weight="bold")
axes[0].scatter([0, 512, 1023], [0, np.floor(512 * 1000 / 1023) / 10, 100], color=orange, zorder=3)

sns.lineplot(x=adc, y=servo_pulse, ax=axes[1], color=orange, linewidth=2.4)
axes[1].set(xlabel="Arduino ADC sample", ylabel="Servo pulse width (µs)", xlim=(0, 1023), ylim=(950, 2050))
axes[1].set_title("Task 4: linear servo command mapping", color=navy, weight="bold")
axes[1].scatter([0, 512, 1023], [1000, 1000 + np.floor(512 * 1000 / 1023), 2000], color=blue, zorder=3)

for axis in axes:
    axis.spines[["top", "right"]].set_visible(False)
    axis.grid(color="#D8E1E7", linewidth=0.7)

fig.tight_layout(pad=1.0)
fig.savefig(output / "task34_pwm_mapping.pdf", bbox_inches="tight")
fig.savefig(output / "task34_pwm_mapping.png", dpi=220, bbox_inches="tight")
plt.close(fig)
