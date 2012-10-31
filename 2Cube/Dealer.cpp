//
//  Dealer.cpp
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "Dealer.h"
#define MaxOffsets 2048
#define CD 16   //max cores per chip
//The algorithm spreads Nodes on cores as thinly as possible
//XD, YD define the extent of the machine grid
//Nodes is the number of nodes (points) in the problem
//Toss - random distribution or not
//seed - for random number generator
//CORE - here to record the number of nodes per core
void Dealer::Deal(int XD, int YD, int Nodes, bool Toss, int seed, Core CORE){
    int X,Y,C,O,n;
    
    unsigned TMP,p1,p2;
    unsigned K;
    bool done= false;
    MAP = new unsigned[Nodes];
    RAN = new unsigned[Nodes];
    for(n=0;n<Nodes;n++)
        RAN[n] = n;
    if(Toss){
        srand48(seed);  
        for(n=0;n<Nodes;n++){
            p1 = lrand48()%Nodes;
            p2 = lrand48()%Nodes;
            TMP = RAN[p1];
            RAN[p1] = RAN[p2];
            RAN[p2] = TMP;
        }
    }
    n=0;
    for(O=0;!done && O<MaxOffsets; O++)
        for(C = 0; !done && C < CD; C++)
            for(Y=0; !done && Y < YD;Y++)
                for(X=0;!done && X < XD; X++){
                    K = KeyFrom(X,Y,C,O);
                    MAP[RAN[n]] = K;
                    PMAP.insert(map<unsigned, unsigned>::value_type(K,RAN[n]));
                    n++;
                    CORE.AllCores[X][Y][C].NodesOnCore++;
                    done = (n == Nodes);
                }
}
void Dealer::PrintMappings(int Nodes){
    int node;
    puts("Node  Key KeyFromMap"); 
    for (node = 0; node < Nodes; node++) {
        printf("%u  %x %u \n",node,MAP[node],PMAP[MAP[node]]);
    }
    
}
