import sys
import socket
import os

BUFFER_SIZE = 1024

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <server_ip_address> <COMMAND>")
        return 1

    server_ip = sys.argv[1]
    port = 12345
    command = sys.argv[2]

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((server_ip, port))
    except Exception as e:
        print("Failed to connect to the server:", e)
        client_socket.close()
        return 1

    try:
        client_socket.sendall(command.encode())

        if command.startswith("LIST"):
            list_buffer = client_socket.recv(BUFFER_SIZE).decode()
            print("File list from server:\n", list_buffer)

        elif command.startswith("DOWNLOAD"):
            file_name = command.split("/")[-1]
            with open(file_name, "wb") as output_file:
                while True:
                    data = client_socket.recv(BUFFER_SIZE)
                    if not data:
                        break
                    output_file.write(data)
            print(f"File '{file_name}' downloaded successfully.")

        else:
            file_to_send = command.split("/")[-1]
            with open(file_to_send, "rb") as input_file:
                while True:
                    data = input_file.read(BUFFER_SIZE)
                    if not data:
                        break
                    client_socket.sendall(data)
            print(f"File '{file_to_send}' sent successfully.")

    except Exception as e:
        print("Error:", e)

    finally:
        client_socket.close()

if __name__ == "__main__":
    main()
