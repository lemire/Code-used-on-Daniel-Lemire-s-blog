import matplotlib.pyplot as plt
import re

def parse_output(output):
    # Regex pour extraire le type d'accès et ns/op
    pattern = r"Benchmark(\w+)Access(?:WithBuffer)?\s+\d+\s+(\d+) ns/op"
    matches = re.findall(pattern, output)

    labels = []
    means = []
    for match in matches:
        label = match[0]
        ns_per_op = int(match[1])
        labels.append(label)
        means.append(ns_per_op / 1e6)  # Conversion ns -> ms
    return labels, means

def plot_bar_chart(labels, means, mins, maxs):
    plt.rcParams.update({'font.size': 16})
    fig, ax = plt.subplots(figsize=(10, 6))
    barlist = ax.bar(labels, means, color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd'])
    for i, mean_val in enumerate(means):
        ax.text(i, mean_val + max(means)*0.02, f"{mean_val:.1f}", ha='center', va='bottom', fontsize=16, fontweight='bold', color=barlist[i].get_facecolor())
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_ylabel("Time per op (ms)")
    ax.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig("plot.png", dpi=300)
    plt.close(fig)

def main():
    output = """BenchmarkSequentialAccessWithBuffer     2937        369939 ns/op
BenchmarkRandomAccess      934     1314153 ns/op
BenchmarkBackwardAccessWithBuffer     3229          354276 ns/op
BenchmarkBouncingAccessWithBuffer     3598          338103 ns/op
BenchmarkInterleavedAccessWithBuffer     3289       331715 ns/op"""

    labels, means = parse_output(output)
    plot_bar_chart(labels, means, None, None)

if __name__ == "__main__":
    main()