//
//  Utils.h
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef Matrix_Utils_h
#define Matrix_Utils_h
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <queue>
using namespace std;

static void KeyTo(unsigned,unsigned&, unsigned&, unsigned&, unsigned&);
static unsigned KeyFrom(unsigned, unsigned, unsigned, unsigned);
static void KeyTo(unsigned Key,unsigned& X, unsigned& Y, unsigned& C, unsigned& O){
    X = Key>>24;
    Y = (Key>>16)&0x0ff;
    C = (Key >> 11)&0x0f;
    O = (Key&0x07FF);
}
static unsigned KeyFrom(unsigned X, unsigned Y, unsigned C, unsigned O){
    return (unsigned)((X<<24)+(Y<<16)+(C<<11)+O);
}

struct TargetTableEntry{        //There are CoreData.NodesOnCore of these
    unsigned Kd;                //identifies this particular node (Key actually)
    vector<queue<unsigned>> OpCodes;   //identifies what function to use on MC message arrival, based on the type of the core that sent it
    char Name[4];              //Name of the node as from the input file
    unsigned XD;                //X dimension of the matrix element
    unsigned YD;                //Y dimension of the matrix element
    unsigned X;                 //X position of element Name[X][Y]
    unsigned Y;                 //X position of element Name[X][Y]
    float IV;                //initial value
    float Temp;
    unsigned counter;
    unsigned V;                //array offset pointer to my value = pid for simulation here.
};


#endif
