//
// Created by Lenovo on 10/01/2023.
//

#include "CLI.h"
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    CommonData* commonData = new CommonData();
    // probably need to do commonData->KNN = NEW KNN...
    this->commands.push_back(new UploadCsv(dio,commonData));
    this->commands.push_back(new AlgSettings(dio,commonData));
    this->commands.push_back(new Classify(dio,commonData));
    this->commands.push_back(new Results(dio,commonData));
    // NEED TO ADD THE COMMAND OF OPTION NUMBER 5 THAT I DIDNT DEFINED..
    this->commands.push_back(new Exit(dio,commonData));
}

void CLI::start(){
    size_t size = commands.size();

    float option = -1;
    string commandStr;
    while(option != 6){
        this->dio->write("Welcome to the KNN Classifier Server. ");
        this->dio->write("Please choose an option:\n");
        for (size_t i = 0; i < size; i++){
            int index = i+1;
            this->dio->write(index);
            this->dio->write(commands[i]->description+"\n");
        }
        commandStr = this->dio->read();
        try {
            option = stoi(commandStr);
            if ((option >= 1 && option < 6) || option == 8 ){
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