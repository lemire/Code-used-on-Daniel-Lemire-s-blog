import re
lanes = 1
with open('raw.txt', 'r') as f:
    for line in f:
        if line.startswith('timing'):
            min = float(re.search(r'min:\s+(\d+\.\d+)', line).group(1))
            mean = float(re.search(r'mean:\s+(\d+\.\d+)', line).group(1))
            std = float(re.search(r'std:\s+(\d+\.\d+)', line).group(1))
            max = float(re.search(r'max :\s+(\d+\.\d+)', line).group(1))
            print(lanes, min, mean, std, max)
            lanes += 1
