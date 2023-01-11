
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
    commonData->isFileUploaded = true;
    dio->write("Please upload your local test CSV file.\n");
    this->fileUpload("test_dataset.csv");
    commonData->test_path = "test_dataset.csv";
    return;
    // save here the test dataset with the test KNN field in the Common struct?
}


AlgSettings::AlgSettings(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"algorithm settings"){}
void AlgSettings::execute(){
    string newParams;
    bool flag = true;
    while(flag){
        dio->write("The current KNN parameters are: K = ");
        dio->write(to_string(commonData->k));
        dio->write(", distance mertic = ");
        dio->write(commonData->metric);
        dio->write("\n");
        newParams = dio->read();

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
            try {
                int nk = stoi(token);
                commonData->k = nk;
            } catch (exception &e) {
                dio->write("invalid value for k\n");
            }
            getline(ss, token, ' ');
            if (token == "AUC" || token == "MIN" || token == "MAN" || token == "CAN" || token == "CHB") {
                string nmetric = token;
                commonData->metric = nmetric;
            } else {
                dio->write("invalid value for metric\n");
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

    //THIS IS EXAMPLE OF WHAT YOU SHOULD DO - RUN YOUR KNN USING THE COMMONDATA.
    commonData->knn = new KNNClassifier(commonData->k, commonData->train_path, commonData->metric);
    //loop that reads off the file and writes the prediction.
    
    // create fstream - file stream with using the path from the function argument.
    // using this file stream we can open the dataset.
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
        // line = "5.3,1,3,6,iris-v"
        // find all the words in the line and store them.
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
        int size = content.size();
        vector<vector<string>> xTest;
        vector<string> yTest;
        int sampleSize = content[0].size();
        for (int i = 0; i < size ; i++) {
            vector<double> sample;
            vector<string> samplest;
            string pred;
            for (int j = 0; j < sampleSize; ++j) {
                string valst = content[i][j];
                double val = stod(content[i][j]);
                sample.push_back(val);
                samplest.push_back(valst);
            }
        xTest.push_back(samplest);
        pred = commonData->knn->predict(sample);
        yTest.push_back(pred);
        }
        pair<vector<vector<string>>,vector<string>> result(xTest, yTest);
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
    int size = commonData->predictions.first.size(); // = knn.size() ... get the CSV size that you loaded.
    for(int i = 0; i < size; i++){
        vector<string> xTest = commonData->predictions.first[i];
        string label = commonData->predictions.second[i];
        int sampleSize = xTest.size();
        stringstream ss;
        for(int j = 0; j < sampleSize; j++) {
            ss << xTest[j];
            if (j < sampleSize - 1) {
                ss << ' ';
            }
        }
        //print the file as they asked in the ass.
        ss << '\t';
        ss << label;
        ss << '\n';
        this->dio->write(ss.str());
    }
    this->dio->write("Done.\n");
    return;
}


// todo - need to write command for option 5..


Exit::Exit(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"exit"){}
void Exit::execute(){
    delete commonData;
    return;
}

