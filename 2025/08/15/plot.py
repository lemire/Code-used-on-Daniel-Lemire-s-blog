import matplotlib.pyplot as plt
import re

def parse_output(output):
    # Modèle regex pour extraire les valeurs min, max et mean
    pattern = r"(\w+(?:\s\w+)?)\s\(ms\)\s*:\s*min=([\d.]+),\s*max=([\d.]+),\s*mean=([\d.]+)"
    matches = re.findall(pattern, output, re.MULTILINE)
    
    labels = []
    means = []
    mins = []
    maxs = []
    for match in matches:
        label = match[0]  # Nom du type d'accès
        min_val = float(match[1])
        max_val = float(match[2])
        mean = float(match[3])  # Valeur moyenne
        labels.append(label)
        mins.append(min_val)
        maxs.append(max_val)
        means.append(mean)
    
    return labels, means, mins, maxs

def plot_bar_chart(labels, means, mins, maxs):
    plt.rcParams.update({'font.size': 16})
    fig, ax = plt.subplots(figsize=(10, 6))
    barlist = ax.bar(labels, means, color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd'])
    for i, (min_val, max_val, mean_val) in enumerate(zip(mins, maxs, means)):
        ax.plot([i, i], [min_val, max_val], color='black', marker='o', linestyle='-', linewidth=2, markersize=8, label='Min/Max' if i == 0 else "")
        ax.scatter(i, min_val, color='green', zorder=5)
        ax.scatter(i, max_val, color='red', zorder=5)
        # Ajout du label au-dessus de chaque barre
        ax.text(i, mean_val + (maxs[i] - mins[i]) * 0.05, f"{mean_val:.0f}", ha='center', va='bottom', fontsize=16, fontweight='bold', color=barlist[i].get_facecolor())
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    #ax.set_xlabel("Access type")
    ax.set_ylabel("Time (ms)")
    ax.grid(axis='y', linestyle='--', alpha=0.7)
    ax.legend(['Min/Max range', 'Min', 'Max'], frameon=False)
    plt.tight_layout()
    plt.savefig("plot.png", dpi=300)
    plt.close(fig)

def main():
    output = """Seq (ms) : min=3.91, max=14.27, mean=5.27
Random (ms) : min=45.97, max=52.92, mean=47.66
Backward (ms) : min=4.21, max=12.71, mean=6.01
Interleaved (ms) : min=4.31, max=12.76, mean=5.33
Bouncing (ms) : min=4.18, max=11.56, mean=5.78"""
    
    labels, means, mins, maxs = parse_output(output)
    plot_bar_chart(labels, means, mins, maxs)

if __name__ == "__main__":
    main()