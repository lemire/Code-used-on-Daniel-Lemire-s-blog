
def plot(data, unit = ''):
    max_value = max(count for _, count in data)
    increment = max_value / 25

    longest_label_length = max(len(label) for label, _ in data)

    for label, count in data:

        # The ASCII block elements come in chunks of 8, so we work out how
        # many fractions of 8 we need.
        # https://en.wikipedia.org/wiki/Block_Elements
        bar_chunks, remainder = divmod(int(count * 8 / increment), 8)

        # First draw the full width chunks
        bar = '█' * bar_chunks

        # Then add the fractional part.  The Unicode code points for
        # block elements are (8/8), (7/8), (6/8), ... , so we need to
        # work backwards.
        if remainder > 0:
            bar += chr(ord('█') + (8 - remainder))

        # If the bar is empty, add a left one-eighth block
        bar = bar or  '▏'

        print(f'{label.rjust(longest_label_length)} ▏ {count}{unit} {bar}')


plot([
    ('graviton 3',            2.6),
    ('graviton 4',          2.8),
], ' GHz')

print()
print("URL parsing")
plot([
    ('graviton 3',            3.8),
    ('graviton 4',          5.9)], ' Murl/s')


print()
print("Unicode Validation")
plot([
    ('graviton 3',            14),
    ('graviton 4',          19)], ' GB/s')


print()

print("simdjson")
plot([
    ('graviton 3',            3.6),
    ('graviton 4',          4.6)], ' GB/s')
print()

print("base64")
plot([
    ('graviton 3',            2.8),
    ('graviton 4',          3.5)], ' GB/s')
print()

print("number parsing")
plot([
    ('graviton 3',            1.0),
    ('graviton 4',          1.3)], ' GB/s')
