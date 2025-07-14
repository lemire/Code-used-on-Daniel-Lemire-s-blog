import matplotlib.pyplot as plt

# Set default font size
plt.rcParams.update({'font.size': 14})

# Data for Apple M2
m2_lanes = list(range(1, 41))
m2_ns_hit = [102.1, 50.4, 34.3, 27.4, 23.6, 20.0, 16.2, 14.2, 12.6, 11.0, 10.2, 9.4, 8.8, 7.8, 7.3, 6.9, 6.6, 6.2, 5.9, 5.7, 5.4, 5.2, 5.0, 4.8, 4.7, 4.6, 4.4, 4.3, 4.4, 4.3, 4.3, 4.3, 4.1, 4.3, 4.0, 3.9, 3.9, 3.9, 3.8, 3.8]
m2_bandwidth = [598, 1210, 1782, 2226, 2588, 3045, 3777, 4290, 4840, 5529, 5997, 6476, 6961, 7866, 8304, 8809, 9277, 9820, 10263, 10737, 11259, 11734, 12120, 12604, 13086, 13400, 13948, 14254, 13901, 14290, 14271, 14273, 15040, 14349, 15129, 15514, 15497, 15696, 16028, 15882]
m2_bandwidth = [2*x/1000 for x in m2_bandwidth]  # Double the M2 bandwidth values (128-bit cache line)
# Data for Apple M4
m4_lanes = list(range(1, 41))
m4_ns_hit = [94.3, 46.6, 31.1, 23.4, 18.8, 15.7, 13.5, 11.9, 10.6, 9.7, 8.8, 8.1, 7.5, 7.0, 6.6, 6.2, 5.8, 5.5, 5.3, 5.0, 4.8, 4.6, 4.4, 4.3, 4.1, 4.0, 3.8, 3.7, 4.0, 4.0, 4.1, 3.8, 3.7, 3.8, 3.6, 3.5, 3.5, 3.4, 3.3, 3.3]
m4_bandwidth = [647, 1309, 1960, 2608, 3244, 3883, 4508, 5126, 5741, 6324, 6924, 7504, 8112, 8666, 9185, 9859, 10437, 11000, 11572, 12116, 12661, 13222, 13775, 14332, 14865, 15395, 15905, 16419, 15236, 15207, 14847, 15888, 16467, 16259, 16856, 17507, 17456, 17780, 18530, 18469]
m4_bandwidth = [2*x/1000 for x in m4_bandwidth]  # Double the M4 bandwidth values (128-bit cache line)
# Plot 1: Number of lanes vs Delay (ns/hit) with logarithmic y-axis
fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(m2_lanes, m2_ns_hit, label='M2', marker='o')
ax.plot(m4_lanes, m4_ns_hit, label='M4', marker='s')
ax.set_xlabel('Number of lanes')
ax.set_ylabel('Delay (ns/hit)')
ax.set_title('Delay vs number of lanes (M2 vs M4)')
ax.set_yscale('log')
ax.set_xlim(left=0, right=28)
ax.legend(frameon=False)
ax.grid(True)
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
plt.savefig('delay_vs_lanes_english.png')
plt.close()

# Plot 2: Number of lanes vs Bandwidth
fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(m2_lanes, m2_bandwidth, label='M2', marker='o')
ax.plot(m4_lanes, m4_bandwidth, label='M4', marker='s')
ax.set_xlabel('Number of lanes')
ax.set_ylabel('Bandwidth (GB/s)')
ax.set_title('Bandwidth vs number of lanes (M2 vs M4)')
ax.set_xlim(left=0, right=28)
ax.legend(frameon=False)
ax.grid(True)
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
plt.savefig('bandwidth_vs_lanes_english.png')
plt.close()