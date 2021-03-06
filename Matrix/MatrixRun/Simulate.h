//
//  Simulate.h
//  MatrixRun
//
//  Created by Jeff Reeve on 27/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef __MatrixRun__Simulate__
#define __MatrixRun__Simulate__

#include <iostream>
#include <queue>
#include "TCRAM.h"
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
    enum EType{LOADFIRE,UPDATE,DELIVER,FIREAWAY,UPALL};
    struct event{
        EType Type;     // LOADFIRE UPDATE FIREAWAY UPALL
        unsigned Ks;    // source key
        unsigned Kd;    // destination key
        float Value; // fired value
        unsigned OutLink; //for through routing
    };
    queue<event> EventQ;
    void LoadFire(event, TCram&);
    void Update(event, TCram&);
    void UpdateAll(TCram&);
    void Deliver(event, TCram&);
    unsigned FireAway(event, MCLines&,TCram&,Machine&);
    void LoadFireAll(TCram&);
    unsigned long CoreHits, CoreMisses;
    void InComing(uint32_t, float, uint32_t , TCram&);
    void PrintByOpCode(unsigned,unsigned, TCram&);
    uint32_t front(uint32_t,uint32_t*);
    uint32_t pop(uint32_t,uint32_t*);
    uint32_t pushandpop(uint32_t,uint32_t*);
public:
    void SimBegin(TCram&, MCLines&,Machine&);
    unsigned matrix_size;
    uint32_t case15_counter;
    uint32_t iterator_counter;
};

#endif /* defined(__MatrixRun__Simulate__) */
