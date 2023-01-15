
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
    // string content;
    // content = this->dio->read();
    // cout << "in commands::::" << content << endl;
    this->fileUpload("train_dataset.csv");
    commonData->train_path = "train_dataset.csv";
    commonData->isFileUploaded = true;
    // cout << "IM HERE\n";
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
        stringstream ss;
        ss << "The current KNN parameters are: K = " << to_string(commonData->k) << ", distance mertic = " << commonData->metric << "\n";
        dio->write(ss.str());
        // dio->write("The current KNN parameters are: K = ");
        // dio->write(to_string(commonData->k));
        // dio->write(", distance mertic = ");
        // dio->write(commonData->metric);
        // dio->write("\n");
        // cout << "before read in settings";
        newParams = dio->read();
        // cout << "got new params they are " << newParams;

        //check if the newParams is empty, then return.
        if (newParams.empty() || newParams == " "){
            // cout << "returning because empty\n";
            return;
        }
        // else - check validation for both and then update them..
        else
        {
            istringstream ss(newParams);
            stringstream si;
            string token;
            getline(ss, token, ' ');
            try {
                int nk = stoi(token);
                commonData->k = nk;
            } catch (exception &e) {
                si << "invalid value for k\n";
                // dio->write("invalid value for k\n");
            }
            getline(ss, token, ' ');
            if (token == "AUC" || token == "MIN" || token == "MAN" || token == "CAN" || token == "CHB") {
                string nmetric = token;
                commonData->metric = nmetric;
            } else {
                si << "invalid value for metric\n";
                // dio->write("invalid value for metric\n");
            }
            if (!si.str().empty()) {
                dio->write(si.str());
            } else {
                dio->write("$$$");
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
        // cout << "ADDEDDDD: " << samplest << endl;
        xTest.push_back(samplest);
        pred = commonData->knn->predict(sample);
        yTest.push_back(pred);
        }
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
    // cout << "YES OK\n";
    // stringstream ss;
    if (!commonData->isFileUploaded)
    {
        // ss << "please upload data\n";
        dio->write("please upload data\n");
        // dio->write("$$$");
        return;
    }
    if (!commonData->isFileClassified)
    {
        // ss << "please classify the data\n";
        dio->write("please classify the data\n");
        return;
    }
    // if (!ss.empty()) {
    //     io->write(ss.str());
    //     return;
    // }
    
    int size = commonData->predictions.first.size(); // = knn.size() ... get the CSV size that you loaded.
    stringstream ss;
    for(int i = 0; i < size; i++){
        // string x = commonData->predictions.first[i];
        string label = commonData->predictions.second[i];
        // cout << "FFF " << i << "\t" << label << endl;
        ss << (i+1) << "\t"  << label << "\n";
        // int sampleSize = xTest.size();
        // for(int j = 0; j < sampleSize; j++) {
        //     cout << "OMG " << j << endl;
        //     cout << "Adding " << xTest[j] << " for the vector " << i << endl;
        //     ss += xTest[j] + " ";
        //     cout << xTest[j] << endl;
        // }
        // ss.pop_back();
        // ss += "\t" + label + "\n";
        // cout << "RRrRRR" << ss << endl;
    }
    // cout << "SERVER:\n" << ss << endl;
    ss << "Done.\n";
    this->dio->write(ss.str());
    return;
    // stringstream ss;
    
    // for(int i = 0; i < size; i++){
    //     vector<string> xTest = commonData->predictions.first[i];
    //     string label = commonData->predictions.second[i];
    //     int sampleSize = xTest.size();
    //     stringstream ss;
    //     for(int j = 0; j < sampleSize; j++) {
    //         ss << xTest[j];
    //         if (j < sampleSize - 1) {
    //             ss << ' ';
    //         }
    //     }
    //     //print the file as they asked in the ass.
    //     ss << '\t';
    //     ss << label;
    //     ss << endl;
    //     string w;
    //     w = ss.str();
    //     cout << "IN SERVER: " << w << endl;
    //     this->dio->write(w);
    // }
    // this->dio->write("Done.\n");
    // return;
}

loadResults::loadResults(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"download results"){}
void loadResults::execute(){
    // cout << "YES OK\n";
    // stringstream ss;
    if (!commonData->isFileUploaded)
    {
        // ss << "please upload data\n";
        dio->write("please upload data\n");
        // dio->write("$$$");
        return;
    }
    if (!commonData->isFileClassified)
    {
        // ss << "please classify the data\n";
        dio->write("please classify the data\n");
        return;
    }
    // if (!ss.empty()) {
    //     io->write(ss.str());
    //     return;
    // }
    
    int size = commonData->predictions.first.size(); // = knn.size() ... get the CSV size that you loaded.
    stringstream ss;
    string label;
    for(int i = 0; i < size; i++){
        // string x = commonData->predictions.first[i];
        label = commonData->predictions.second[i];
        // cout << "FFF " << i << "\t" << label << endl;
        ss << (i+1) << "\t"  << label << "\n";
        // int sampleSize = xTest.size();
        // for(int j = 0; j < sampleSize; j++) {
        //     cout << "OMG " << j << endl;
        //     cout << "Adding " << xTest[j] << " for the vector " << i << endl;
        //     ss += xTest[j] + " ";
        //     cout << xTest[j] << endl;
        // }
        // ss.pop_back();
        // ss += "\t" + label + "\n";
        // cout << "RRrRRR" << ss << endl;
    }
    // cout << "SERVER:\n" << ss << endl;
    ss << "Done.\n";
    // cout << "SERVER:\n" << ss.str() << endl;
    this->dio->write(ss.str());
    return;
}




Exit::Exit(DefaultIO* dio, CommonData* commonData):Command(dio,commonData,"exit"){}
void Exit::execute(){
    return;
}

