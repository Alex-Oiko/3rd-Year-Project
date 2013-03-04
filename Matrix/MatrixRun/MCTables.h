//
//  MCTables.h
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef __MatrixRun__MCTables__
#define __MatrixRun__MCTables__

#include <vector>
#include <stdlib.h>
#include "Utils.h"
using namespace std;
/*
 This class fills in the MC tables from information in th eTask graph and the topology.
 It starts with forming P2P tables from the selected machine topology,
 next the raw MC tables are formed, then through routing is implemented.
 The machine is an NX.NY (NX>=NY) Manchester grid with or without wrap around
 Sometimes Chips are given single value ids XY = X*NY + Y
 */
class MCLines{
private:
    uint32_t NX, NY;
    vector<uint32_t> **Key, **Mask, **Data;     //The raw MC table data
    uint32_t ConvertXY(uint32_t,uint32_t);
    
public:
    MCLines(uint32_t,uint32_t);
    void RevertXY(uint32_t,uint32_t&, uint32_t&);             //inverse of convert
    uint32_t Neighbour(uint32_t,uint32_t,uint32_t);               //on the grid
    uint32_t NeighbourWrap(uint32_t,uint32_t,uint32_t);           //ditto for wrapped grid
    uint32_t GetMCData(uint32_t,uint32_t);
    bool ReadData(string);
    void PrintMCT();
    uint32_t Route(uint32_t, uint32_t);
};


#endif /* defined(__MatrixRun__MCTables__) */
