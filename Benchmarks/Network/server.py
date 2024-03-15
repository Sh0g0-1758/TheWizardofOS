import socket

# Create a TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the server address (IP and port)
server_address = ('localhost', 9999)

# Bind the socket to the server address
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen(5)

print("Server is listening on", server_address)

while True:
    # Wait for a connection
    print("Waiting for a connection...")
    client_socket, client_address = server_socket.accept()

    try:
        print("Connection from", client_address)

        # Receive data from the client
        data = client_socket.recv(1024)
        print("Received:", data.decode())

        # Send back a response
        response = "Message received!"
        client_socket.sendall(response.encode())

    finally:
        # Clean up the connection
        client_socket.close()
