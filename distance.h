//file of functions 
#pragma once //adds all the imports from here to all the other files as long as I include this file to the others
#include <iostream>
using namespace std;
#include <vector>


class Distance
{
    public: 
        Distance(); //constructor 
        ~Distance(); //destructor
        virtual double measure(vector<double>& u, vector<double>& v) = 0;

    protected: 
        //declaring distance funtions
        vector<double> getAbs(int p, int denom, vector<double>& u, vector<double>& v);
        double generalDist(int p, int denom, vector<double>& u, vector<double>& v);    
 
};

