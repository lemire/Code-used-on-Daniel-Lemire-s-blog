import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update({'font.size': 14})

# Read data from alloc.txt
requested = []
usable = []

with open('alloc.txt', 'r') as f:
    lines = f.readlines()

# Skip header lines
data_started = False
for line in lines:
    line = line.strip()
    if 'Requested size | Actual usable size' in line:
        data_started = True
        continue
    if '---------------' in line and data_started:
        continue
    if data_started and '|' in line:
        parts = line.split('|')
        if len(parts) == 2:
            try:
                req = int(parts[0].strip())
                act = int(parts[1].strip())
                requested.append(req)
                usable.append(act)
            except ValueError:
                continue
    if 'Total requested:' in line:
        break

# Convert to numpy arrays
requested = np.array(requested)
usable = np.array(usable)

# Plot
plt.figure(figsize=(10, 6))
ax = plt.gca()
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
plt.plot(requested, requested, label='usable', color='blue')  # Ideal case: actual = requested
plt.plot(requested, usable, label='total', color='red')      # Actual allocated
plt.xlabel('requested memory')
plt.ylabel('actual memory')
#plt.title('Memory Allocation Overhead')
plt.legend(frameon=False)
plt.savefig('plot.png', bbox_inches='tight')
plt.show()