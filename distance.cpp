//here we will implement all the class functions
#pragma once
#include "distance.h"
#include "man.h"
#include "euc.h"
#include "cheb.h"
#include "mink.h"
#include "can.h"
#include "knn.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>
#include <math.h>


using namespace std;

//implememting constructor

Distance::Distance()  //:: thats how you access the functions of the class
{
}

//implementing constructor
Distance::~Distance() 
{
  //dont need 'this' bc no arguments 

}
double Distance::measure(vector<double>& u, vector<double>& v) {
    return 0.0;
    
}
vector<double> Distance::getAbs(int p, int denom, vector<double>& u, vector<double>& v) 
{
    int size = u.size();
    static vector<double> dist;
    dist.clear();
    for(int i = 0; i < size; i++) {
        double a = abs((u[i] - v[i]));
        if (denom == 2) {
            int d = abs(u[i]) + abs(v[i]);
            double b = (a / d);
            a = b;
        }
        double v = pow(a, p);
        if (p == INT_MAX) {
            v = a;
        }
        dist.insert(dist.begin() + i, v);
        }
    return dist;
}

//implementing functions
double Distance::generalDist(int p, int denom, vector<double>& u, vector<double>& v) 
{
    double dist = 0;
    double s = 0.0;
    vector<double> abso;
    abso = getAbs(p, denom, u, v);
    if (p == INT_MAX) {
        dist = *max_element(abso.begin(), abso.end());
        return dist;
    }
    s = accumulate(abso.begin(), abso.end(), 0.0);
    double pu = 1.0 / p;
    dist = pow(s, pu);
    abso.clear();
    return dist;

}
double Euc::measure(vector<double>& u, vector<double>& v) {
    return generalDist(2, 1, u, v);
}

double Man::measure(vector<double>& u, vector<double>& v) {
    return generalDist(1, 1, u, v);
}

double Cheb::measure(vector<double>& u, vector<double>& v) {
    return generalDist(INT_MAX, 1, u, v);
}

double Can::measure(vector<double>& u, vector<double>& v) {
    return generalDist(1, 2, u, v);
}

double Mink::measure(vector<double>& u, vector<double>& v) {
    return generalDist(p, 1, u, v);
}
void Mink::setP(int pi) {
    this->p = pi;
}

