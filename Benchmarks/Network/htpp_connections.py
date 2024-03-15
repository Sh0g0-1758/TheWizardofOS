import requests

def herty(proc) :
    url = "https://example.com/"
    connections = []
    try:
        # Send a GET request
        response = requests.get(url)
        connections = proc.connections()

        # Check if the request was successful (status code 200)
        if response.status_code == 200:
            print("GET request successful!")
        else:
            print("Failed to retrieve data. Status code:", response.status_code)

    except requests.RequestException as e:
        print("Error:", e)
    return connections
    
