//
//  Simulate.h
//  Matrix
//
//  Created by Jeff Reeve on 13/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef __Matrix__Simulate__
#define __Matrix__Simulate__

#include <iostream>

#include <queue>
#include "Task.h"
#include "Dealer.h"
#include "Cores.h"
#include "Utils.h"
#include "MCTables.h"
#include "Machine.h"
using namespace std;
/*
 This class runs an event driven simulation of the problem.
 event:LOADFIRE loads the source value and queues a FIREAWAY
 event:FIREAWAY delivers to cores if at destination,
 otherwise schedules a new FIREAWAY to new destination
 event:UPDATE calcultaes the value of a node
 */
class Simulate{
private:
    enum EType{LOADFIRE,UPDATE,DELIVER,FIREAWAY};
    struct event{
        EType Type;     // LOADFIRE UPDATE FIREAWAY
        unsigned Ks;    // source key
        unsigned Kd;    // destination key
        float Value; // fired value
        unsigned OutLink; //for through routing
    };
    queue<event> EventQ;
    void LoadFire(event, Core&, Dealer&);
    void Update(event, Core&);
    void Deliver(event, Core&);
    unsigned FireAway(event, MakeMCTables&,Core&,Machine&);
    void LoadFireAll(Task&, Dealer&, Core&);
    void UpdateAll(Task&, Dealer&);
    unsigned long CoreHits, CoreMisses;
public:
    void SimBegin(Task&, Dealer&, Core&, MakeMCTables&,Machine&);
    unsigned matrix_size;
};


#endif /* defined(__Matrix__Simulate__) */
