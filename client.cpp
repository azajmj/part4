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
#include <pthread.h>
#include <functional>

using namespace std;
// client as a class - recieves ip and port at init
Client::Client(string ip, int port) {
    this->ip = ip;
    this->port = port;
}
Client::~Client() {
    delete dio;
}

string Client::uploadfile(DefaultIO* dio, string file_path) {
    fstream file(file_path, ios::in);
    // string response;
    stringstream ss;
    //open the file and check if opening the file worked.
    if (file.is_open())
    {
        string line;
        // start read the file content.
        // as long as we have lines to read OR to 120 lines of content- to fit the sockets buffer.
        try {
            int flag = 0;
            while(getline(file, line)) {
                ss << line << endl;;
                if (flag == 120) {
                    break;
                }
                flag++;
            }
            // add done for conventions at server
            ss << "done$" << endl;
        } catch (...) {
        // print an error.
        cout << "invalid file path\n";
        }
    }
    if(file.is_open()){
    file.close();
    }
    return ss.str();
}

void Client::downloadfile(DefaultIO* dio, string file_path, string content) {
    //split to thread here?
    try{
        // users file name
        ofstream file(file_path);
        // string content is the servers predictions.
        if (content == "please upload data\n" || content == "please classify the data\n") {
            cout << content << endl;
            return;
        }
        istringstream ss(content);
        string line;
        // copy content line by line to the file until Done.
        do {
            getline(ss, line, '\n');
            if(line != "Done."){
                file << line << endl;
            } else {
                break;
            }
        } while(line != "Done.");
    
        if(file.is_open()){
            file.close();
        }
    } catch (...) {
        cout <<"invalid file path\n";
    }
    return;
}
// check if menu option valid
bool Client::validChoice(string m) {
    try {
        int n = stoi(m);
        if ((n >= 1 && n <=5) || n == 8) {
            return true;
        } else {
            return false;
        }
    } catch (...) {
        return false;
    }
    
}

// void* Client::downloadWrapper(void* args) {
//     auto& client = *static_cast<Client*>(args);
//     client.downloadfile(dio, file_path, content);
//     return NULL;
// }

// start runs the client side
void Client::start() {
    try {
        // connecting to server
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
        //creating an io object with the client sock
        this->dio = new SocketIO(sock);

        //run loop
        while(true) {
            string server_message = "";
            string user_input = "";
            thread t;
            // get menu
            server_message = dio->read();
            cout << server_message << endl;
            //get user choice
            while (true) {
                getline(cin, user_input);
                if (this->validChoice(user_input)){
                    break;
                }
                cout << "invalid option\n";
            }
            // act acording to option by if statments:
            // upload files to server
            if (user_input == "1") {
                //execute
                dio->write(user_input);
                int flag = 0;
                while(flag != 2) {
                    string content;
                    server_message = dio->read();
                    cout << server_message << endl;
                    //get usres file path
                    getline(cin, user_input);
                    content = this->uploadfile(dio, user_input);
                    // send file content (first 120 lines) to server
                    dio->write(content);
                    // servers response to upload
                    server_message = dio->read();
                    cout << server_message << endl;;
                    // need to do twice for test and train
                    flag += 1;
                    server_message = "";
                    user_input = "";
                }
                server_message = "";
                user_input = "";
            }
            // settings
            else if (user_input == "2") {
                //execute
                dio->write(user_input);
                server_message = dio->read();
                cout << server_message << endl;
                // get users params
                getline(cin, user_input);
                if(user_input.empty()) {
                    dio->write(" ");
                    // settigs stay the same
                }
                else {
                    // if params both valid, server send changed params, else en error. to keep simetry
                    dio->write(user_input);
                    server_message = dio->read();
                    cout << server_message << endl;              
                }
                server_message = "";
                user_input = "";
                
            }
            // classify 
            else if (user_input == "3") {
                //execute
                dio->write(user_input);
                // get response
                server_message = dio->read();
                cout << server_message << endl;;
                server_message = "";
                user_input = "";
            }
            // results (same as calssify can be merged)
            else if (user_input == "4") {
                //execute
                dio->write(user_input);
                // get response
                server_message = dio->read();
                cout << server_message << endl;
                server_message = "";
                user_input = "";
            }
            // download results - in a seperate thread. right now terminates client when finishing
            else if (user_input == "5") {
                // execute
                dio->write(user_input);
                // get servers predictions
                server_message = dio->read();
                if (server_message != "please upload data\n" && server_message != "please classify the data\n") {
                    cout << "Enter file path:\n";
                    getline(cin, user_input);
                    // start a thread to do downloadfile with path(userinput) and content(servermessage) as arguments
                    // two ways not sure which is better
                    // std::thread t(std::bind(&Client::downloadfile, this, dio, user_input, server_message));
                    thread t(&Client::downloadfile, this, dio, user_input, server_message);
                    cout << "Download complete." << endl;
                } else {
                    // not uploaded or calssified error
                    cout << server_message << endl;
                }
            }
            // exit
            else if (user_input == "8") {
                //execute
                dio->write(user_input);
                // join on download thread
                try {
                    t.join();
                } catch (const std::exception& e) {
                    cout << "problem in download\n";
                }
                // right now theres a segmentation fault when exiting
                close(sock);
                return;
            }
            // reads not in sync without this cout for some reason
            cout<< "\n";
        }
    }
    catch (const exception &e) {
        cerr << e.what() << endl;
        return;
    }
    return;
}
// main for client recieves 2 args at initation
int main(int argsc, char *argv[]) {
    string ip = (argv[1]);
    int port = atoi(argv[2]);
    // add check arguments
    Client client(ip, port);
    try{
        client.start();
    } catch (...) {
        cout << "ERROR in client\n";
    }
    return 0;
}

// old servers code still not delted just in case..

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
