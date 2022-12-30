#pragma once //adds all the imports from here to all the other files as long as I include this file to the others
#include <iostream>
using namespace std;
#include <vector>
#include "distance.h"

class Euc:public Distance
{
    public: 
    double measure(vector<double>& u, vector<double>& v);
};