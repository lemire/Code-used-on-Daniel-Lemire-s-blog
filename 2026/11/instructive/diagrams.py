"""Draws the three diagrams of the post as SVG: python3 diagrams.py, then see convert in README."""

FONT = "Helvetica, Arial, sans-serif"
MONO = "Menlo, Consolas, monospace"
INK = "#1f2933"
MUTED = "#6b7785"
MEM = "#e4e7eb"      # memory
GPR = "#cfe0f5"      # general-purpose register work
GPR_EDGE = "#4a78b5"
VEC = "#d3efe6"      # vector register work
VEC_EDGE = "#2f8f74"
HOT = "#fde2dc"      # where the time goes
HOT_EDGE = "#c8412b"


def svg(width, height, body):
    return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
            f'viewBox="0 0 {width} {height}" font-family="{FONT}">\n'
            f'<defs>'
            f'<marker id="a" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse">'
            f'<path d="M0,0 L10,5 L0,10 z" fill="{MUTED}"/></marker>'
            f'<marker id="r" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse">'
            f'<path d="M0,0 L10,5 L0,10 z" fill="{HOT_EDGE}"/></marker>'
            f'</defs>\n'
            f'<rect width="{width}" height="{height}" fill="white"/>\n{body}</svg>\n')


def box(x, y, w, h, fill, edge, label, mono=True, size=13, weight="normal", color=INK, rx=6):
    family = MONO if mono else FONT
    return (f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" stroke="{edge}" stroke-width="1.5"/>'
            f'<text x="{x + w / 2}" y="{y + h / 2 + size * 0.35}" text-anchor="middle" font-family="{family}" '
            f'font-size="{size}" font-weight="{weight}" fill="{color}">{label}</text>\n')


def text(x, y, s, size=14, anchor="start", color=INK, weight="normal", mono=False, style="normal"):
    family = MONO if mono else FONT
    return (f'<text x="{x}" y="{y}" text-anchor="{anchor}" font-family="{family}" font-size="{size}" '
            f'font-weight="{weight}" font-style="{style}" fill="{color}">{s}</text>\n')


def arrow(x1, y1, x2, y2, red=False, width=1.5, dash=None):
    color = HOT_EDGE if red else MUTED
    d = f' stroke-dasharray="{dash}"' if dash else ""
    return (f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" stroke-width="{width}"{d} '
            f'marker-end="url(#{"r" if red else "a"})"/>\n')


def cells(x, y, w, h, n, fill, edge, labels=None, size=12):
    """A register drawn as a bar of n lanes."""
    out = f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="4" fill="{fill}" stroke="{edge}" stroke-width="1.5"/>\n'
    lane = w / n
    for i in range(1, n):
        out += f'<line x1="{x + i * lane}" y1="{y}" x2="{x + i * lane}" y2="{y + h}" stroke="{edge}" stroke-width="0.8"/>\n'
    if labels:
        for i, s in enumerate(labels):
            out += (f'<text x="{x + (i + 0.5) * lane}" y="{y + h / 2 + size * 0.35}" text-anchor="middle" '
                    f'font-family="{MONO}" font-size="{size}" fill="{INK}">{s}</text>\n')
    return out


def legend(x, y, items):
    out = ""
    for fill, edge, label in items:
        out += f'<rect x="{x}" y="{y - 11}" width="14" height="14" rx="3" fill="{fill}" stroke="{edge}" stroke-width="1.5"/>'
        out += text(x + 20, y, label, size=13, color=MUTED)
        x += 34 + 6.6 * len(label)
    return out


def m4_jdk25():
    W, H = 960, 560
    body = text(30, 36, "JDK 25 on Apple M4: one word at a time, each waiting for the previous one", 18, weight="bold")
    steps = [  # label, fill, edge
        ("ldr a, b", MEM, "#9aa5b1"),
        ("and x, a, b", GPR, GPR_EDGE),
        ("mov v16.d[0], x", HOT, HOT_EDGE),
        ("cnt v16.8b", VEC, VEC_EDGE),
        ("addv b16", VEC, VEC_EDGE),
        ("mov x, v16.d[0]", HOT, HOT_EDGE),
        ("add sum, sum, x", GPR, GPR_EDGE),
    ]
    cols = 4
    bw, bh, gapy = 170, 34, 20
    x0, y0, gapx = 40, 90, 72
    for c in range(cols):
        x = x0 + c * (bw + gapx)
        body += text(x + bw / 2, y0 - 14, ["word k", "word k+1", "word k+2", "word k+3"][c], 14, "middle", MUTED, "bold")
        for s, (label, fill, edge) in enumerate(steps):
            y = y0 + s * (bh + gapy)
            body += box(x, y, bw, bh, fill, edge, label)
            if s:
                body += arrow(x + bw / 2, y - gapy, x + bw / 2, y - 2)
        if c + 1 < cols:
            # addv of this word -> insert of the next word, through v16
            ya = y0 + 4 * (bh + gapy) + bh / 2
            yi = y0 + 2 * (bh + gapy) + bh / 2
            body += (f'<path d="M{x + bw},{ya} C{x + bw + 40},{ya} {x + bw + gapx - 40},{yi} {x + bw + gapx - 2},{yi}" '
                     f'fill="none" stroke="{HOT_EDGE}" stroke-width="2.5" marker-end="url(#r)"/>\n')
            body += text(x + bw + gapx / 2 + 7, (ya + yi) / 2 + 5, "v16", 13, "start", HOT_EDGE, "bold", mono=True)
            # the running sum, a cheap one-cycle dependency
            ys = y0 + 6 * (bh + gapy) + bh / 2
            body += arrow(x + bw + 2, ys, x + bw + gapx - 2, ys, dash="4 3")
    y = y0 + len(steps) * (bh + gapy) + 14
    body += text(40, y, "The first mov is an insert: it writes the low half of v16 and keeps the high half, so it must wait for the", 14)
    body += text(40, y + 22, "previous addv. The red chain (insert, cnt, addv) runs through every word: about 8 cycles per word.", 14)
    body += legend(40, y + 58, [(MEM, "#9aa5b1", "memory"), (GPR, GPR_EDGE, "general-purpose register"),
                                (VEC, VEC_EDGE, "vector register"), (HOT, HOT_EDGE, "most samples land here")])
    return svg(W, H, body)


def m4_jdk27():
    W, H = 960, 620
    body = text(30, 36, "JDK 27 on Apple M4: two words at a time, in 128-bit vector registers", 18, weight="bold")
    x, bw, bh = 290, 400, 32
    rows = [
        ("ldr q", "load a[k], a[k+1]", lambda y: cells(x, y, bw, bh, 2, MEM, "#9aa5b1", ["a[k]", "a[k+1]"])),
        ("ldr q", "load b[k], b[k+1]", lambda y: cells(x, y, bw, bh, 2, MEM, "#9aa5b1", ["b[k]", "b[k+1]"])),
        ("and .16b", "AND, 128 bits at once", lambda y: cells(x, y, bw, bh, 2, VEC, VEC_EDGE, ["a&amp;b", "a&amp;b"])),
        ("cnt .16b", "bits in each of 16 bytes", lambda y: cells(x, y, bw, bh, 16, VEC, VEC_EDGE, ["4", "3", "5", "4", "2", "6", "4", "3", "4", "5", "3", "4", "4", "2", "5", "3"], 11)),
        ("uaddlp", "add pairs: 8 counts", lambda y: cells(x, y, bw, bh, 8, VEC, VEC_EDGE, ["7", "9", "8", "7", "9", "7", "6", "8"])),
        ("uaddlp", "add pairs: 4 counts", lambda y: cells(x, y, bw, bh, 4, VEC, VEC_EDGE, ["16", "15", "16", "14"])),
        ("uaddlp", "add pairs: 1 count per word", lambda y: cells(x, y, bw, bh, 2, VEC, VEC_EDGE, ["31", "30"])),
        ("xtn", "narrow to 32 bits", lambda y: cells(x, y, bw / 2, bh, 2, VEC, VEC_EDGE, ["31", "30"])),
        ("add .2s", "add to running totals", lambda y: cells(x, y, bw / 2, bh, 2, VEC, VEC_EDGE, ["Σ", "Σ"])),
    ]
    y0, gap = 80, 22
    for i, (op, what, draw) in enumerate(rows):
        y = y0 + i * (bh + gap)
        body += text(x - 20, y + bh / 2 + 5, op, 14, "end", INK, "bold", mono=True)
        body += text(x + bw + 20, y + bh / 2 + 5, what, 14, color=MUTED)
        body += draw(y)
        if i >= 2:
            body += arrow(x + 100, y - gap, x + 100, y - 2)
    # the two loads both feed the AND
    y2 = y0 + 2 * (bh + gap)
    body += arrow(x + bw - 60, y0 + bh + gap + bh, x + bw - 60, y2 - 2)
    y = y0 + len(rows) * (bh + gap) + 10
    body += text(40, y, "Eight such blocks per iteration: 16 words. The words never leave the vector registers, and the blocks", 14)
    body += text(40, y + 22, "do not depend on each other, so the processor overlaps them.", 14)
    return svg(W, H, body)


def xeon():
    W, H = 1000, 420
    body = text(30, 36, "Intel Xeon (AVX-512), JDK 25 and 27: eight words at a time, in 512-bit registers", 18, weight="bold")
    x, bw, bh = 200, 560, 34
    words = [f"a[k+{i}]" if i else "a[k]" for i in range(8)]
    rows = [
        ("vmovdqu32", "load 8 words", lambda y: cells(x, y, bw, bh, 8, MEM, "#9aa5b1", words, 11)),
        ("vpandd", "AND with 8 words of b", lambda y: cells(x, y, bw, bh, 8, VEC, VEC_EDGE, ["a&amp;b"] * 8, 11)),
        ("vpopcntq", "count the bits of each word", lambda y: cells(x, y, bw, bh, 8, VEC, VEC_EDGE, ["33", "30", "31", "35", "29", "32", "34", "31"])),
        ("vpmovqd", "narrow to 32 bits", lambda y: cells(x, y, bw / 2, bh, 8, VEC, VEC_EDGE, ["33", "30", "31", "35", "29", "32", "34", "31"], 11)),
        ("vpaddd", "add to running totals", lambda y: cells(x, y, bw / 2, bh, 8, VEC, VEC_EDGE, ["Σ"] * 8, 11)),
    ]
    y0, gap = 80, 26
    for i, (op, what, draw) in enumerate(rows):
        y = y0 + i * (bh + gap)
        body += text(x - 20, y + bh / 2 + 5, op, 14, "end", INK, "bold", mono=True)
        body += text(x + bw + 16, y + bh / 2 + 5, what, 13, color=MUTED) if i < 3 else text(x + bw / 2 + 16, y + bh / 2 + 5, what, 13, color=MUTED)
        body += draw(y)
        if i:
            body += arrow(x + 70, y - gap, x + 70, y - 2)
    y = y0 + len(rows) * (bh + gap) + 10
    body += text(40, y, "One instruction (vpopcntq) counts the bits of eight words. Eight such blocks per iteration: 64 words.", 14)
    return svg(W, H, body)


if __name__ == "__main__":
    for name, f in [("m4_jdk25", m4_jdk25), ("m4_jdk27", m4_jdk27), ("xeon", xeon)]:
        with open(name + ".svg", "w") as out:
            out.write(f())
