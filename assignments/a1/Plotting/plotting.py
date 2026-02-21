import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

timePower = 0.3

# Read the CSV file
df = pd.read_csv('results.csv')

# Create pivot tables for 3D plotting
df['LogMapSize'] = np.log2(df['MapSize'])  # Apply logarithmic transformation
lp_pivot = df.pivot(index='LogMapSize', columns='LoadFactor', values='LP_Median_ms')
ch_pivot = df.pivot(index='LogMapSize', columns='LoadFactor', values='CH_Median_ms')

# Create meshgrid for 3D surface
X = lp_pivot.columns.values  # Load factors
Y = lp_pivot.index.values    # Log-transformed map sizes
X, Y = np.meshgrid(X, Y)
Z_lp = np.power(lp_pivot.values, timePower)
Z_ch = np.power(ch_pivot.values, timePower)

# Determine the maximum values for Z_lp and Z_ch
max_Z_lp = np.max(Z_lp)
max_Z_ch = np.max(Z_ch)

# Create side-by-side 3D surfaces
fig = plt.figure(figsize=(20, 8))

# Linear Probing surface
ax1 = fig.add_subplot(121, projection='3d')
surf1 = ax1.plot_surface(X, Y, Z_lp, cmap='viridis', alpha=0.8,
                          edgecolor='k', linewidth=0.2, antialiased=True,
                          vmin=0, vmax=max_Z_lp)  # Set vmax to max_Z_lp
ax1.set_xlabel('Load Factor', fontsize=13, labelpad=15)
ax1.set_ylabel('Map Size', fontsize=13, labelpad=15)
ax1.set_zlabel('Time', fontsize=13, labelpad=15)
ax1.set_title('Linear Probing', fontsize=16, fontweight='bold', pad=25)

# Adjust y-axis ticks to show original MapSize values
original_map_ticks = [10, 100, 1000, 10000, 100000, 1000000]
log_map_ticks = np.log2(original_map_ticks)
ax1.set_yticks(log_map_ticks)
ax1.set_yticklabels(original_map_ticks)

# Adjust z-axis ticks to show original Time values
original_time_ticks = [100, 500, 1000, 1500]
log_time_ticks = np.power(original_time_ticks, timePower)
ax1.set_zticks(log_time_ticks)
ax1.set_zticklabels(original_time_ticks)

ax1.view_init(elev=25, azim=225)
ax1.grid(True, alpha=0.3)
ax1.tick_params(labelsize=10)
ax1.dist = 11
cbar1 = fig.colorbar(surf1, ax=ax1, shrink=0.6, pad=0.1)
cbar1.set_label('Time (ms)', fontsize=11)

# Set color bar ticks to original time values
cbar1.set_ticks(log_time_ticks)
cbar1.set_ticklabels(original_time_ticks)

# Chaining surface
ax2 = fig.add_subplot(122, projection='3d')
surf2 = ax2.plot_surface(X, Y, Z_ch, cmap='plasma', alpha=0.8,
                          edgecolor='k', linewidth=0.2, antialiased=True,
                          vmin=0, vmax=max_Z_ch)  # Set vmax to max_Z_ch
ax2.set_xlabel('Load Factor', fontsize=13, labelpad=15)
ax2.set_ylabel('Map Size', fontsize=13, labelpad=15)
ax2.set_zlabel('Time', fontsize=13, labelpad=15)
ax2.set_title('Chaining', fontsize=16, fontweight='bold', pad=25)

# Adjust y-axis ticks to show original MapSize values
ax2.set_yticks(log_map_ticks)
ax2.set_yticklabels(original_map_ticks)

# Adjust z-axis ticks to show original Time values
ax2.set_zticks(log_time_ticks)
ax2.set_zticklabels(original_time_ticks)

ax2.view_init(elev=25, azim=225)
ax2.grid(True, alpha=0.3)
ax2.tick_params(labelsize=10)
ax2.dist = 11
cbar2 = fig.colorbar(surf2, ax=ax2, shrink=0.6, pad=0.1)
cbar2.set_label('Time (ms)', fontsize=11)

# Set color bar ticks to original time values
cbar2.set_ticks(log_time_ticks)
cbar2.set_ticklabels(original_time_ticks)

plt.savefig('surface_comparison.png', dpi=300, bbox_inches='tight', facecolor='white')
print("✓ Saved: surface_comparison.png")
plt.show()