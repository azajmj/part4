
#include "CLI.h"
#include <sstream>
using namespace std;
// CLI to be used by the server

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
// destructor
CLI::~CLI() {
    delete commonData;
    for (Command* command : commands) {
        delete command;
    }
}

// start method, 
void CLI::start(){
    size_t size = commands.size();

    float option = -1;
    string commandStr;
    bool isRunning = true;
    cout <<"Server Running...\n";
    while(isRunning){
        // printing menu;
        stringstream ss;
        ss << "Welcome to the KNN Classifier Server. Please choose an option:\n";
        for (size_t i = 0; i < size; i++){
            int index = i+1;
            if (index == 6) {
                index = 8;
            }
            //get the descriptions from commands
            ss << to_string(index) << ". " << commands[i]->description+"\n";

        }
        this->dio->write(ss.str());
        commandStr = this->dio->read();
        try {
            // choose option to execute
            option = stoi(commandStr);
            if ((option >= 1 && option <= 5) || option == 8 ){
                if (option == 8) {
                    option = 6;
                    this->commands[option-1]->execute();
                    cout << "terminating client..\n";
                    isRunning = false;
                }
                this->commands[option-1]->execute();
                cout << "\n";
                // dont know why doesnt work without another cout.
                // otherwise client get stuck after some of the commands

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
    return;
}