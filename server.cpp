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
// #include <cstudio>

#define MAX_MSG 100

int main(int argc, char *argv[]) {
    int port = atoi(argv[2]);
    string file(argv[1]);
    const int server_port = port;
    int sock  = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);
    if (bind(sock, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
    }
    if (listen(sock, 5)) {
        perror("error listening to socket");
    }
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    while (true){
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
        if (client_sock < 0){
            perror("error accepting client");
        }
        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        int read_bytes = recv(client_sock, buffer, expected_data_len,0);
        if (read_bytes == 0) {
            perror("error lost connection to cleint");
        }
        else if (read_bytes < 0) {
            perror("error reading from socket");
        }
        else {
            // cout <<"the client sent: " << buffer << endl;
            // int len = sizeof(buffer) / sizeof(buffer[0]);
            // for(int i = 0; i < len; i++){
            //     buffer[i] = toupper(buffer[i]);
            // }

            // get parameters from client buffer
            int flag = 0;
            int k;
            string dist = "";
            string pred = "";
            vector<double> test;
            try {
                vector<string> tokens;
                char* token = strtok(buffer, " ");
                while (token != NULL) {
                    string s = token;
                    tokens.push_back(s);
                    token = strtok(NULL, " ");
                }
                k = stoi(tokens[tokens.size() - 1]);
                dist = (tokens[tokens.size() - 2]);
                for(int i = 0; i < tokens.size() - 2; i++) {
                    double v = stof(tokens[i]);
                    test.push_back(v);
                }
                    } catch (...) {
                        perror("invalid input");
                        flag = 1;
                        }
            // knn
            if (flag == 0){
                KNNClassifier model(k, file, dist);
                if (model.getSampleSize() == test.size()) {
                    pred = model.predict(test);
                    if (!pred.empty()) {
                        for (int i = 0; i < pred.size(); i++){
                            buffer[i] = pred[i];
                        }
                        buffer[pred.size()] = '\0';
                    } else {
                        perror("no prediction");
                    } 
                } else {
                    perror("test vector invalid size");
                }
            }
        }
        int sent_bytes = send(client_sock, buffer, read_bytes, 0);
        if (sent_bytes < 0) {
            perror("error sending to client");
        }
    }
    close(sock);
    return 0;

}