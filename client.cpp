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

Client::Client(string ip, int port) {
    this->ip = ip;
    this->port = port;
}
Client::~Client() {
    delete dio;
}

string Client::uploadfile(DefaultIO* dio, string file_path) {
    // cout << "in upload";
    fstream file(file_path, ios::in);
    // string response;
    stringstream ss;
    //open the file and check if opening the file worked.
    if (file.is_open())
    {
        string line;
        // start read the file content.
        // as long as we have lines to read - read.
        try {
            // getline(file, line);
            // this->dio->write(line);
            // string check;
            int flag = 0;
            while(getline(file, line)) {
                // cout << "writing to server lines: " << line << endl;
                ss << line << endl;;
                if (flag == 120) {
                    break;
                    // break;
                }
                // dio->write(line);
                // check = this->dio->read();
                // if (check != "RFNL$"){
                //     break;
                // }
                flag++;
            }
            
            ss << "done$" << endl;
            
            
            // dio->write("done$");
            // cout << "SS IS: " << ss.str();
        } catch (...) {
        // print an error.
        cout << "invalid file path\n";
        }
    }
    if(file.is_open()){
    file.close();
    }
    return ss.str();
    // response = this->dio->read();
    // cout << response;
    // return;
}

void Client::downloadfile(DefaultIO* dio, string file_path, string content) {

    //split to thread here?
    try{
        // cout <<"IN HERE\n";
        ofstream file(file_path);
        // string content;
        // content = dio->read();
        // cout << "got path " << file_path << "got content " << content << endl;
        if (content == "please upload data\n" || content == "please classify the data\n") {
            cout << content << endl;
            return;
        }
        // assume the file end with "done", change this according to your files..
        istringstream ss(content);
        string line;
        do {
            // cout << "in the loooooooooop line " << line << " ";
            getline(ss, line, '\n');
            if(line != "Done."){
                file << line << endl;
            } else {
                // cout << "OMGGG\n";
                break;
            }
        } while(line != "Done.");
    
        if(file.is_open()){
            file.close();
        }
    } catch (...) {
        cout <<"invalid file path\n";
    }
    // close the file
    //end thread?
    // cout << "Download complete\n";
    return;
}
// void Client::getTitle(DefaultIO* io) {
//     while(true) {
//         string server_message;
//         server_message = dio->read();
//         if (server_message == "done$") {
//             return;
//         }
//         cout << server_message << endl;
//     }
// }
// void Client::choiceOne(DefaultIO* io, string user_input) {
//     dio->write(user_input);
//     int flag = 0;
//     string server_message;
//     while(flag != 2) {
//         server_message = dio->read();
//         if(server_message == "$$$") {
//             return;
//         }
//         cout << server_message;
//         getline(cin, user_input);
//         this->uploadfile(dio, user_input);
//         flag += 1;
//     }
//     return;
// }
// void Client::choiceTwo(DefaultIO* io, string user_input) {
//     dio->write(user_input);
//     string server_message;
//     while(true) {
//         server_message = dio->read();
//         if(server_message == "$$$") {
//             return;
//         }
//         cout << server_message;
//         getline(cin, user_input);
//         dio->write(user_input);
//     }
// }

// void Client::choiceFive(DefaultIO* io) {
//     dio->write(user_input);
//     thread t(&Client::downloadfile, this, dio, user_input);
//     while(true) {
//         thread t(&Client::downloadfile, this, dio, user_input);
//         server_message = dio->read();
//         if(server_message == "$$$") {
//             return;
//         }
//         cout << server_message;
//         getline(cin, user_input);
//         this->uploadfile(dio, user_input);
//         flag += 1;
//     }
// }
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
            // cout<< "ready to get menu\n";
            string server_message = "";
            string user_input = "";
            thread t;
            // int choice;
            // this->getTitle(dio);
            server_message = dio->read();
            // if (server_message == "$$$") {
            //     server_message = dio->read();
            // }
            cout << server_message << endl;
            while (true) {
                getline(cin, user_input);
                if (this->validChoice(user_input)){
                    break;
                }
                cout << "invalid option\n";
            }
            // choice = stoi(user_input);
            // dio->write(user_input);
            // cout << "got choice" << choice << endl;
            // cout << "requesting to execute " << user_input << endl;
            if (user_input == "1") {
                dio->write(user_input);
                int flag = 0;
                // string server_message;
                while(flag != 2) {
                    string content;
                    server_message = dio->read();
                    cout << server_message << endl;;
                    getline(cin, user_input);
                    this->uploadfile(dio, user_input);
                    content = this->uploadfile(dio, user_input);

                    dio->write(content);
              
                    // dio->write(content);
                    server_message = dio->read();
                    cout << server_message << endl;;
                    // cout << "got back from uplaod";
                    flag += 1;
                    server_message = "";
                    user_input = "";
                }
                server_message = "";
                user_input = "";
            }
            // || user_input == "3" || user_input == "4"){
            else if (user_input == "2") { 
                dio->write(user_input);
                // while(server_message != "$$$") {
                server_message = dio->read();
                cout << server_message << endl;;
                getline(cin, user_input);
                if(user_input.empty()) {
                    dio->write(" ");
                    // break;
                }
                else {
                    dio->write(user_input);
                    server_message = dio->read();
                    if(server_message != "$$$") {
                        cout << server_message << endl;;
                        
                }
                server_message = "";
                user_input = "";
                }

                // if(server_message == "$$$") {
                //     break;
                // }
                // if(server_message == "invalid value for metric\n" || server_message == "invalid value for k\n" || server_message == "invalid value for k\ninvalid value for metric") {
                //     cout << server_message;
                //     break;
                // }
                // cout << server_message;
                // getline(cin, user_input);
                // if(user_input.empty()) {
                //     dio->write(" ");
                //     break;
                // }
                // dio->write(user_input);
                // // }
            }
            else if (user_input == "3") {
                dio->write(user_input);
                // string server_message;
                server_message = dio->read();
                cout << server_message << endl;;
                server_message = "";
                user_input = "";
            }
            else if (user_input == "4") {
                dio->write(user_input);
                // string server_message;
                server_message = dio->read();
                // cout << server_message;
                // server_message = dio->read();
                cout << server_message << endl;
                // if (server_message != "please upload data\n" && server_message != "please classify the data\n") {
                //     cout << server_message;
                //     // do {
                //     //     // cout <<"IN\n";
                //     //     cout << server_message;
                //     //     server_message = "";
                //     //     server_message = dio->read();
                //     //     // string one;
                //     //     // // getline(cin, one);
                //     //     // // if (one == "8") {
                //     //     // //     break;
                //     //     // // }
                //     // } while (server_message != "Done.\n");
                //     // cout << "OUT\n";
                // }
                //  else {
                //     cout << server_message;
                //     cout << "OMG";
                // }
                server_message = "";
                user_input = "";
            }
            //     dio->write(user_input);
            //     int flag = 0;
            //     while(flag != 2) {
            //         server_message = dio->read();
            //         if(server_message == "$$$") {
            //             break;
            //         }
            //         cout << server_message;
            //         getline(cin, user_input);
            //         this->uploadfile(dio, user_input);
            //         flag += 1;
            //     }
            // }
            // if (choice == 5 ) {
            //     this->choiceTwo(dio);
            // }
            //     dio->write(user_input);
            //     while(true) {
            //         server_message = dio->read();
            //         if(server_message == "$$$") {
            //             break;
            //         }
            //         cout << server_message << endl;
            //         getline(cin, user_input);
            //         if (user_input == "") {
            //             cout <<"OMG"<< endl;
            //             dio->write(user_input);
            //             break;
            //         } else{
            //             dio->write(user_input);
            //         }


            //     }
            // }
            // if (choice == 3 ) {
            //     dio->write(user_input);
            // }
            // if (choice == 4) {
            //     dio->write(user_input);
            //     while (server_message != "Done.") {
            //         server_message = dio->read();
            //         if(server_message == "$$$") {
            //             break;
            //         }
            //         cout << server_message<< endl;
            //     }
            // }
            else if (user_input == "5") {
            //     dio->write(user_input);
            //     //lock thread?
                // cout << "goint to exe " << user_input << endl;
                dio->write(user_input);
                server_message = dio->read();
                if (server_message != "please upload data\n" && server_message != "please classify the data\n") {
                    cout << "Enter file path:\n";
                    getline(cin, user_input);
                    std::thread t(std::bind(&Client::downloadfile, this, dio, user_input, server_message));
                    // std::thread t(std::ref(this->downloadfile()), dio, user_input, server_message);
                    cout << "Download complete." << endl;
                    // cout << "\n";
                } else {
                    cout << server_message << endl;
                }
                // thread t(&Client::downloadfile, this, dio, user_input);
            //     //unlock thread?
            }
            else if (user_input == "8") {
                cout << "client inside 8\n";
                dio->write(user_input);
                try {
                    t.join();
                } catch (const std::exception& e) {
                    cout << "problem in download\n";
                }

                close(sock);
                return;
            }
            // else {
            //     this->choiceTwo(dio, user_input);
            // }
            cout<< "\n";
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
