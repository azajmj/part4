#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <cctype>
#include "knn.h"
#include "server.h"
#include "CLI.h"
#include <sstream>
#include <thread>
#include "defaultIO.h"
// #include <cstudio>
#define MAX_MSG 100
// gets port at init
Server::Server(int port) {
    this->port = port;
}
Server::~Server() {}
// handle client with a CLI object, when it returns ie client done, we close sock, and return to listen 
void Server::handleClient(DefaultIO* dio, int client_sock) {
    CLI cli(dio);
    cli.start();
    close(client_sock);
    delete dio;
    return;
}
// start method runs the server
void Server::start() {
    try{
        //create server 
        const int server_port = port;
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            cout <<"error creating socket";
        }
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(server_port);
        if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            cout << "error binding socket";
        }
        if (listen(sock, 5)) {
            cout << "error listening to socket";
        }
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        // accept clients
        while(true) {
            // accept
            int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
            //create io object
            DefaultIO* dio = new SocketIO(client_sock);
            // create thread
            thread t(&Server::handleClient, this, dio, client_sock);
            // add to threads
            threads.push_back(move(t));
            // fixs compilation error - not sure why?
            threads.back().detach();
            
        }
        // join threads
        for (auto &t : threads) {
            t.join();
        }
        close(sock);
    } catch (const exception &e) {
    cerr << e.what() << endl;
    }
    return;
   
}

int main(int argsc, char *argv[]) {
    int port = atoi(argv[1]);
    // add check arguments
    Server server(port);
    try{
        server.start();
    } catch (...) {
        cout << "ERROR in server\n";
    }
    return 0;
}


// old server code not deleted yet...

// int main(int argc, char *argv[]) {
//     try {
//         // Check that there are exactly 2 arguments
//         if (argc != 3) {
//             throw invalid_argument("Expected 2 arguments: <dataset_file> <server_port>");
//         }
//         // Check that the first argument is a string
//         string file(argv[1]);
//         // Check that the second argument is an integer
//         int port = atoi(argv[2]);
//         if (port == 0) {
//             throw invalid_argument("Expected an integer value for <server_port>");
//         }
//         const int server_port = port;
//         int sock = socket(AF_INET, SOCK_STREAM, 0);
//         if (sock < 0) {
//             //perror("error creating socket");
//         }
//         struct sockaddr_in sin;
//         memset(&sin, 0, sizeof(sin));
//         sin.sin_family = AF_INET;
//         sin.sin_addr.s_addr = INADDR_ANY;
//         sin.sin_port = htons(server_port);
//         if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
//             //perror("error binding socket");
//         }
//         if (listen(sock, 5)) {
//             //perror("error listening to socket");
//         }
//         struct sockaddr_in client_sin;
//         unsigned int addr_len = sizeof(client_sin);
//     while(true) {
//         int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
//         if (client_sock < 0) {
//             //perror("error accepting client");
//         }
//         while (true) {
//             char buffer[4096];
//             int expected_data_len = sizeof(buffer);
//             int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
//             if (read_bytes == 0) {
//                 //perror("error lost connection to client");
//                 break;
//             } else if (read_bytes < 0) {
//                 break;
//                 //perror("error reading from socket");
//             } else {

//                 //cout << buffer << endl;

//                 // Deserialize the data
//                 stringstream deserialized_data(buffer);
//                 int vector_size;
//                 vector<double> vec;
//                 string pred = "";
//                 deserialized_data >> vector_size;
//                 //move to the next client
//                 if (vector_size == -1) {
//                     close(client_sock);
//                     break;
//                 }
//                 for (int i = 0; i < vector_size; i++) {
//                     double number;
//                     deserialized_data >> number;
//                     vec.push_back(number);
//                 }
//                 string dist;
//                 int k;
//                 deserialized_data >> dist;
//                 deserialized_data >> k;
//                 // Perform KNN classification:
//                 try {
//                     KNNClassifier model(k, file, dist);
//                     if (model.getTrainSize() > k) {
//                         if (model.getSampleSize() == vec.size()) {
//                             pred = model.predict(vec);
//                             if (!pred.empty()) {
//                                 for (int i = 0; i < pred.size(); i++) {
//                                     buffer[i] = pred[i];
//                                 }
//                                 buffer[pred.size()] = '\0';
//                             } else {
//                                 strncpy(buffer, "no prediction", sizeof(buffer));
//                             }
//                         } else {
//                             strncpy(buffer, "invalid input", sizeof(buffer));
//                         }
//                     } else {
//                         strncpy(buffer, "invalid input", sizeof(buffer));
//                     }
//                 } catch (...) {
//                     strncpy(buffer, "invalid input", sizeof(buffer));
//                 }
//             }
//             //cout << buffer << endl;
//             int sent_bytes = send(client_sock, buffer, read_bytes, 0);
//             if (sent_bytes < 0) {
//                 //perror("error sending to client");
//             }
//         }
//     }
//         close(sock);
//     }
//     catch (const exception &e) {
//         cerr << e.what() << endl;
//         return 1;
//     }


//     return 0;
// }
