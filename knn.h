//file of functions 
#pragma once //adds all the imports from here to all the other files as long as I include this file to the others
#include <iostream>
using namespace std;
#include <vector>
#include "distance.h"
#include <unordered_map>


class KNNClassifier
{
    public: 
        KNNClassifier(int neighbors,string datasetName, string distance); //constructor: inits k, **and distance metric*use template?
        ~KNNClassifier(); //destructor
        pair<vector<vector<double>>,vector<string>> fit(string datasetName); // gets a pointer to a files path - reads line by line and initialize it to list of vector vectors(4d) xTrain, and vectors of string lables
        string predict(vector<double>& test); //calls getNeighbors and getLabel, returns prediction
        int getSampleSize();

    private: 
        vector< pair <double, string> > getNeighbors(vector<double>& test); //creates vector of neighbors by pair - dist fro meach xTrain to test and the corespanding label (by index)
                                                                            // might use get distances vector memeber function
                                                                            // sorts by dist before return to predict  
        string getMaxString(const unordered_map<string, int>& map);
        void sortPairs(std::vector<std::pair<double,string>>& pairs);
        Distance *initDistance(string distance); // inits the distance metric typr by string identifier** 
    
        int k; // k neihbors
        pair<vector<vector<double>>,vector<string>> dataTrain; //models data from fit
        Distance *metric; //distnace class object -- to be used to init a distance metric**
        int samplesSize; //for input check 

};
