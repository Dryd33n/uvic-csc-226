import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LightSource
ls = LightSource(azdeg=315, altdeg=45)


df = pd.read_csv('results.csv')
df = df.groupby(['density', 'vertex_count', 'k'], as_index=False).mean()
densities = sorted(df['density'].unique())


for density in densities:

    subset = df[df['density'] == density].copy()
    target_fractions = [0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0]

    rows = []

    for vertex_count, group in subset.groupby('vertex_count'):
        for target in target_fractions:
            target_k = max(1, round(target * vertex_count))
            closest = group.iloc[(group['k'] - target_k).abs().argsort()[:1]]
            row = closest.copy()
            row['k_fraction'] = target
            rows.append(row)

    clean = pd.concat(rows, ignore_index=True)
    clean = clean.groupby(['vertex_count', 'k_fraction'], as_index=False).mean()

    dijkstra_pivot = clean.pivot(index='vertex_count', columns='k_fraction', values='dijkstra_median_ms')
    fwr_pivot = clean.pivot(index='vertex_count', columns='k_fraction', values='fwr_median_ms')

    k_fractions = np.array(target_fractions)
    vertex_counts = dijkstra_pivot.index.values
    VC, K = np.meshgrid(vertex_counts, k_fractions)

    Z_dijkstra = dijkstra_pivot.values.T
    Z_fwr = fwr_pivot.values.T

    fig = plt.figure(figsize=(12, 8))
    fig.patch.set_facecolor('white')
    ax = fig.add_subplot(111, projection='3d')

    surf1 = ax.plot_surface(VC, K, Z_dijkstra, cmap='Blues', alpha=0.85,
                             edgecolor='k', linewidth=0.15, antialiased=True, shade=True)
    surf2 = ax.plot_surface(VC, K, Z_fwr, cmap='YlOrRd', alpha=0.45,
                             edgecolor='k', linewidth=0.15, antialiased=True, shade=True)

    crossover_k   = []
    crossover_vc  = []
    crossover_z   = []

    for i, vc in enumerate(vertex_counts):
        dijk_row = dijkstra_pivot.values[i]
        fwr_row  = fwr_pivot.values[i]
        diff     = dijk_row - fwr_row

        for j in range(len(diff) - 1):
            if diff[j] <= 0 and diff[j + 1] > 0:
                t = -diff[j] / (diff[j + 1] - diff[j])
                k_cross  = k_fractions[j] + t * (k_fractions[j + 1] - k_fractions[j])
                z_cross  = dijk_row[j] + t * (dijk_row[j + 1] - dijk_row[j])
                crossover_k.append(k_cross)
                crossover_vc.append(vc)
                crossover_z.append(z_cross)
                break

    if crossover_k:
        crossover_z_raised = [z * 1.02 for z in crossover_z]
        ax.plot(crossover_vc, crossover_k, crossover_z_raised,
                color='red', linewidth=1, zorder=10, label='Crossover')

    dijkstra_legend = plt.Rectangle((0, 0), 1, 1, fc='steelblue', alpha=0.75)
    fwr_legend = plt.Rectangle((0, 0), 1, 1, fc='tomato', alpha=0.45)
    crossover_legend = plt.Line2D([0], [0], color='red', linewidth=3)
    ax.legend([dijkstra_legend, fwr_legend, crossover_legend],
              ["Dijkstra's Algorithm", 'Floyd-Warshall-Roy', 'Crossover'],
              loc='upper left', fontsize=11)

    ax.set_xlabel("Vertex Count (n)", fontsize=13, labelpad=15)
    ax.set_ylabel("k / n", fontsize=13, labelpad=15)
    ax.set_zlabel('Time (ms)', fontsize=13, labelpad=15)
    ax.set_title(f'k-Source Shortest Path Performance Density = {density}',
                 fontsize=14, fontweight='bold', pad=20)
    ax.view_init(elev=25, azim=225)
    ax.grid(True, alpha=0.3)
    ax.tick_params(labelsize=10)
    ax.dist = 11

    plt.tight_layout()
    plt.savefig(f'plot_density_{density}.png', dpi=300, bbox_inches='tight', facecolor='white')
    plt.close()

fig, ax = plt.subplots(figsize=(9, 6))
fig.patch.set_facecolor('white')

for density in densities:
    subset = df[df['density'] == density].copy()
    subset['k_fraction'] = subset['k'] / subset['vertex_count']

    crossover_fractions     = []
    crossover_vertex_counts = []

    for vertex_count, group in subset.groupby('vertex_count'):
        group   = group.sort_values('k_fraction')
        crossed = group[group['dijkstra_median_ms'] > group['fwr_median_ms']]
        if not crossed.empty:
            crossover_fractions.append(crossed.iloc[0]['k_fraction'])
            crossover_vertex_counts.append(vertex_count)

    ax.plot(crossover_vertex_counts, crossover_fractions,
            marker='o', label=f'Density = {density}')

ax.set_xlabel('Vertex Count (n)', fontsize=13)
ax.set_ylabel('Crossover k / n', fontsize=13)
ax.set_title('k/n at which Floyd-Warshall-Roy Becomes Faster than Dijkstra',
             fontsize=13, fontweight='bold')
ax.legend(fontsize=11)
ax.grid(True, alpha=0.3)
ax.tick_params(labelsize=10)

plt.tight_layout()
plt.savefig('plot_crossover.png', dpi=300, bbox_inches='tight', facecolor='white')
plt.close()