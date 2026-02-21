import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


timePower = 0.3


df = pd.read_csv('results.csv')


df = df[df['LoadFactor'].isin([0.5, 0.75])]


df = df.sort_values('MapSize')
fig, axs = plt.subplots(1, 4, figsize=(28, 6))
plt.subplots_adjust(wspace=0.25)

def plot_no_fit(ax, x, y, color, label, ymin, ymax, yticks):
    y_scaled = np.power(y, timePower)
    ax.plot(x, y_scaled, marker='o', color=color, linewidth=2, label=label)
    ax.legend(fontsize=10)
    ax.set_ylim(ymin, ymax)
    ax.set_xscale('log', base=2)
    xticks = x
    ax.set_xticks(xticks)
    ax.set_xticklabels([f'$2^{{{int(np.log2(val))}}}$' for val in xticks], fontsize=11)
    ax.tick_params(axis='x', rotation=0)
    ytick_scaled = np.power(yticks, timePower)
    ax.set_yticks(ytick_scaled)
    ax.set_yticklabels([f'{int(tick)}' if tick >= 10 else f'{tick:.2f}' for tick in yticks], fontsize=11)


df_05 = df[df['LoadFactor'] == 0.5]
lp_05 = df_05['LP_Median_ms']
ch_05 = df_05['CH_Median_ms']
all_05 = pd.concat([lp_05, ch_05])
ymin_05 = min(np.power(lp_05.min(), timePower), np.power(ch_05.min(), timePower))
ymax_05 = max(np.power(lp_05.max(), timePower), np.power(ch_05.max(), timePower))
yticks_05 = np.linspace(all_05.min(), all_05.max(), 15)  # More ticks


df_075 = df[df['LoadFactor'] == 0.75]
lp_075 = df_075['LP_Median_ms']
ch_075 = df_075['CH_Median_ms']
all_075 = pd.concat([lp_075, ch_075])
ymin_075 = min(np.power(lp_075.min(), timePower), np.power(ch_075.min(), timePower))
ymax_075 = max(np.power(lp_075.max(), timePower), np.power(ch_075.max(), timePower))
yticks_075 = np.linspace(all_075.min(), all_075.max(), 15)  # More ticks


plot_no_fit(
    axs[0],
    df_05['MapSize'],
    lp_05,
    color='seagreen',
    label='Data',
    ymin=ymin_05,
    ymax=ymax_05,
    yticks=yticks_05
)
axs[0].set_title('Linear Probing (Load Factor = 0.5)', fontsize=15, fontweight='bold', pad=15)
axs[0].set_xlabel('Map Size', fontsize=12)
axs[0].set_ylabel('Run Time (ms)', fontsize=12)
axs[0].grid(True, alpha=0.3)
axs[0].tick_params(labelsize=10)


plot_no_fit(
    axs[1],
    df_05['MapSize'],
    ch_05,
    color='crimson',
    label='Data',
    ymin=ymin_05,
    ymax=ymax_05,
    yticks=yticks_05
)
axs[1].set_title('Chaining (Load Factor = 0.5)', fontsize=15, fontweight='bold', pad=15)
axs[1].set_xlabel('Map Size', fontsize=12)
axs[1].set_ylabel('Run Time (ms)', fontsize=12)
axs[1].grid(True, alpha=0.3)
axs[1].tick_params(labelsize=10)


plot_no_fit(
    axs[2],
    df_075['MapSize'],
    lp_075,
    color='orange',
    label='Data',
    ymin=ymin_075,
    ymax=ymax_075,
    yticks=yticks_075
)
axs[2].set_title('Linear Probing (Load Factor = 0.75)', fontsize=15, fontweight='bold', pad=15)
axs[2].set_xlabel('Map Size', fontsize=12)
axs[2].set_ylabel('Run Time (ms)', fontsize=12)
axs[2].grid(True, alpha=0.3)
axs[2].tick_params(labelsize=10)


plot_no_fit(
    axs[3],
    df_075['MapSize'],
    ch_075,
    color='darkblue',
    label='Data',
    ymin=ymin_075,
    ymax=ymax_075,
    yticks=yticks_075
)
axs[3].set_title('Chaining (Load Factor = 0.75)', fontsize=15, fontweight='bold', pad=15)
axs[3].set_xlabel('Map Size', fontsize=12)
axs[3].set_ylabel('Run Time (ms)', fontsize=12)
axs[3].grid(True, alpha=0.3)
axs[3].tick_params(labelsize=10)

plt.suptitle('Hash Table Performance by Map Size and Load Factor', fontsize=18, fontweight='bold', y=1.05)
plt.tight_layout(rect=[0, 0, 1, 1])
plt.savefig('line_comparison_by_mapsize.png', dpi=300, bbox_inches='tight', facecolor='white')
