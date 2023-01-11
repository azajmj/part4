//
// Created by Lenovo on 10/01/2023.
//

#include "CLI.h"
using namespace std;

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    // auto commonData = make_shared<CommonData>();
    CommonData* commonData = new CommonData();
    // probably need to do commonData->KNN = NEW KNN...
    this->commands.push_back(new UploadCsv(dio,commonData));
    this->commands.push_back(new AlgSettings(dio,commonData));
    this->commands.push_back(new Classify(dio,commonData));
    this->commands.push_back(new Results(dio,commonData));
    // NEED TO ADD THE COMMAND OF OPTION NUMBER 5 THAT I DIDNT DEFINED..
    this->commands.push_back(new Exit(dio,commonData));
}
CLI::~CLI() {
    delete dio;
    size_t size = commands.size();
    for (size_t i = 0; i < size; i++){
        delete commands[i];
    }
}

//after adding 5th command change every 5 to 6
void CLI::start(){
    size_t size = commands.size();

    float option = -1;
    string commandStr;
    bool isRunning = true;
    while(isRunning){
        this->dio->write("Welcome to the KNN Classifier Server. Please choose an option:\n");
        for (size_t i = 0; i < size; i++){
            int index = i+1;
            if (index == 5) {
                index = 8;
            }
            this->dio->write(to_string(index));
            this->dio->write(". ");
            this->dio->write(commands[i]->description+"\n");
        }
        commandStr = this->dio->read();
        try {
            option = stoi(commandStr);
            if ((option >= 1 && option < 5) || option == 8 ){
                if (option == 8) {
                    option = 5;
                    this->commands[option-1]->execute();
                    isRunning = false;
                }
                this->commands[option-1]->execute();

            }
            else {
                break;
            }

        }
        catch (const invalid_argument&){

        }
    }
}