#include <iostream>
#include <string>
#include <sys/socket.h>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
int main() {
    //Create a socket:
    // the socket function return an int which is the id(fd = file descriptor) of the socket.
    // AF_INET - use ipv4 (1.2.3.4).
    // SOCK_STREAM - TCP
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "Error creating socket" << endl;
        return -1;
    }

    //Connect to the server:
    // define all the details:
    // what is the port of the server?
    // what is the ip of the server? etc ...
    //sockaddr_in is a struct where we can put the details.
    sockaddr_in addr;
    // family = ip type.
    addr.sin_family = AF_INET;
    // set the ip address of the server:
    // todo - we will use 127.0.0.1 ip for now, in order to allow a connection from this computer using 2 terminals
    // todo - we will need to change it to the user input ip.
    // we will cast the ip address to inet_addr.
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // cast the port to htons.
    // todo - we will need to change it to the user input port.
    addr.sin_port = htons(1025);

    // explicit casting from sockaddr_in to sockaddr*
    //        int x = 5;
    //        float y = (float)x;
    //        float* z = (float*)&x;
    //
    //        sockaddr_in addr;
    //        sockaddr y = (sockaddr)addr;
    if (connect(sock,(sockaddr*)&addr, sizeof (addr)) < 0 ){
        cout << "Error connecting to the server" << endl;
        return -1;
    }

    //todo - this is just for now, in order to check the connection.
    // need to change to the input: vector,metric and k.
    string message = "2 3 4 5 MAN 3";
    // c_str() convert the string from string to const char*, as needed in send function.
    if (send(sock,message.c_str(), message.size(), 0) < 0){
        cout << "Error sending message " << endl;
        return -1;
    }

    //Read the Response from the server:
    // create a buffer (char array in size of 1024 bits) to save the server response.
    char buffer[1024];
    // we will use the recv function:
    // the data we received will save in the buffer, and the return value is the size.
    int bytesReceived = recv(sock,buffer,1024,0);
    if ( bytesReceived < 0) {
        cout << "Error receiving message from server " << endl;
        return -1;
    }

    // Print the response from the server:
    cout << "Response from server: " << string(buffer,bytesReceived) << endl;

    //Close the socket:
    close(sock);

    return 0;
}

// todo list: 1. user input. 2. input validation. 3. send the user input instead of message. 4. infinite loop