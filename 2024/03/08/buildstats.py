import os
import xml.etree.ElementTree as ET

def read_xml_file(file_path: str):
    """
    Reads an XML file and processes the data.

    Args:
        file_path (str): Path to the XML file.
    """
    if not os.path.exists(file_path):
        print(f"Error: File '{file_path}' does not exist.")
        return

    try:
        tree = ET.parse(file_path)
        root = tree.getroot()

        # Initialize a dictionary to store Yea votes by CaucusShortName
        yea_votes_by_caucus = {}

        for participant in root.findall("VoteParticipant"):
            caucus_name = participant.find("CaucusShortName").text
            is_vote_yea = participant.find("IsVoteYea").text

            is_vote_yea = is_vote_yea == "true"

            if caucus_name not in yea_votes_by_caucus:
                yea_votes_by_caucus[caucus_name] = 0
            if is_vote_yea:
                yea_votes_by_caucus[caucus_name] += 1

        # Calculate the total votes for each CaucusShortName
        total_votes_by_caucus = {caucus: sum_votes for caucus, sum_votes in yea_votes_by_caucus.items()}

        # Calculate the percentage of Yea votes
        answer = {}
        for caucus, yea_votes in yea_votes_by_caucus.items():
            total_votes = total_votes_by_caucus[caucus]
            percentage_yea = (yea_votes / total_votes) * 100 if total_votes > 0 else 0
            answer[caucus] = percentage_yea/100.0
        return answer

    except ET.ParseError:
        print(f"Error: Unable to parse XML file '{file_path}'.")



def list_xml_files(directory_path: str):
    """
    Lists all XML files in the specified directory.

    Args:
        directory_path (str): Path to the directory containing XML files.
    """
    if not os.path.exists(directory_path):
        print(f"Error: Directory '{directory_path}' does not exist.")
        return

    xml_files = [filename for filename in os.listdir(directory_path) if filename.lower().endswith('.xml')]

    if not xml_files:
        print(f"No XML files found in '{directory_path}'.")
    else:
       return xml_files
print("loading xml data")
arrays = {}
count = 0
for file in list_xml_files("votes"):
    a = read_xml_file(f"votes/{file}")
    for key in a:
        if key in arrays:
            arrays[key].append(a[key])
        else:
            arrays[key] = [0 for i in range(count)]
            arrays[key].append(a[key])
    for key in arrays:
        if key not in a:
            arrays[key].append(0)
    count += 1

print("constructing data frame")
import pandas as pd
del arrays['Independent']
data = pd.DataFrame(arrays)
print(data.corr())


import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler  # Import StandardScaler

# Standardize the data
scaler = StandardScaler()
scaled_data = scaler.fit_transform(data.transpose())

# Apply PCA
pca = PCA(n_components=2)
principal_components = pca.fit_transform(scaled_data)
print(pca.explained_variance_ratio_)
# Create a DataFrame with the principal components
pc_df = pd.DataFrame(data=principal_components, columns=['PC1', 'PC2'])
pc_df['Caucus'] = data.columns
print(pc_df.head(10))

# Plot the original columns projected onto the principal components
plt.figure(figsize=(8, 6))
plt.xlim(-30, 30)  # Set x-axis limits from -30 to 30
plt.ylim(-30, 30) 
plt.scatter(pc_df['PC1'], pc_df['PC2'], alpha=0.6)

for xi, yi, label in pc_df[['PC1', 'PC2', 'Caucus']].values:
    plt.text(xi, yi, label, va='bottom', ha='center')  # Label each point with its value
plt.xlabel('Principal Component 1')
plt.ylabel('Principal Component 2')
plt.title('Federal caucus votes  projected in 2D (44th parliament of Canada)')
plt.grid(True)
plt.savefig('44parliament.png')
print("See 44parliament.png")
print("done")





# Apply PCA
pca = PCA(n_components=3)
principal_components = pca.fit_transform(scaled_data)
print(pca.explained_variance_ratio_)
# Create a DataFrame with the principal components
pc_df = pd.DataFrame(data=principal_components, columns=['PC1', 'PC2', 'PC3'])
pc_df['Caucus'] = data.columns
print(pc_df.head(10))

# Plot the original columns projected onto the principal components
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(projection='3d')
ax.set_xlim(-30, 30)  # Set x-axis limits from -30 to 30
ax.set_ylim(-30, 30) 
ax.set_zlim(-30, 30) 
ax.scatter(pc_df['PC1'], pc_df['PC2'], pc_df['PC3'], alpha=0.6)

for xi, yi, zi, label in pc_df[['PC1', 'PC2', 'PC3', 'Caucus']].values:
    ax.text(xi, yi, zi, label, va='bottom', ha='center')  # Label each point with its value
plt.title('Federal caucus votes  projected in 3D (44th parliament of Canada)')
plt.grid(True)
plt.savefig('44parliament3d.png')
print("See 44parliament3d.png")
print("done")