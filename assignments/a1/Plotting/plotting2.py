import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


df = pd.read_csv('results.csv')

map_size1 = 8388608
map_size2 = 16777216


df1 = df[df['MapSize'] == map_size1]
df2 = df[df['MapSize'] == map_size2]


ymin1 = min(df1['CH_Median_ms'].min(), df1['LP_Median_ms'].min())
ymax1 = max(df1['CH_Median_ms'].max(), df1['LP_Median_ms'].max())
ymin2 = min(df2['CH_Median_ms'].min(), df2['LP_Median_ms'].min())
ymax2 = max(df2['CH_Median_ms'].max(), df2['LP_Median_ms'].max())


fig, axs = plt.subplots(1, 4, figsize=(28, 6))
plt.subplots_adjust(wspace=0.25)

def plot_with_fit(ax, x, y, color, label, fit_color, ymin, ymax):
    ax.plot(x, y, marker='o', color=color, linewidth=2, label=label)
    coeffs = np.polyfit(x, y, 1)
    fit_fn = np.poly1d(coeffs)
    x_fit = np.linspace(min(x), max(x), 100)
    y_fit = fit_fn(x_fit)
    ax.plot(x_fit, y_fit, linestyle='--', color=fit_color, linewidth=2, label='Best Fit')
    ax.legend(fontsize=10)
    ax.set_ylim(ymin, ymax)


plot_with_fit(
    axs[0],
    df1['LoadFactor'],
    df1['LP_Median_ms'],
    color='seagreen',
    label='Data',
    fit_color='black',
    ymin=ymin1,
    ymax=ymax1
)
axs[0].set_title('Linear Probing (Map Size = 8388608)', fontsize=15, fontweight='bold', pad=15)
axs[0].set_xlabel('Load Factor', fontsize=12)
axs[0].set_ylabel('Run Time (ms)', fontsize=12)
axs[0].grid(True, alpha=0.3)
axs[0].tick_params(labelsize=10)


plot_with_fit(
    axs[1],
    df1['LoadFactor'],
    df1['CH_Median_ms'],
    color='crimson',
    label='Data',
    fit_color='black',
    ymin=ymin1,
    ymax=ymax1
)
axs[1].set_title('Chaining (Map Size = 8388608)', fontsize=15, fontweight='bold', pad=15)
axs[1].set_xlabel('Load Factor', fontsize=12)
axs[1].set_ylabel('Run Time (ms)', fontsize=12)
axs[1].grid(True, alpha=0.3)
axs[1].tick_params(labelsize=10)


plot_with_fit(
    axs[2],
    df2['LoadFactor'],
    df2['LP_Median_ms'],
    color='orange',
    label='Data',
    fit_color='black',
    ymin=ymin2,
    ymax=ymax2
)
axs[2].set_title('Linear Probing (Map Size = 16777216)', fontsize=15, fontweight='bold', pad=15)
axs[2].set_xlabel('Load Factor', fontsize=12)
axs[2].set_ylabel('Run Time (ms)', fontsize=12)
axs[2].grid(True, alpha=0.3)
axs[2].tick_params(labelsize=10)


plot_with_fit(
    axs[3],
    df2['LoadFactor'],
    df2['CH_Median_ms'],
    color='darkblue',
    label='Data',
    fit_color='black',
    ymin=ymin2,
    ymax=ymax2
)
axs[3].set_title('Chaining (Map Size = 16777216)', fontsize=15, fontweight='bold', pad=15)
axs[3].set_xlabel('Load Factor', fontsize=12)
axs[3].set_ylabel('Run Time (ms)', fontsize=12)
axs[3].grid(True, alpha=0.3)
axs[3].tick_params(labelsize=10)

plt.suptitle('Hash Table Performance by Load Factor and Map Size', fontsize=18, fontweight='bold', y=1.05)
plt.tight_layout(rect=[0, 0, 1, 0.97])
plt.savefig('line_comparison.png', dpi=300, bbox_inches='tight', facecolor='white')
