//
// Created by Lenovo on 10/01/2023.
//

#include "CLI.h"
#include <sstream>
using namespace std;

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    // auto commonData = make_shared<CommonData>();
    this->commonData = new CommonData();
    // probably need to do commonData->KNN = NEW KNN...
    this->commands.push_back(new UploadCsv(dio,commonData));
    this->commands.push_back(new AlgSettings(dio,commonData));
    this->commands.push_back(new Classify(dio,commonData));
    this->commands.push_back(new Results(dio,commonData));
    this->commands.push_back(new loadResults(dio, commonData));
    this->commands.push_back(new Exit(dio,commonData));
}

CLI::~CLI() {
    delete commonData;
    for (Command* command : commands) {
        delete command;
    }
    // size_t size = commands.size();
    // for (size_t i = 0; i < size; i++){
    //     delete commands[i];
    // }
}

//after adding 5th command change every 5 to 6
void CLI::start(){
    size_t size = commands.size();

    float option = -1;
    string commandStr;
    bool isRunning = true;
    cout <<"Server Running...\n";
    while(isRunning){
        // cout <<"printing menue\n";
        stringstream ss;
        ss << "Welcome to the KNN Classifier Server. Please choose an option:\n";
        // this->dio->write("Welcome to the KNN Classifier Server. Please choose an option:\n");
        for (size_t i = 0; i < size; i++){
            int index = i+1;
            if (index == 6) {
                index = 8;
            }
            ss << to_string(index) << ". " << commands[i]->description+"\n";
            // this->dio->write(to_string(index));
            // this->dio->write(". ");
            // this->dio->write(commands[i]->description+"\n");
        }
        this->dio->write(ss.str());
        commandStr = this->dio->read();
        try {
            option = stoi(commandStr);
            cout << "got option " << option << endl;
            if ((option >= 1 && option <= 5) || option == 8 ){
                if (option == 8) {
                    cout << "server in 8\n";
                    option = 6;
                    this->commands[option-1]->execute();
                    cout << "terminating client..\n";
                    isRunning = false;
                }
                // cout << "going to exe " << option << endl;
                this->commands[option-1]->execute();
                if (option == 5) {
                    cout << "\n";
                }
                // cout << "returned from command"<< option <<"\n";

            }
            else {
                break;
            }

        }
        catch (const invalid_argument&){

        }
        // cout << "send $$$\n";
        // this->dio->write("$$$");
    }
    cout << "client off...\n";
    return;
}