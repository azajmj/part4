//
// Created by Lenovo on 10/01/2023.
//


#pragma once
#include <string.h>
//#include KNN..
#include "commands.h"

using namespace std;

class CLI {
    // array of commands.
    vector<Command*> commands;
    // need to make a field of the KNN, probably..
    DefaultIO* dio;
    // you can add data members
    CommonData* commonData;
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};


