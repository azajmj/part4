
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
#include "commands.h"


using namespace std;

UploadCsv::UploadCsv(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"upload an unclassified csv data file"){}
void UploadCsv::execute(){
    dio->write("Please upload your local train CSV file.\n");
    this->fileUpload("train_dataset.csv");
    commonData->train_path = "train_dataset.csv";
    dio->write("Please upload your local test CSV file.\n");
    this->fileUpload("test_dataset.csv");
    commonData->test_path = "test_dataset.csv";
    commonData->isFileUploaded = true;
    return;
}

AlgSettings::AlgSettings(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"algorithm settings"){}
void AlgSettings::execute(){
    string newParams;
    bool flag = true;
    while(flag){
        // send current params
        stringstream ss;
        ss << "The current KNN parameters are: K = " << to_string(commonData->k) << ", distance mertic = " << commonData->metric << "\n";
        dio->write(ss.str());
        // get new params
        newParams = dio->read();
        //check if the newParams is empty, then return, (send space from client empty doesnt work.)
        if (newParams.empty() || newParams == " "){
            return;
        }
        // else - check validation for both and then update them, or just the valid one
        else
        {
            istringstream ss(newParams);
            // output message si
            stringstream si;
            string token;
            getline(ss, token, ' ');
            //check k
            try {
                int nk = stoi(token);
                commonData->k = nk;
            } catch (exception &e) {
                si << "invalid value for k\n";
            }
            getline(ss, token, ' ');
            //check metric
            if (token == "AUC" || token == "MIN" || token == "MAN" || token == "CAN" || token == "CHB") {
                string nmetric = token;
                commonData->metric = nmetric;
            } else {
                si << "invalid value for metric\n";
            }
            // return invalid message
            if (!si.str().empty()) {
                dio->write(si.str());
            }
            else {
                // to keep client in sync
                dio->write("changed params");
            }
            return;

        }
    }
}



Classify::Classify(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"classify data"){}
void Classify::execute(){
    //in case we didnt uploaded a file yet:
    if (!commonData->isFileUploaded)
    {
        dio->write("please upload data\n");
        return;
    }
    //create the knn at runtime using the current params
    commonData->knn = new KNNClassifier(commonData->k, commonData->train_path, commonData->metric);
    
    // using this file stream we can open the test dataset.
    fstream file(commonData->test_path, ios::in);

    //open the file and check if opening the file worked.
    if (file.is_open())
    {
    string line, word;
    vector<vector<string>> content;
    vector<string> row;
    // start read the file content.
    // as long as we have lines to read - read.
    try {
    while(getline(file,line)) {
        row.clear();
        // create a string stream out of the line, so we can iterate the line in order to find the words.
        stringstream str(line);
        // define the line to end with ',' instead of '\n' in order to extract the words.
        while(getline(str,word,','))
        {
            row.push_back(word);
        }
        content.push_back(row);
        // now we have vector like this:
        // content = [ (1,2,3,4,iris-v), (5,6,7,8,iris-d) ... ]
        // craeting two outputs one for the knn with doubles and another to be saved in predictions as string
        int size = content.size();
        vector<string> xTest;
        vector<string> yTest;
        int sampleSize = content[0].size();
        for (int i = 0; i < size ; i++) {
            vector<double> sample;
            string samplest;
            string pred;
            for (int j = 0; j < sampleSize; ++j) {
                string valst = content[i][j];
                double val = stod(content[i][j]);
                sample.push_back(val);
                samplest += valst += " ";
            }
        xTest.push_back(samplest);
        // get the prediction
        pred = commonData->knn->predict(sample);
        yTest.push_back(pred);
        }
        // save test results as pairs 
        pair<vector<string>,vector<string>> result(xTest, yTest);
        commonData->predictions = result;
        commonData->isFileClassified = true;
    }
    dio->write("classifying data complete\n");
    } catch (...) {
    // print an error.
    dio->write("invalid test file\n");
    }
    }
    if(file.is_open()){
    file.close();
    }
    return;

}


Results::Results(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"display results"){}
void Results::execute(){
    //check status
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

    int size = commonData->predictions.first.size(); // = knn.size() ... get the test size that was loaded.
    stringstream ss;
    // add each one according to assignment
    for(int i = 0; i < size; i++){
        string label = commonData->predictions.second[i];
        ss << (i+1) << "\t"  << label << "\n";

    }
    // add done.
    ss << "Done.\n";
    this->dio->write(ss.str());
    return;

}
// load is ame exact function jut with a differnet description
loadResults::loadResults(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"download results"){}
void loadResults::execute(){
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

    int size = commonData->predictions.first.size();
    stringstream ss;
    string label;
    for(int i = 0; i < size; i++){
        label = commonData->predictions.second[i];
        ss << (i+1) << "\t"  << label << "\n";
    }
    ss << "Done.\n";
    this->dio->write(ss.str());
    return;
}

// exit just returns
Exit::Exit(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"exit"){}
void Exit::execute(){
    return;
}

