#include "distance.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>
#include <math.h>
#include "knn.h"
using namespace std;

vector<double> getVector(string stringVector) {
    vector<double> v;
    string buff;

    for(int i = 0; i <= stringVector.length(); i++)
    {
        if(stringVector[i] == ' ' || stringVector[i] == '\0') 
        {
            try {
                v.push_back(stod(buff));
                buff = "";
                } catch (...) { cout << "Invalid Input Vector"; exit(0); }
        }
        else { buff += stringVector[i]; }

    }
    return v;
}

int main(int argc, char *argv[]) {
    cout << "Hello\n";
    int k;
    k = stoi( argv[1] );
    string file = argv[2];
    string distance = argv[3];
    KNNClassifier model(k, file, distance);
    while(true) {
        string vec, pred;
        vector<double> test;
        getline(cin, vec);
        if (vec == "finish") {
            break;
        }
        test = getVector(vec);
        if (model.getSampleSize() == test.size()) {
            pred = model.predict(test);
            cout << pred <<"\n";
        }
        else throw std::invalid_argument( "Invalid Size Input Vector" );
    }
    return 0;
}
