#include "defaultIO.h"
#include <string>
#include<iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
string StandardIO::read() {
    string s;
    getline(cin, s);
    return s;
}
void StandardIO::write(string text) {
    cout << text;
}


SocketIO::SocketIO(int socket){
        this->socketID = socket;
}

string SocketIO::read() {
    // all the requriments for TCP read
    char buffer[4096];
    int bytesReceived = recv(this->socketID, buffer, 4096, 0);
    if (bytesReceived < 0) {
        cout << "Error receiving message" << endl;
        return "Error receiving message";
    }
    int index = 0;
    stringstream ss;
    while(buffer[index] != '\0')
    {
        ss << buffer[index];
        index++;
    }
    memset(buffer, 0, sizeof(buffer));
    string message = ss.str();
    return message;

}

void SocketIO::write(string message){
    // send through socket
    if (send(this->socketID, message.c_str(), message.size(), 0) < 0) {
            cout << "Error sending message " << endl;
    }
    return;
}
