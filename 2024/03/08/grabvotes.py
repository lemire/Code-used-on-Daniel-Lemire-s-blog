import os
import requests

def download(url: str, dest_folder: str):
    """
    Downloads a file from the given URL and saves it in the specified folder.

    Args:
        url (str): The URL of the file to download.
        dest_folder (str): The folder where the file should be saved.
    """
    if not os.path.exists(dest_folder):
        os.makedirs(dest_folder)  # Create the folder if it doesn't exist

    filename = ".".join(url.split('/')[-2:])
    file_path = os.path.join(dest_folder, filename)

    with requests.get(url, stream=True) as r:
        if r.ok:
            print(f"Saving to {os.path.abspath(file_path)}")
            with open(file_path, 'wb') as f:
                for chunk in r.iter_content(chunk_size=1024 * 8):
                    if chunk:
                        f.write(chunk)
                        f.flush()
                        os.fsync(f.fileno())
        else:
            # HTTP status code 4XX/5XX
            print(f"Download failed: status code {r.status_code}\n{r.text}")

# assume a recent version of Python.
# This script will grab all the votes from the 44th Parliament of Canada and save them to a folder.
for i in range(1, 853+1):
    url = f"https://www.ourcommons.ca/Members/en/votes/44/1/{i}/xml"
    download(url, "votes")