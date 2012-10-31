//
//  Cores.h
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#ifndef TimeMarch_Cores_h
#define TimeMarch_Cores_h
#include <map>
#include <vector>
#include <stdlib.h>
#include "Task.h"
#include "Dealer.h"
#include "Utils.h"
using namespace std;
class Dealer;
class Task;
//This class looks after the memory for each core (the TCRAM)
class Core{
private:
    struct TargetTableEntry{        //There are CoreData.NodesOnCore of these
        unsigned Od;                //identifies this particular node (Key actually)
        unsigned Fixed;             //1 = boundary, 0 otherwise
        unsigned Valency;           //number of contributing nodes
        unsigned Arrived;           //number of nodes that have arrived
        unsigned Value;             //the value of thuis node
        unsigned *Weights;          //for the weighted sum
        unsigned *VinPoint;         //places in Values in to find each contributing value
    };
    struct CoreData{                //one of these per core
        int NodesOnCore;
        TargetTableEntry *TargetTable;
        unsigned NoValuesIn;
        unsigned *ValuesIn;         //storage for incoming values
        map<unsigned,unsigned> LUT; //LUT[Ks] = offset in ValuesIn for Value placement
    };
    int UsedCores(int, int, int);
    int UsedChips(int, int, int);
    uint32_t* FillCoreDump(int, int, int,  uint32_t&, uint32_t*);
    uint32_t MyWalloc(int,bool);
public:
    CoreData ***AllCores; //particular core is AllCores[X][Y][C]
    Core(unsigned, unsigned, unsigned);
    void MakeTargetTables(Task, Dealer, int, int, int);
    void PrintValues(Dealer, Task);
    void PrintLUT(int,int,int);
    void WriteTC(string, int, int, int);
};
#endif
