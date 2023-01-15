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

struct CommonData {
    bool isFileUploaded;
    bool isFileClassified;
    int k;
    string train_path;
    string test_path;
    string metric;
    KNNClassifier* knn;
    pair<vector<string>, vector<string>> predictions;
    //todo - probably need to have more fields, like KNN object for the train and KNN object for test.
    CommonData(): isFileUploaded(false),isFileClassified(false),k(5),metric("AUC"),knn(),predictions(){}
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
        try {
            ofstream file(name);
            string line;
            string content;
            content = this->dio->read();
            // cout << "iserver got:::::" << content << endl;
            istringstream ss(content);
            // ss.str(ss.str() + "\n" + "done$\n");
            // ss << "done$" << endl;
            do {
                getline(ss, line, '\n');
                if(line != "done$"){
                    // cout << "inserver lineis77 " << line << endl;
                    file << line << endl;
                } 
            } while (line != "done$");
            this->dio->write("Upload complete.\n");
            return;
        } catch (...) {
            cout << "error uploading file in server\n";
            this->dio->write("invalid input");
        }

        // this function will load the classifier CSV, you can change it according to what you did in your
        // previous ass.. this is just in general way..

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



