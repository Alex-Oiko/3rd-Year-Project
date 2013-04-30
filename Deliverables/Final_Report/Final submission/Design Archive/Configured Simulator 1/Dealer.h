//
//  Dealer.h
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef Matrix_Dealer_h
#define Matrix_Dealer_h
#include <map>
#include <set>
#include "Utils.h"
#include "Cores.h"
#include "Machine.h"
#include "Task.h"

using namespace std;
/*
 This is the class that distributes the nodes (points) of the problem
 among the available processors.
 This distribution is either deal of the deck or random.
 */
class Core;
class Task;
class Dealer{
private:
    unsigned MaxChips;
    unsigned MaxCores;
    unsigned MaxOffsets;
    unsigned NextChip;              //Next available chip
    unsigned NextCore;              //Next available Core
    vector<vector<unsigned>> Offset;              //Next available offset for chip and core Offset[Chip][Core]
    bool StartingPlace(unsigned);
public:
    Dealer(const unsigned, const unsigned);
    vector<unsigned> MAP;                  //from the node id to the Key MAP[node] = Key
    map<unsigned, unsigned> PMAP;           // map of key to node number (to find all the keys in order
    vector<unsigned> RAN;                  //shuffled node ids
    void Deal(Machine&, Task&, bool, int, Core&);   //does the distribution
    void PrintMappings(Task&);
    int *MaxCoreNo;                 //per chip
};




#endif
