import json

def get_user_wealth_from_json(file_path):
    try:
        # Open and load the JSON file
        with open(file_path, 'r') as file:
            data = json.load(file)
        
        # Navigate through the JSON structure to find user wealth
        wealth_str = data['user']['wealth']
        
        # Convert the string to float
        wealth = float(wealth_str)
        
        return wealth
    
    except FileNotFoundError:
        print(f"Error: The file at {file_path} was not found.")
        return None
    except KeyError as e:
        print(f"Error: Key not found in JSON structure. {str(e)}")
        return None
    except ValueError:
        print(f"Error: The value at 'user/wealth' could not be converted to a float.")
        return None
    except json.JSONDecodeError:
        print("Error: The file does not contain valid JSON.")
        return None

import sys
file_path = sys.argv[1]
wealth = get_user_wealth_from_json(file_path)

if wealth is not None:
    print(f"The user's wealth is: {wealth}")
