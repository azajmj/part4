#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <unistd.h>


using namespace std;

int main(int argc, char *argv[]) {
    try {
            // Check that there are exactly 2 arguments
            if (argc != 3) {
                throw invalid_argument("Expected 2 arguments: <dataset_file> <server_port>");
            }
            // Check that the first argument is a string
            string ip = (argv[1]);
            // Check that the second argument is an integer
            int port = atoi(argv[2]);
            if (port == 0) {
                throw invalid_argument("Expected an integer value for <server_port>");
            }

        //Create a socket:
        // the socket function return an int which is the id(fd = file descriptor) of the socket.
        // AF_INET - means use ipv4 (1.2.3.4).
        // SOCK_STREAM - means TCP

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
        // we will cast the ip address to inet_addr.
        addr.sin_addr.s_addr = inet_addr(argv[1]);
        // cast the port to htons.
        addr.sin_port = htons(port);

        // explicit casting from sockaddr_in to sockaddr*
        // like saying:  int x = 5;
        //              float y = (float)x;
        //              float* z = (float*)&x;
        // here its equivalent:
        //              sockaddr_in addr;
        //              sockaddr y = (sockaddr)addr;
        if (connect(sock, (sockaddr *) &addr, sizeof(addr)) < 0) {
            cout << "Error connecting to the server" << endl;
            return -1;
        }


        while (1) {
            vector<double> numbers;
            string dist;
            int k;
            //cout << "Enter your data" << endl;
            string input;
            getline(cin, input);
            // "input looks something like =  "1 2 3 4 AUC 1"
            istringstream stream_str(input); //keeps string in input stringstream
            if (input == "-1") {
                close(sock);
                return 0;
            }
            int flag = 0;
            while (stream_str.good()) {
                string token;
                getline(stream_str, token, ' ');
                double val = 0;
                try
                    //trying to convert vector to double
                {
                    val = stod(token.c_str());
                    numbers.push_back(val);
                }
                catch (exception &e) {
                    //making sure token is one of the distances
                    if (token == "AUC" || token == "MIN" || token == "MAN" || token == "CAN" || token == "CHB") {
                        dist = token;
                    } else {
                        cout << "invalid input" << endl;
                        flag = 1;
                        break;
                    }

                    // after we took the dist, the stream string contain only the k value.
                    // so now we can just take the last value and save him in k variable.
                    getline(stream_str, token, ' ');
                    try {
                        k = stoi(token);
                        break;
                    }
                    catch (exception &e) {
                        cout << "invalid input" << endl;
                        flag = 1;
                        break;
                    }
                }
            }

            //checking vector input
            if (numbers.size() == 0) {
                cout << "invalid input" << endl;
                continue;
            }
            if (flag == 1) {
                continue; //invalid input, new iteration
            }
            // we want to do Serialization to our input in order to send it to the server:
            // we will use string stream:
            // 4 1 2 3 4 AUC 2
            stringstream data;
            data << numbers.size() << " ";
            for (int i = 0; i < numbers.size(); i++) {
                data << numbers[i] << " ";
            }
            data << dist << " ";
            data << k;
            string message = data.str();

            // c_str() convert the string from string to const char*, as needed in send function.
            if (send(sock, message.c_str(), message.size(), 0) < 0) {
                cout << "Error sending message " << endl;
                return -1;
            }
            //Read the Response from the server:
            // create a buffer (char array in size of 1024 bits) to save the server response.
            char buffer[1024];
            // we will use the recv function:
            // the data we received will save in the buffer, and the return value is the size.
            int bytesReceived = recv(sock, buffer, 1024, 0);
            if (bytesReceived < 0) {
                cout << "Error receiving message from server " << endl;
                return -1;
            }
            //cout << "Response from server: ";
            int index = 0;
            while(buffer[index] != '\0')
            {
                cout<< buffer[index];
                index++;
            }
        }
    }
    catch (const exception &e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
