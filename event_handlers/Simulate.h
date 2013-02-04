//
//  Simulate.h
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#ifndef TimeMarch_Simulate_h
#define TimeMarch_Simulate_h

#include <queue>
#include "Task.h"
#include "Dealer.h"
#include "Cores.h"
#include "Utils.h"
#include "MCTables.h"
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
    enum EType{LOADFIRE,CALCULATE,DELIVER,FIREAWAY};
    struct event{
        EType Type;     // LOADFIRE UPDATE FIREAWAY
        unsigned Ks;    // source key
        unsigned Kd;    // destination key
        unsigned Value; // fired value
        unsigned OutLink; //for through routing
    };
    queue<event> EventQ;
    queue<unsigned> KeysQ;
    void LoadFire(event, Core, Dealer);
    void Update(event, Core);
    void Deliver(event, Core);
    unsigned FireAway(event, MakeMCTables,Core);
    void ComputingInX(Event E,Core CORE,Deal DEAL);
    void ComputinInA(Core CORE);
public:
    void SimBegin(Task, Dealer, Core, MakeMCTables);
    
};


#endif
