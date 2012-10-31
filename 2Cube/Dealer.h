//
//  Dealer.h
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#ifndef TimeMarch_Dealer_h
#define TimeMarch_Dealer_h
#include <map>
#include "Utils.h"
#include "Cores.h"
using namespace std;
/*
    This is the class that distributes the nodes (points) of the problem 
    among the available processors.
    This distribution is either deal of the deck or random.
*/ 
class Core;
class Dealer{
private:
    
public:
    unsigned *MAP;                  //from the node id to the Key MAP[node] = Key
    unsigned *RAN;                  //shuffled node ids
    map<unsigned, unsigned> PMAP;   //from the Key to the node id PMAP[Key] = node
    void Deal(int, int, int, bool, int, Core);   //does the distribution
    void PrintMappings(int);
};
#endif
