#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <unistd.h>
#include "defaultIO.h"
#include "Client.h"
#include <thread>

using namespace std;

Client::Client(string ip, int port) {
    this->ip = ip;
    this->port = port;
}
Client::~Client() {
    delete dio;
}

void Client::uploadfile(DefaultIO* dio, string file_path) {
    fstream file(file_path, ios::in);
    string response;
    //open the file and check if opening the file worked.
    if (file.is_open())
    {
    string line;
    // start read the file content.
    // as long as we have lines to read - read.
    try {
    while(getline(file,line)) {
        this->dio->write(line);
    }
    this->dio->write("done$");
    } catch (...) {
    // print an error.
    cout << "invalid file path\n";
    }
    }
    if(file.is_open()){
    file.close();
    }
    response = this->dio->read();
    cout << response;
    return;
}

void Client::downloadfile(DefaultIO* dio, string file_path) {

    //split to thread here?
    try{
        ofstream file(file_path);
        string line;

        do
        {
            line = dio->read();
            // assume the file end with "done", change this according to your files..
            if(line != "done.\n"){
                file << line << endl;
            }
        } while (line != "done.\n");
        if(file.is_open()){
            file.close();
        }
    } catch (...) {
        cout <<"invalid file path\n";
    }
    // close the file
    //end thread?
    return;
}

void Client::start() {
    try {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            cout << "Error creating socket" << endl;
            return;
        }
        sockaddr_in addr;
       
        addr.sin_family = AF_INET;
   
        addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
        
        addr.sin_port = htons(this->port);

        if (connect(sock, (sockaddr *) &addr, sizeof(addr)) < 0) {
            cout << "Error connecting to the server" << endl;
            return;
        }
        DefaultIO* dio = new SocketIO(sock);
        while(true) {
            string server_message;
            thread t;
            int choice;
            string user_input;
            server_message = dio->read();
            cout << server_message << endl;
            getline(cin, user_input);
            choice = stoi(user_input);
            if (choice == 1) {
                dio->write(user_input);
                int flag = 0;
                while(flag != 2) {
                    server_message = dio->read();
                    cout << server_message;
                    getline(cin, user_input);
                    this->uploadfile(dio, user_input);
                    flag += 1;
                }
            }
            if (choice == 2 || choice == 3 ){
                dio->write(user_input);
            }
            if (choice == 4) {
                dio->write(user_input);
                while (server_message != "Done.") {
                    server_message = dio->read();
                    cout << server_message<< endl;
                }
            }
            if (choice == 5) {
                dio->write(user_input);
                //lock thread?
                thread t(&Client::downloadfile, this, dio, user_input);
                //unlock thread?
            }
            if (choice >= 6) {
                this->dio->write(user_input);
                t.join();
                close(sock);
                return;
            }
        }
    }
    catch (const exception &e) {
        cerr << e.what() << endl;
        return;
    }
    return;
}
int main(int argsc, char *argv[]) {
    string ip = (argv[1]);
    int port = atoi(argv[2]);
    // check arguments
    Client client(ip, port);
    try{
        client.start();
    } catch (...) {
        cout << "ERROR in client\n";
    }
    return 0;
}

// int main(int argc, char *argv[]) {
//     try {
//             // Check that there are exactly 2 arguments
//             if (argc != 3) {
//                 throw invalid_argument("Expected 2 arguments: <dataset_file> <server_port>");
//             }
//             // Check that the first argument is a string
//             string ip = (argv[1]);
//             // Check that the second argument is an integer
//             int port = atoi(argv[2]);
//             if (port == 0) {
//                 throw invalid_argument("Expected an integer value for <server_port>");
//             }

//         //Create a socket:
//         // the socket function return an int which is the id(fd = file descriptor) of the socket.
//         // AF_INET - means use ipv4 (1.2.3.4).
//         // SOCK_STREAM - means TCP

//         int sock = socket(AF_INET, SOCK_STREAM, 0);
//         if (sock < 0) {
//             //cout << "Error creating socket" << endl;
//             return -1;
//         }
//         //Connect to the server:
//         // define all the details:
//         // what is the port of the server?
//         // what is the ip of the server? etc ...
//         //sockaddr_in is a struct where we can put the details.
//         sockaddr_in addr;
//         // family = ip type.
//         addr.sin_family = AF_INET;
//         // set the ip address of the server:
//         // we will cast the ip address to inet_addr.
//         addr.sin_addr.s_addr = inet_addr(argv[1]);
//         // cast the port to htons.
//         addr.sin_port = htons(port);

//         // explicit casting from sockaddr_in to sockaddr*
//         // like saying:  int x = 5;
//         //              float y = (float)x;
//         //              float* z = (float*)&x;
//         // here its equivalent:
//         //              sockaddr_in addr;
//         //              sockaddr y = (sockaddr)addr;
//         if (connect(sock, (sockaddr *) &addr, sizeof(addr)) < 0) {
//             //cout << "Error connecting to the server" << endl;
//             return -1;
//         }


//         while (1) {
//             vector<double> numbers;
//             string dist;
//             int k;
//             //cout << "Enter your data" << endl;
//             string input;
//             int validationCounter = 0;
//             getline(cin, input);
//             // "input looks something like =  "1 2 3 4 AUC 1"
//             istringstream stream_str(input); //keeps string in input stringstream
//             if (input == "-1") {
//                 string  message = "-1";
//                 if (send(sock, message.c_str(), message.size(), 0) < 0) {
//                     //cout << "Error sending message " << endl;
//                     return -1;
//                 }
//                 close(sock);
//                 return 0;
//             }
//             int flag = 0;
//             while (stream_str.good()) {
//                 string token;
//                 getline(stream_str, token, ' ');
//                 double val = 0;
//                 try
//                     //trying to convert vector to double
//                 {
//                     val = stod(token.c_str());
//                     numbers.push_back(val);
//                 }
//                 catch (exception &e) {
//                     //making sure token is one of the distances
//                     if (token == "AUC" || token == "MIN" || token == "MAN" || token == "CAN" || token == "CHB") {
//                         dist = token;
//                         validationCounter++;
//                     } else {
//                         flag = 1;
//                         break;
//                     }

//                     // after we took the dist, the stream string contain only the k value.
//                     // so now we can just take the last value and save him in k variable.
//                     getline(stream_str, token, ' ');
//                     try {
//                         k = stoi(token);
//                         validationCounter++;
//                         break;
//                     }
//                     catch (exception &e) {
//                         flag = 1;
//                         break;
//                     }
//                 }
//             }

//             if (validationCounter != 2){
//                 cout <<"invalid input" << endl;
//                 continue;
//             }

//             //checking vector input
//             if (numbers.size() == 0) {
//                 cout << "invalid input" << endl;
//                 continue;
//             }
//             if (flag == 1) {
//                 cout << "invalid input" << endl;
//                 continue; //invalid input, new iteration
//             }
//             // we want to do Serialization to our input in order to send it to the server:
//             // we will use string stream:
//             // 4 1 2 3 4 AUC 2
//             stringstream data;
//             data << numbers.size() << " ";
//             for (int i = 0; i < numbers.size(); i++) {
//                 data << numbers[i] << " ";
//             }
//             data << dist << " ";
//             data << k;
//             string message = data.str();

//             // c_str() convert the string from string to const char*, as needed in send function.
//             if (send(sock, message.c_str(), message.size(), 0) < 0) {
//                 //cout << "Error sending message " << endl;
//                 return -1;
//             }
//             //Read the Response from the server:
//             // create a buffer (char array in size of 1024 bits) to save the server response.
//             char buffer[4096];
//             // we will use the recv function:
//             // the data we received will save in the buffer, and the return value is the size.
//             int bytesReceived = recv(sock, buffer, 4096, 0);
//             if (bytesReceived < 0) {
//                 //cout << "Error receiving message from server " << endl;
//                 return -1;
//             }
//             //cout << "Response from server: ";
//             int index = 0;
//             while(buffer[index] != '\0')
//             {
//                 cout<< buffer[index];
//                 index++;
//             }
//             cout << endl;
//             memset(buffer, 0, sizeof(buffer));
//         }
//     }
//     catch (const exception &e) {
//         cerr << e.what() << endl;

//         return 1;
//     }

//     return 0;
// }
