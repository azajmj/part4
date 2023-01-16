//
// Created by Lenovo on 10/01/2023.
//
#pragma once
#include<iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include "knn.h"
#include "defaultIO.h"


using namespace std;

// will hold shared data to be reached by all commands
struct CommonData {
    bool isFileUploaded;
    bool isFileClassified;
    int k;
    string train_path;
    string test_path;
    string metric;
    KNNClassifier* knn;
    pair<vector<string>, vector<string>> predictions;
    //init struct vals
    CommonData(): isFileUploaded(false),isFileClassified(false),k(5),metric("AUC"),knn(),predictions(){}
};

// parent class
class Command{
protected:
    DefaultIO* dio;
    CommonData* commonData;
public:
    string description;
    Command(DefaultIO* dio, CommonData* commonData, string description):dio(dio),commonData(commonData) ,description(description){}
    virtual void execute()=0;
    virtual ~Command(){}
    // upload file function to be inherited
    virtual void fileUpload(string name){
        try {
            ofstream file(name);
            string line;
            string content;
            content = this->dio->read();
            istringstream ss(content);
            do {
                getline(ss, line, '\n');
                if(line != "done$"){
                    file << line << endl;
                } 
            } while (line != "done$");
            this->dio->write("Upload complete.\n");
            return;
        } catch (...) {
            cout << "error uploading file in server\n";
            this->dio->write("invalid input");
        }

        // this function is the one written by Karnih:
        
        // try{
        //     ofstream file(name);
        //     string line;
        //     do
        //     {
        //         line = this->dio->read();
        //         cout << "recieved! " << line;
        //         // assume the file end with "done", change this according to your files..
        //         if(line != "done$"){
        //             file << line << endl;
        //         }
        //     } while (line != "done$");
            
        //     this->dio->write("Upload complete.\n");
        //     return;
        //     // // close the file
        //     // if(file.is_open())
        //     //     file.close();
        // } catch (...) {
        //     cout << "errror\n";
        //     this->dio->write("invalid input");
        // }
    }
};


class UploadCsv:public Command{
public:
    UploadCsv(DefaultIO* dio, CommonData* commonData);
    virtual void execute();
};

class AlgSettings:public Command{
public:
    AlgSettings(DefaultIO* dio, CommonData* commonData);
    virtual void execute();
};

class Classify:public Command{
public:
    Classify(DefaultIO* dio, CommonData* commonData);
    virtual void execute();
};

class Results:public Command{
public:
    Results(DefaultIO* dio, CommonData* commonData);

    virtual void execute();
};

class loadResults:public Command{
public:
    loadResults(DefaultIO* dio, CommonData* commonData);

    virtual void execute();
};

class Exit:public Command{
public:
    Exit(DefaultIO* dio, CommonData* commonData);
    virtual void execute();
};



