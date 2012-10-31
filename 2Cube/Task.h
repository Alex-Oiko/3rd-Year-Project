//
//  Task.h
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#ifndef TimeMarch_Task_h
#define TimeMarch_Task_h

#define BUFFER 32768
#include <map>
#include "Dealer.h"
using namespace std;
class Dealer;
/*
    This class reads the task file and puts it into internal format
    The file format is: (all are unsigned integers)
    Nodes MaxValence
    Node# Value Boundary Valency #Arrived snode# weight value ..... (Valence entries)
    .
    . (Nodes lines like this)
    .
*/    
class Task{
private:
    FILE *TaskFile;
    char buffer[BUFFER];
    char number[BUFFER];
    int GetNextInt();
    void GetLine();
public: 
    /*
     This is the receiving Node so:-
     Value = sum (Weights*Values)
     */
    struct Node{
        unsigned Value;     //temperature?
        unsigned Fixed;     //1 means on a boundary - 0 otherwise
        int Valency;        //number attached
        int Arrived;        //the number of values that have arrived
        unsigned *Nodes;    //Node number of driving nodes
        unsigned *Weights;  //weight
        unsigned *Values;   //temperatures?
    };
    
    multimap<unsigned, unsigned>    KsKd;   //from source to destination   
    multimap<unsigned, unsigned>    KdKs;   //from destination to source
    Node *NodeList;   
    
    int Nodes;
    void PrintNodeList();
    void ReadNodeList(const char*);
    void FillKeyMaps(Dealer);
    void PrintKeyMaps();    
};




#endif
