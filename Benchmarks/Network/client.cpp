#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

const int BUFFER_SIZE = 1024;
//----WITH LIST FUNCTION


int main(int argc, char* argv[]) {
    //std::cout<<"Give command like   --->  {  ./clientA 'ip-address' 'COMMAND'  } \n  COMMAND:\n UPLOAD/<fileinClientDirectory> \n DOWNLOAD/<fileinServerDirectory> \n LIST  ";
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip_address> <COMMAND>\n";
        return 1;
    }

    const char* serverIP = argv[1];
    int port = std::stoi("12345");
    //const char* ToDo =argv[3];
    const char* fileToSend = argv[2];
   

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to connect to the server");
        close(clientSocket);
        return 1;
    }
//    if (send(clientSocket, ToDo, strlen(ToDo), 0) == -1) {
//         perror("Failed to send ToDo");
//         close(clientSocket);
//         return 1;
//     }
    // Send the file name as the first chunk
    if (send(clientSocket, fileToSend, strlen(fileToSend), 0) == -1) {
        perror("Failed to send file name");
        close(clientSocket);
        return 1;
    }
    std::string Do;
    std::string file;
    int fist=0;
    for(int i=0;i<256;i++){
        if(fileToSend[i]=='/'|| fileToSend[i]=='\0'){
         fist=i;
          break;
        }
        Do =Do+fileToSend[i];
    }
    for(int i=fist+1;i<256;i++){
        if( fileToSend[i]=='\0'){
            
          break;
        }
      file=file+fileToSend[i];
    }
     const char* fileN = file.c_str();
if (Do=="LIST") {
    char listBuffer[1024];
    ssize_t byteRead = recv(clientSocket, listBuffer, sizeof(listBuffer), 0);
    if (byteRead == -1) {
        std::cerr << "Receive failed" << std::endl;
        close(clientSocket);
        return 1;
    }

    if (byteRead == 0) {
        std::cout << "Server disconnected." << std::endl;
        return 1;
    }

    listBuffer[byteRead] = '\0'; // Null-terminate the received data
    std::cout << "File list from server:\n" << listBuffer;

    // Close the socket after printing the list
    close(clientSocket);

    return 1;
}
else if(Do=="DOWNLOAD"){
    // Send the file name as the request
    const char* fileName = fileN; // Modify this with the desired filename
    if (send(clientSocket, fileName, strlen(fileName), 0) == -1) {
        perror("Failed to send file request");
        close(clientSocket);
        return 1;
    }

    // Receive the file data from the server and save it
    std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);
    if (!outputFile.is_open()) {
        perror("Error opening file for download");
        close(clientSocket);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        outputFile.write(buffer, bytesRead);
    }

    outputFile.close();
    std::cout << "File '" << fileName << "' downloaded successfully.\n";

    close(clientSocket);
    return 0;
}
else{
    // Open the file to send
    std::ifstream inputFile(fileN, std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file.\n";
        close(clientSocket);
        return 1;
    }

    // Send file data to server in chunks
    char buffer[BUFFER_SIZE];
    while (!inputFile.eof()) {
        inputFile.read(buffer, sizeof(buffer));
        int bytesRead = inputFile.gcount();
        if (bytesRead > 0) {
            if (send(clientSocket, buffer, bytesRead, 0) == -1) {
                perror("Failed to send file data");
                inputFile.close();
                close(clientSocket);
                return 1;
            }
        }
    }

    inputFile.close();
    std::cout << "File '" << fileN << "' sent successfully.\n";
}
    close(clientSocket);
    return 0;
}
