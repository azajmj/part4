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
    char buffer[4096];
        // we will use the recv function:
        // the data we received will save in the buffer, and the return value is the size.
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

    // string str = "";
    // char ch = 0;
    // ::read(clientID, &ch, sizeof(char));
    // while (ch != '\n') {
    //     str += ch;
    //     ::read(clientID, &ch, sizeof(char));
    // }
    // return str;

}

void SocketIO::write(string message){
    if (send(this->socketID, message.c_str(), message.size(), 0) < 0) {
            cout << "Error sending message " << endl;
    }
    return;
    // ::write(clientID, text.c_str(), text.length());
}
