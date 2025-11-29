import matplotlib.pyplot as plt
import numpy as np

# Data
browsers = ['Safari', 'SigmaOS', 'Chrome', 'Edge', 'Brave', 'Servo', 'Firefox']

encoding = [17, 17, 19, 19, 19, 0.34, 0.34]   # GB/s
decoding = [9.4, 9.4, 4.6, 4.6, 4.6, 0.40, 0.40]  # GB/s

# Colors exactly matching your SVG
colors = [
    '#1e88e5',  # WebKit (Safari, SigmaOS) → blue
    '#1e88e5',
    '#d81b60',  # Blink (Chrome, Edge, Brave) → pink/red
    '#d81b60',
    '#d81b60',
    '#43a047',  # Gecko/Servo → green
    '#43a047'
]

x = np.arange(len(browsers))

# Create figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7), dpi=200)

# === Encoding chart ===
bars1 = ax1.bar(x, encoding, color=colors, width=0.66)
ax1.set_title('Browser encoding speed (higher is better)', fontsize=18, pad=20, fontweight='600')
ax1.set_ylabel('GB/s', fontsize=14)
ax1.set_ylim(0, 20)
ax1.set_yticks(np.arange(0, 21, 2))
ax1.grid(True, axis='y', linestyle='-', alpha=0.3)
ax1.set_xticks(x)
ax1.set_xticklabels(browsers, fontsize=12, rotation=0)

# Remove top and right spines
ax1.spines['top'].set_visible(False)
ax1.spines['right'].set_visible(False)

# Value labels on top of bars
for bar in bars1:
    height = bar.get_height()
    if height >= 1:
        ax1.text(bar.get_x() + bar.get_width()/2, height + 0.6,
                 f'{height} GB/s', ha='center', va='bottom', fontsize=12, fontweight='bold')
    else:
        ax1.text(bar.get_x() + bar.get_width()/2, height + 0.8,
                 f'{height} GB/s', ha='center', va='bottom', fontsize=10, color='black')

# === Decoding chart ===
bars2 = ax2.bar(x, decoding, color=colors, width=0.66)
ax2.set_title('Browser decoding speed (higher is better)', fontsize=18, pad=20, fontweight='600')
ax2.set_ylabel('GB/s', fontsize=14)
ax2.set_ylim(0, 10)
ax2.set_yticks(np.arange(0, 11, 1))
ax2.grid(True, axis='y', linestyle='-', alpha=0.3)
ax2.set_xticks(x)
ax2.set_xticklabels(browsers, fontsize=12, rotation=0)

# Remove top and right spines
ax2.spines['top'].set_visible(False)
ax2.spines['right'].set_visible(False)

# Value labels on top of bars
for bar in bars2:
    height = bar.get_height()
    if height >= 1:
        ax2.text(bar.get_x() + bar.get_width()/2, height + 0.3,
                 f'{height} GB/s', ha='center', va='bottom', fontsize=12, fontweight='bold')
    else:
        ax2.text(bar.get_x() + bar.get_width()/2, height + 0.4,
                 f'{height} GB/s', ha='center', va='bottom', fontsize=10, color='black')

# Layout and save
plt.tight_layout(pad=3.0)
plt.savefig('browser_encoding_decoding_speed.png', bbox_inches='tight', dpi=300)
plt.close()

print("Done! File saved as: browser_encoding_decoding_speed.png")
