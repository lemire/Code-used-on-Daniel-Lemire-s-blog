import matplotlib.pyplot as plt
import math


def main() -> None:
    measured_counts = {
        1: 2,
        2: 7,
        3: 26,
        4: 90,
        5: 340,
        6: 1238,
        7: 4647,
        8: 17578,
        9: 67592,
        10: 259768,
        11: 1004348,
        12: 3902357,
        13: 15202050,
        14: 59410557,
        15: 232483840,
        16: 911689012,
        30: 204505763483830092,
        32: 3215709724700470901,
    }

    measured_bits = sorted(measured_counts)
    measured_percentages = []
    for bit_count in measured_bits:
        max_value = (1 << bit_count) - 1
        max_product_range = max_value * max_value + 1
        measured_percentages.append(
            100.0 * measured_counts[bit_count] / max_product_range
        )

    bits = list(range(1, 33))

    # Erdos-Ford-Tenenbaum constant in the multiplication-table asymptotic.
    c = 1.0 - (1.0 + math.log(math.log(2.0))) / math.log(2.0)

    ford_percentages = []
    for bit_count in bits:
        n = (1 << bit_count) - 1
        if n <= 1:
            ford_percentages.append(float("nan"))
            continue

        log_n = math.log(n)
        log_log_n = math.log(log_n)
        if log_n <= 0.0 or log_log_n <= 0.0:
            ford_percentages.append(float("nan"))
            continue

        approx_distinct = (n * n) / ((log_n ** c) * (log_log_n ** 1.5))
        max_product_range = n * n + 1
        ford_percentages.append(100.0 * approx_distinct / max_product_range)

    plt.figure(figsize=(8, 5), dpi=150)
    ax = plt.gca()
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    plt.plot(
        measured_bits,
        measured_percentages,
        marker="o",
        linewidth=2,
        color="#1f77b4",
        label="Measured",
    )
    #plt.plot(
    #    bits,
    #    ford_percentages,
    #    linestyle="--",
    #    linewidth=2,
    #    color="#d62728",
    #    label="Ford approximation",
    #)
    plt.title("Distinct Product Percentage by Bit Width")
    plt.xlabel("Number of bits (n)")
    plt.ylabel("Percentage (%)")
    plt.xticks([1] + list(range(2, 33, 2)))
    plt.xlim(1, 32)
    plt.ylim(0, 52)
    plt.grid(True, linestyle="--", alpha=0.4)
    #plt.legend()
    plt.tight_layout()
    plt.savefig("products_percentage.png")


if __name__ == "__main__":
    main()
