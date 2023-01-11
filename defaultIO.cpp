#include "defaultIO.h"
#include <string>
#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
string StandardIO::read() {
    string s;
    getline(cin, s);
    return s;
}
void StandardIO::write(string text) {
    cout << text;
}

// class socketIO : public DefaultIO{
//     int clientID;
// public:
//     socketIO(int clientid){
//         this->clientID = clientid;
//     }

//     virtual string read(){
//         string str = "";
//         char ch = 0;
//         ::read(clientID, &ch, sizeof(char));
//         while (ch != '\n') {
//             str += ch;
//             ::read(clientID, &ch, sizeof(char));
//         }
//         return str;

//     }

//     virtual void write(string text){
//         ::write(clientID, text.c_str(), text.length());
//     }
// };