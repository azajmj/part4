#include "distance.h"
#include "man.h"
#include "euc.h"
#include "cheb.h"
#include "mink.h"
#include "can.h"
#include "knn.h"
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <numeric>
#include <climits>
#include <math.h>

using namespace std;

KNNClassifier::KNNClassifier(int neighbors, string datasetName, string distance){
    k = neighbors;
    metric = initDistance(distance);
    dataTrain = fit(datasetName);

}
//destructor
KNNClassifier::~KNNClassifier() 
{
  delete metric;

}
pair<vector<vector<double>>,vector<string>> KNNClassifier::fit(string datasetName) {
    // init
    string line, word;
    vector<vector<string>> content;
    vector<string> row;
    // create fstream - file stream with using the path from the function argument.
    // using this file stream we can open the dataset.
    fstream file(datasetName, ios::in);

    //open the file and check if opening the file worked.
    if (file.is_open())
    {
       // start read the file content.
       // as long as we have lines to read - read.
       while(getline(file,line))
       {
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
       }
    }
    else
    {
        // print an error.
        cout <<"could not open the file" <<endl;
    }

    // split each row to xTrain and yTrain.
    int size = content.size();

    //init the xTrain & yTrain
    vector<vector<double>> xTrain;
    vector<string> yTrain;

    int sampleSize = content[0].size();
    // the xTrain vectors size
    this->samplesSize = sampleSize - 1;

    for (int i = 0; i < size ; i++)
    {
        vector<double> sample;
        string label;
        //content = [  [1,2,3,4,iris-d] , [] , [], ] therefore we need an inner loop for each sample:
        //create a vector of doubles for the sample in xTrain and then get the label.
        for (int j = 0; j < sampleSize -1; ++j)
        {
            // use stof - "string to double" to convert the value.
            double val = stod(content[i][j]);
            sample.push_back(val);
        }

        // store the sample in xTrain.
        xTrain.push_back(sample);
        label = content[i][sampleSize-1];
        yTrain.push_back(label);
    }
    

    pair<vector<vector<double>>,vector<string>> result(xTrain,yTrain);
    return result;
}



//sorts pairs by first pair value
void KNNClassifier::sortPairs(std::vector<std::pair<double,string>>& pairs) {
        sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
}
//gets the max label from knn
string KNNClassifier::getMaxString(const unordered_map<string, int>& map) {
  // Keep track of the maximum int value and the corresponding string
  int maxValue = 0;
  string maxString;

  // Iterate over the elements in the map
  for (const auto& [key, value] : map) {
    // If the current value is greater than the maximum value, update the maximum value and string
    if (value > maxValue) {
      maxValue = value;
      maxString = key;
    }
  }

  // Return the string with the maximum int value
  return maxString;
}


string KNNClassifier::predict(vector<double>& test) {
    // Calculate distances between input and each vector in data
    int trainSize = dataTrain.first.size();
    vector <pair<double,string>> distanceVec;
    double tempDist; //saves current distance
    for (int i = 0; i < trainSize; i++) {
        vector<double>& temp = dataTrain.first[i];
        tempDist = metric->measure(test, temp);
        // Save distance and corresponding label in distances vector
        distanceVec.push_back({tempDist, dataTrain.second[i]});
    }

    //this returns a sorted vector of pairs, eacvh pair has the distance from input to train vector and its label
    this->sortPairs(distanceVec);
    
    // Count the number of occurrences of each label in the first k distances
    unordered_map <string,int> labelCounts; //saving a map of label and amount of labels of that type (like python dictionary)
    for (int i = 0; i < k; i++) {
        if (labelCounts.count(distanceVec[i].second)) //checking if map has this label, if so add to its count number{
        {
            labelCounts[distanceVec[i].second]++;
        }
        else //this label is now being added because it wasnt there before
        {
            labelCounts[distanceVec[i].second] = 1;
        }
    }
    
    // Return the label with the highest count from map
    string maxLabel = this->getMaxString(labelCounts);

    return maxLabel;
}



Distance *KNNClassifier::initDistance(string distance) {
    if (distance == "AUC") {
      return new Euc();
    }
    if (distance == "MAN") {
       return new Man();
    }
    if (distance == "CHB") {
       return new Cheb();
    }
    if (distance == "CAN") {
      return new Can();
    }
    if (distance == "MIN") {
        Mink *m = new Mink();
        string pn;
        cout << "Enter p Value for Mink' Distance Metric:\n";
        getline(cin, pn);
        m->setP(stoi(pn));
        return m;
    }
    else throw std::invalid_argument( "Invalid Distance Metric" ); 
}

int KNNClassifier::getSampleSize() {
    return this->samplesSize;
}

int KNNClassifier::getTrainSize() {
    int trainSize = dataTrain.first.size();
    return trainSize;
}



    




    
