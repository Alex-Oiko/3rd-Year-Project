//
//  Task.h
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef Matrix_Task_h
#define Matrix_Task_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include "Dealer.h"
#include "Cores.h"
using namespace std;
class Dealer;
class Core;
class Task{
public:
    unsigned nextPID;
    unsigned DataSize;
    bool readTask(string);
    void printConnections();
    map<string,unsigned> pidStart;              //mapping of variable names to process ID starting value
    map<string, unsigned> OpCodes;              //OpCodes for each element
    map<unsigned,string> pidName;               //mapping of process ID starting value to variable name
    map<string,unsigned> nameSize;              //mapping of variable names to sizes
    map<string,unsigned> nameD1;                //mapping of variable name to first dimension
    map<string,unsigned> nameD2;                //mapping of variable name to second dimension
    map<string,string> overlays12;                //overlays 1st value is first declared second id is the second declaration
    map<string,string> overlays21;                //overlays 1st value is second declared second id is the first declaration
    multimap<unsigned, unsigned> Connections;   //list of connections in PIDs source to destination
    map<string,float**> mStore;                 //map of name to variable values
    void printMatrixValues();                   //look at the values of the matrices
    multimap<unsigned, unsigned>    KsKd;       //from source to destination
    void FillKeyMaps(Dealer&);
    string elementFromPID(const unsigned);
    multimap<string,TargetTableEntry>  CoreFill;         //info for Cores in pid order
    vector<TargetTableEntry>  CoreFlix;
private:
    ifstream inTask;
    string inLine;
    int lineNumber;
    bool parseTask();
    enum state {BEGIN_VALUES, END_VALUES, BEGIN_OVERLAYS, END_OVERLAYS, BEGIN_CONNECTIONS, END_CONNECTIONS, ERROR, END};
    enum dimension {SCALAR, VECTOR, MATRIX2, ROGUE};
    state interpret(state);
    bool readMatrix();
    bool readFloats(vector<float>&, int);
    void split(const string&, char, vector<string>& );
    void lprint();
    bool allocatePIDs(string, unsigned, unsigned);
    bool pidFromName(string,unsigned&);
    bool readOverlay();
    bool readConnection();
    void makeMatrix(string, unsigned, unsigned);
    void CoreFiller();
};


#endif
