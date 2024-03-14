import socket

def run_client(proc):
    # Create a TCP/IP socket
    connections = []
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Define the server address (IP and port)
    server_address = ('localhost', 9999)

    # Connect the socket to the server
    client_socket.connect(server_address)

    try:
        # Send data to the server
        connections = proc.connections()
        message = "Hello, server!"
        print("Sending:", message)
        client_socket.sendall(message.encode())

        # Receive a response from the server
        response = client_socket.recv(1024)
        print("Received:", response.decode())

    finally:
        # Clean up the connection
        client_socket.close()
    
    return connections