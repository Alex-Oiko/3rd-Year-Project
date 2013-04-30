//
//  MCTables.h
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef Matrix_MCTables_h
#define Matrix_MCTables_h

#include <vector>
#include <set>
#include <stdlib.h>
#include "Task.h"
#include "Utils.h"
#include "Machine.h"
#include "Dealer.h"
using namespace std;
#define REPORTKEY 0xffff7fff
#define STOPKEY 0xffff3fff
#define STARTKEY 0xffff1fff
#define ALLMASK 0xffffffff
#define ALLCORES 0xfffff800
/*
 This class fills in the MC tables from information in th eTask graph and the topology.
 It starts with forming P2P tables from the selected machine topology,
 next the raw MC tables are formed, then through routing is implemented.
 The machine is an NX.NY (NX>=NY) Manchester grid with or without wrap around
 Sometimes Chips are given single value ids XY = X*NY + Y
 */
class MakeMCTables{
private:
    unsigned ChipCount;
    vector<unsigned> **Key, **Mask, **Data;     //The raw MC table data
    vector<unsigned> **TKey, **TMask, **TData;  //MC table dat with through routing                           
    vector<vector<unsigned>>    P2P;            //P2P[XYs][XYd] = link from XYs to XYd
    int GetLink(int, int, int, int);            //link from A to B
    int GetTableCount();
    int NX,NY;
    void TreeRouteIn(unsigned, unsigned,Machine&);
    void TreeRouteOut(unsigned, unsigned,Machine&,Dealer&);
    void SetRoute(int, int, int, int,int, unsigned, unsigned,int, Machine&);
    void MCTEnter(int, int, int,unsigned, unsigned, unsigned,int);
    void PutMCLine(unsigned,unsigned,unsigned, unsigned, unsigned);
    
public:
    MakeMCTables(int, int, int);
    ~MakeMCTables();
    void PrintCommsTables(Task&, unsigned*,unsigned, unsigned);
    unsigned GetMCData(unsigned,unsigned);
    void FillTables(Task&,Machine&);                      //starting point
    void PrintTableSizes(int,int);
    void ThroughRoute(int,int,Machine&);                 //from raw to through routed MC tables
    void FANIO(Machine&,Dealer&);
    void PrintTTableSizes(int,int);
    void PrintTTables(int,int);
    void PrintTables(int,int);
    void MakeP2P(Machine&);
    void WriteMC(string);
};



#endif
