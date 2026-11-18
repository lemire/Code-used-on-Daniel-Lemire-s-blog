"""The overflow, byte by byte.

128 'A's and one '=' make Java allocate 98 bytes. The main loop writes
bytes 0-95 as two 48-byte masked stores. The tail computes n = -1;
bzhi reads 255 as the count, keeps all 64 mask bits, and the next
store writes bytes 96-159. Two land inside the array, 62 past the end.
"""

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

CREAM = "#fff6ee"
INK = "#1c1917"
MUTED = "#57534e"
FAINT = "#d6d3d1"
TEAL = "#0f766e"
TEAL_PALE = "#ccfbf1"
RED = "#be123c"
RED_PALE = "#ffe4e6"
AMBER = "#b45309"

END = 98  # array length
TOTAL = 160  # last byte touched + 1


def font(size, weight="regular", color=INK):
    return {"fontsize": size, "fontweight": weight, "color": color}


def main():
    plt.rcParams["font.family"] = "Avenir Next"
    fig, ax = plt.subplots(figsize=(15.4, 5.6), dpi=160)
    fig.patch.set_facecolor(CREAM)
    ax.set_facecolor(CREAM)
    x0 = 14  # left margin for row labels
    ax.set_xlim(0, x0 + TOTAL + 2)
    ax.set_ylim(0, 56)
    ax.axis("off")

    def x(b):
        return x0 + b

    def cells(y, h, first, last, face, edge, lw=0.6):
        """One rectangle per byte, first..last inclusive."""
        for b in range(first, last + 1):
            ax.add_patch(Rectangle((x(b), y), 1, h, facecolor=face, edgecolor=edge, linewidth=lw))

    def outline(y, h, first, last, edge, lw=2.4):
        ax.add_patch(Rectangle((x(first), y), last - first + 1, h, fill=False, edgecolor=edge, linewidth=lw))

    # Row 1: memory. The array, then whatever follows it on the heap.
    ym, hm = 38, 7
    cells(ym, hm, 0, END - 1, TEAL_PALE, TEAL, 0.5)
    cells(ym, hm, END, TOTAL - 1, "#f5f5f4", FAINT, 0.5)
    outline(ym, hm, 0, END - 1, TEAL)
    ax.text(x(END / 2), ym + hm / 2, "byte[98] from decode()", ha="center", va="center", **font(17, "heavy", TEAL),
            bbox=dict(boxstyle="round,pad=0.25", facecolor=TEAL_PALE, edgecolor="none"))
    ax.text(x((END + TOTAL) / 2), ym + hm / 2, "next object on the heap", ha="center", va="center",
            **font(15, "medium", MUTED), bbox=dict(boxstyle="round,pad=0.25", facecolor="#f5f5f4", edgecolor="none"))
    ax.text(x0 - 1, ym + hm / 2, "memory", ha="right", va="center", **font(15, "demibold", MUTED))

    # Row 2: main loop, two 48-byte stores, bytes 0-95.
    y1, h1 = 26, 7
    cells(y1, h1, 0, 95, TEAL, CREAM, 0.5)
    ax.text(x(48), y1 + h1 / 2, "main loop: two stores of 48 bytes", ha="center", va="center", **font(15, "heavy", "white"))
    ax.text(x0 - 1, y1 + h1 / 2, "store 1", ha="right", va="center", **font(15, "demibold", MUTED))

    # Row 3: tail store, 64 bytes from offset 96, mask from bzhi(ones, 255).
    y2, h2 = 14, 7
    cells(y2, h2, 96, END - 1, AMBER, CREAM, 0.5)
    cells(y2, h2, END, TOTAL - 1, RED, CREAM, 0.5)
    ax.text(x((END + TOTAL) / 2), y2 + h2 / 2, "62 bytes past the end", ha="center", va="center",
            **font(15, "heavy", "white"))
    ax.text(x0 - 1, y2 + h2 / 2, "store 2", ha="right", va="center", **font(15, "demibold", MUTED))
    ax.text(x(94), y2 + h2 / 2, "tail: n = −1, bzhi count 255, mask = 64 ones",
            ha="right", va="center", **font(13, "medium", RED))
    # The two bytes that still land in the array.
    ax.annotate("2 in bounds", xy=(x(97), y2), xytext=(x(78), y2 - 6),
                ha="center", va="center", **font(13, "demibold", AMBER),
                arrowprops=dict(arrowstyle="-", color=AMBER, lw=1.4))

    # End-of-array line through every row.
    ax.plot([x(END), x(END)], [5, ym + hm + 3], color=INK, lw=1.6, ls=(0, (4, 3)))
    ax.text(x(END), ym + hm + 4.5, "end of array", ha="center", va="center", **font(13, "demibold", INK))

    # Offsets.
    for b, ha in ((0, "center"), (96, "right"), (98, "left"), (160, "center")):
        dx = {"right": -0.3, "left": 0.3}.get(ha, 0)
        ax.text(x(b) + dx, 3.0, str(b), ha=ha, va="center", **font(13, "medium", MUTED))
        ax.plot([x(b), x(b)], [4.6, 5.6], color=MUTED, lw=1)
    ax.plot([x(0), x(TOTAL)], [5.1, 5.1], color=FAINT, lw=1)
    ax.text(x0 - 1, 3.0, "byte offset", ha="right", va="center", **font(13, "medium", MUTED))

    fig.tight_layout(pad=0.4)
    fig.savefig("overflow.png", dpi=160, facecolor=fig.get_facecolor())
    plt.close()


if __name__ == "__main__":
    main()
