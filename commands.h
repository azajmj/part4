//
// Created by Lenovo on 10/01/2023.
//

#ifndef COMMAND_PATTERN_COMMANDS_H
#define COMMAND_PATTERN_COMMANDS_H

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>


using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual ~DefaultIO() {}

    virtual void readAndFile(string name) {};
};

class StandardIO: public DefaultIO{
    virtual string read() {
        string s;
        std::cin >> s;
        return s;
    }
    virtual void write(string text){
        std::cout << text << endl;
    }
};

class socketIO : public DefaultIO{
    int clientID;
public:
    socketIO(int clientid){
        this->clientID = clientid;
    }

    virtual string read(){
        string str = "";
        char ch = 0;
        ::read(clientID, &ch, sizeof(char));
        while (ch != '\n') {
            str += ch;
            ::read(clientID, &ch, sizeof(char));
        }
        return str;

    }

    virtual void write(string text){
        ::write(clientID, text.c_str(), text.length());
    }
};


struct CommonData {
    bool isFileUploaded;
    bool isFileClassified;
    int k;
    string metric;
    //todo - probably need to have more fields, like KNN object for the train and KNN object for test.
    CommonData(): isFileUploaded(false),isFileClassified(false),k(5),metric("EUC"){}
};


class Command{
protected:
    DefaultIO* dio;
    CommonData* commonData;
public:
    string description;
    Command(DefaultIO* dio, CommonData* commonData, string description):dio(dio),commonData(commonData) ,description(description){}
    virtual void execute()=0;
    virtual ~Command(){}
    virtual void fileUpload(string name){
        // this function will load the classifier CSV, you can change it according to what you did in your
        // previous ass.. this is just in general way..

        ofstream file(name);
        string line;

        do
        {
            line = this->dio->read();
            // assume the file end with "done", change this according to your files..
            if(line != "done"){
                file << line << endl;
            }
        } while (line != "done");

        this->dio->write("Upload complete.\n");
        // close the file
        if(file.is_open())
            file.close();
    }
};

class UploadCsv:public Command{
public:
    UploadCsv(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"upload an unclassified csv data file"){}
    virtual void execute(){
        dio->write("Please upload your local train CSV file.\n");
        this->fileUpload("your_dataset.csv");
        // save here the train dataset with the train KNN field in the Common struct?
        dio->write("Please upload your local test CSV file.\n");
        this->fileUpload("anomalyTest.csv");
        // save here the test dataset with the test KNN field in the Common struct?
    }
};

class AlgSettings:public Command{
public:
    AlgSettings(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"algorithm settings"){}
    virtual void execute(){
        string newParams;
        bool  flag = true;
        while(flag){
            dio->write("The current KNN parameters are: K = ");
            dio->write(commonData->k);
            dio->write(", distance mertic = ");
            dio->write(commonData->metric);
            dio->write("\n");
            dio->read(&newParams);

            //check if the newParams is empty, then return.
            if (newParams.empty()){
                return;
            }
            // else - check validation for both and then update them..
            else
            {
                istringstream ss(newParams);
                string token;
                getline(ss, token, ' ');

                //check validition for K..
                getline(ss, token, ' ');
                //check validition for metric..

            }
        }
    }
};

class Classify:public Command{
public:
    Classify(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"classify data"){}
    virtual void execute(){
        //in case we didnt uploaded a file yet:
        if (!commonData->isFileUploaded)
        {
            dio->write("please upload data\n");
            return;
        }

        //THIS IS EXAMPLE OF WHAT YOU SHOULD DO - RUN YOUR KNN USING THE COMMONDATA.
        //commonData->KNN-> RUN
        dio->write("classifying data complete\n");
    }
};

class Results:public Command{
public:
    Results(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"display results"){}

    virtual void execute(){
        if (!commonData->isFileUploaded)
        {
            dio->write("please upload data\n");
            return;
        }
        if (!commonData->isFileClassified)
        {
            dio->write("please classify the data\n");
            return;
        }
        auto size; // = knn.size() ... get the CSV size that you loaded.
        for(int i = 0; i < size; i++){

            //print the file as they asked in the ass.
            this->dio->write();
            this->dio->write("\t");
            this->dio->write(" ");
            this->dio->write();

            this->dio->write("\n");
        }
        this->dio->write("Done.\n");
    }
};

// todo - need to write command for option 5..

class Exit:public Command{
public:
    Exit(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"exit"){}
    virtual void execute(){
    }
};


#endif //COMMAND_PATTERN_COMMANDS_H
