//
//  TCRAM.h
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef __MatrixRun__TCRAM__
#define __MatrixRun__TCRAM__

#include <iostream>
#include <string>
#include <queue>
#include "MCTables.h"
#include "Utils.h"
using namespace std;
class TCram{
public:
    TCram(uint32_t, uint32_t, uint32_t);
    bool ReadData(string);
    void PrintTCram();
    bool InComing(uint32_t, float , uint32_t);

    struct _CoreCommon{
        uint32_t PointCount;
        uint32_t LUTStart;
        uint32_t LUTCount;
        uint32_t TTStart;
        uint32_t ValuesStart;
    };
    struct _DTTE{
        uint32_t Kd;
	uint32_t OpCode;
	char Name;
	uint32_t YD;
        uint32_t Y;
	uint32_t X;
	float temp;
	uint32_t counter;
        float IV;
        uint32_t oV;
	uint32_t V;
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
    _LookUp  *LUT;                  //Look up table for incoming
    //uint32_t *OpCodes;
    uint32_t holder_size;
    queue<uint32_t> **OpCodesA;
    uint32_t *counters;
    float *Temps;
    uint32_t ****TCData;
    uint32_t NX,NY,NC;
};

#endif /* defined(__MatrixRun__TCRAM__) */
