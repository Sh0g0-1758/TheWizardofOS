    #include <iostream>
    #include <fstream>
    #include <string>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <cstring>
    #include <dirent.h>
    /// 
    /// FINAL-----With List Function
    // Function to list files in the server's directory
    std::string listFiles() {
        std::string fileList;
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(".")) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                if (ent->d_type == DT_REG) {
                    fileList += ent->d_name;
                    fileList += "\n";
                }
            }
            closedir(dir);
        } else {
            perror("opendir");
        }

        return fileList;
    }
    int main() {
        int serverSocket, newSocket;
        struct sockaddr_in serverAddr;
        struct sockaddr_storage serverStorage;
        socklen_t addr_size;

        // Create socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            perror("Error creating socket");
            exit(1);
        }

        // Bind socket to a port
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345); // Choose a port number
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Error binding");
            exit(1);
        }
         while(true){
        // Listen for incoming connections
        if (listen(serverSocket, 10) == 0) {
            printf("Server listening...\n");
        } else {
            perror("Error listening");
            exit(1);
        }

        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);
        if (newSocket < 0) {
            perror("Error accepting connection");
            exit(1);
        }
    //    char Do[256]; // Adjust the buffer size as needed
    //     int byteRead = recv(newSocket, Do, sizeof(Do), 0);
    //     if (byteRead < 0) {
    //         perror("Error receiving TODO");
    //         exit(1);
    //     }
    //     Do[byteRead] = '\0';
        // Receive the file name as the first chunk
        char fileName[256]; // Adjust the buffer size as needed
        int bytesRead = recv(newSocket, fileName, sizeof(fileName), 0);
        if (bytesRead < 0) {
            perror("Error receiving file name");
            exit(1);
        }
        fileName[bytesRead] = '\0';
        std::string Do;
        std::string file;
        int fist=0;
        for(int i=0;i<256;i++){
            if(fileName[i]=='/'|| fileName[i]=='\0'){
            fist=i;
            break;
            }
            Do =Do+fileName[i];
        }
        for(int i=fist+1;i<256;i++){
            if( fileName[i]=='\0'){
                
            break;
            }
        file=file+fileName[i];
        }
        const char* fileN = file.c_str();
        //std::cout<<Do<<std::endl;
        //std::cout<<file<<std::endl;

        // Check if the client requests file listing
                if (Do=="LIST") {
                    std::string fileList = listFiles();
                    ssize_t bytesSent = send(newSocket, fileList.c_str(), fileList.size(), 0);
                    if (bytesSent == -1) {
                        std::cerr << "Send failed" << std::endl;
                        close(newSocket);
                        close(serverSocket);
                        return 1;
                    }
                   
                }
                else if(Do=="DOWNLOAD"){
            const char* fileName=fileN;
        std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file for download.\n";
            close(newSocket);
            close(serverSocket);
            return 1;
        }

        // Send file data to client in chunks
        char buffer[1024];
        while (!inputFile.eof()) {
            inputFile.read(buffer, sizeof(buffer));
            int bytesRead = inputFile.gcount();
            if (bytesRead > 0) {
                if (send(newSocket, buffer, bytesRead, 0) == -1) {
                    perror("Failed to send file data");
                    inputFile.close();
                    close(newSocket);
                    close(serverSocket);
                    return 1;
                }
            }
        }

        inputFile.close();
        std::cout << "File '" << fileName << "' sent successfully.\n";
        close(newSocket);
        
                }else if(Do=="STOP"){
                    std::cout<<"Server has been closed successfully.\n";
                    break;
                }else{

        // Open the file to write
        std::ofstream outputFile(fileN, std::ios::out | std::ios::binary);
        if (!outputFile.is_open()) {
            perror("Error opening file");
            exit(1);
        }

        // Receive file data from client
        char buffer[1024];
        while ((bytesRead = recv(newSocket, buffer, sizeof(buffer), 0)) > 0) {
            outputFile.write(buffer, bytesRead);
        }

        outputFile.close();
        std::cout << "File received and stored as '" << file << "'.\n";
                }
         }
        close(newSocket);
        close(serverSocket);
        return 0;
    }
