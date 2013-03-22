//
//  WriteCore.h
//  Matrix
//
//  Created by Jeff Reeve on 23/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef __Matrix__WriteCore__
#define __Matrix__WriteCore__

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Task.h"
#include "Dealer.h"
#include "Cores.h"
#include "MCTables.h"
#include "Utils.h"
using namespace std;
class WriteCore{ 
public:
    void BinaryDump(string, Task&, Dealer&, Core&, MakeMCTables&);
private:
    struct _CoreCommon{
        uint32_t PointCount;
        uint32_t LUTStart;
        uint32_t LUTCount;
        uint32_t TTStart;
        uint32_t ValuesStart;
    };
    struct _DTTE{
        uint32_t Kd;
        vector<queue<uint32_t>> OpCodes;
        float IV;
	float Temp;
	char Name;
	uint32_t YD;
	uint32_t X;
	uint32_t Y;
	uint32_t counter;
        uint32_t oV;
        uint32_t Expected;
        uint32_t Arrived;
    };
    struct _LookUp {
        uint32_t Ks;
        uint32_t idx;
    };
    unsigned PointCount, LUTSize;;
    _DTTE *TTArray;                 //Array for the target table entries
    float *Values;               //Array for the values
    float *Temps;		//Array for the Temp values
    uint32_t *counters;		//Array for the counters
    vector<queue<uint32_t>> *OpCodesA;  //Array for OpCodes
    _LookUp  *LUT;                  //Look up table for incoming
    unsigned UsedCores(unsigned,Dealer&);
    unsigned UsedChips(Dealer&);
    void DumpCore(unsigned,Core&, Dealer&, Task&);  //Write the core with this Key&0xfffff800 returns the size of the arrays in 32bit words.
};
#endif /* defined(__Matrix__WriteCore__) */
