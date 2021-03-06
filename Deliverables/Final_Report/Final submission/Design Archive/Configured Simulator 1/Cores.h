//
//  Cores.h
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef Matrix_Cores_h
#define Matrix_Cores_h

#include <map>
#include <vector>
#include <stdlib.h>
#include "Task.h"
#include "Utils.h"
#include "Machine.h"
using namespace std;
class Dealer;
class Task;
#define CD 16
//This class looks after the memory for each core (the TCRAM)
class Core{
public:
    map<unsigned,vector<unsigned>> LUT;  //mapping of Keys to node info
    map<unsigned,TargetTableEntry> CoreEntries;  //mostly filled in by Dealer
    void MakeLUT(Task&);
    void PrintLUT();
    void PrintTTE(TargetTableEntry&,unsigned Offset);
    vector<float>  Mstore;       //storage for values Mstore[X][Y][C][]
    vector<vector<queue<unsigned>>> mop;
    vector<float> Mtemp;
    vector<unsigned> Mcounter;
    void AllocateStorage(Task&);
    void PrintByOpCode(unsigned,unsigned);
};



#endif
