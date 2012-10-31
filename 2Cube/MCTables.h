//
//  MCTables.h
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#ifndef TimeMarch_MCTables_h
#define TimeMarch_MCTables_h
#include <vector>
#include <stdlib.h>
#include "Task.h"
#include "Utils.h"
using namespace std;
/*
    This class fills in the MC tables from information in th eTask graph and the topology.
    It starts with forming P2P tables from the selected machine topology,
    next the raw MC tables are formed, then through routing is implemented.
    The machine is an NX.NY (NX>=NY) Manchester grid with or without wrap around
    Sometimes Chips are given single value ids XY = X*NY + Y
*/
class MakeMCTables{
private:
    vector<unsigned> **Key, **Mask, **Data;     //The raw MC table data
    vector<unsigned> **TKey, **TMask, **TData;  //MC table dat with through routing
    unsigned **P2P;                             //P2P[XYs][XYd] = link from XYs to XYd
    int ConvertXY(int, int);                    //calculated XY
    int GetLink(int, int, int, int);            //link from A to B
    int GetTableCount();

public:
    int NX, NY;                                 //extent of grid
    bool Wrap;                                  //Wrapped topology or not
    MakeMCTables(int, int);
    void RevertXY(int, int&, int&);             //inverse of convert
    int Neighbour(int, int, int);               //on the grid
    int NeighbourWrap(int, int, int);           //ditto for wrapped grid
    void PrintCommsTables(Task, unsigned*,unsigned, unsigned);
    void SetRoute(int, int, int, int,int, unsigned, unsigned,int);
    void MCTEnter(int, int, int,unsigned, unsigned, unsigned,int);
    unsigned GetMCData(unsigned,unsigned);
    void FillTables(Task);                      //starting point
    void PrintTableSizes(int,int);
    void ThroughRoute(int,int);                 //from raw to through routed MC tables
    void PrintTTableSizes(int,int); 
    void PrintTTables(int,int);
    void PrintTables(int,int);
    void MakeP2P(int, int, bool);    
    void WriteMC(string);
};
#endif
