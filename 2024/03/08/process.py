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

print(data["Conservative"] - data["Liberal"])

lib = data["Liberal"].sum()

print("b + l", (data["Bloc Québécois"] * data["Liberal"]).sum() / lib)
print("b - l", (data["Bloc Québécois"].sum() - (data["Bloc Québécois"] * data["Liberal"]).sum()) / lib)

print("lib", data["Liberal"].sum())

print("c + l", (data["Conservative"] * data["Liberal"]).sum() / lib)
print("c - l", (data["Conservative"].sum() - (data["Conservative"] * data["Liberal"]).sum())/lib)


print("n + l", (data["NDP"] * data["Liberal"]).sum()/lib)
print("n - l", (data["NDP"].sum() - (data["NDP"] * data["Liberal"]).sum())/lib)