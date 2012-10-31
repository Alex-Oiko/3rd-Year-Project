//
//  Simulate.cpp
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#include <iostream>
#include "Simulate.h"
void Simulate::LoadFireAll(Task TASK, Dealer DEAL){
    event E;
    E.Type = LOADFIRE;
    E.Kd = -1;
    E.Value = -1;
    for(int node = 0; node < TASK.Nodes; node++){ //report initial state
        E.Ks = DEAL.MAP[node];
        EventQ.push(E);
    };
}
void Simulate::UpdateAll(Task TASK, Dealer DEAL){
    event E;
    E.Type = UPDATE;
    E.Ks = -1;
    E.Value = -1;
    for(int node = 0; node < TASK.Nodes; node++){ //report initial state
        E.Kd = DEAL.MAP[node];
        EventQ.push(E);
    };
}

void Simulate::SimBegin(Task TASK, Dealer DEAL, Core CORE, MakeMCTables MCT) {
    event E;
    int l = 0;
    long loops = 0;
    unsigned Thru=0;
    puts("Starting simulation");
    LoadFireAll(TASK, DEAL);
    while(true){
        if(EventQ.empty()){
            if(l == 40) break;
            UpdateAll(TASK, DEAL);
            LoadFireAll(TASK, DEAL); 
            l++;
        }
        E = EventQ.front();
        //printf("Event type = %d, %lu more events to go\n",E.Type, EventQ.size());
        //2cout << "Event type = " << E.Type << " " << EventQ.size() << " more events to go\n";
        EventQ.pop();
        if(E.Type == LOADFIRE)
            LoadFire(E, CORE, DEAL);
        else if(E.Type == UPDATE)
            Update(E, CORE);
        else if(E.Type == FIREAWAY)
            Thru += FireAway(E, MCT,CORE);  
        else {
            printf("unknown event %d\n",E.Type);
            exit(1);
        }
        loops++;
    }
    printf("%lu events in total\n%u Through routes\n",loops,Thru);
}
void Simulate::LoadFire(event E, Core CORE, Dealer DEAL){
    unsigned Xs, Ys, Cs, Os;
    KeyTo(E.Ks, Xs, Ys, Cs, Os);
    E.Value = CORE.AllCores[Xs][Ys][Cs].TargetTable[Os].Value;
    E.Kd = E.Ks;    //start off at source
    E.Type = FIREAWAY;
    E.OutLink = 6; //this is the start
    EventQ.push(E);
    return;
}
void Simulate::Update(event E, Core CORE){
    unsigned Xd, Yd, Cd, Od;
    unsigned Valency;
    unsigned *W, *Vp;
    long long Wt,Vt, WVt;
    
    KeyTo(E.Kd, Xd, Yd, Cd, Od);
    if(CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Fixed) //Boundary - don't touch
        return;
    Valency = CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Valency;
    W = CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Weights;
    Vp = CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].VinPoint;  
    WVt = 0;
    for(int node = 0; node < Valency; node++){
        Wt = (long long)W[node];
        Vt = (long long)CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];
        WVt += Wt*Vt;
    }    
    CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value = (unsigned)(WVt >> 31);
    return;
}
unsigned Simulate::FireAway(event E, MakeMCTables MCT, Core CORE){
    unsigned DATA, CORES, LINKS;
    unsigned Xd,Yd,Cd,Od;
    int XYn,Xn,Yn;
    unsigned ThroughRouted=0;
    KeyTo(E.Kd, Xd, Yd, Cd, Od);
    int link, core;
    DATA = MCT.GetMCData(E.Ks, E.Kd);
    LINKS = DATA&0x03f;
    if(DATA == 0){
        LINKS = (1<<E.OutLink);//through route
        ThroughRouted++;
    }
    link = 1;
    for(int i = 0;LINKS && i < 6; i++, link = (link<<1)){
        if(link&LINKS){
            E.OutLink = i;//set for through routing
            
            if(MCT.Wrap)
                XYn = MCT.NeighbourWrap(Xd, Yd, i);  
            else 
                XYn = MCT.Neighbour(Xd, Yd, i);
            MCT.RevertXY(XYn, Xn, Yn);
            E.Kd = KeyFrom(Xn, Yn, Cd, Od); //off to new destination
            EventQ.push(E);
         }     
    }
    CORES = (DATA>>7)&0x0ffff; //look only at cores 1 to 16 (0 to 15 in my data structures)   
    core = 1;
    for(int i = 0;CORES && i < 16; i++,core = (core<<1)){   //deliver as needed
        if(core&CORES){
            E.Kd = KeyFrom(Xd, Yd, i, 0);
            E.Type = DELIVER;
            Deliver(E, CORE);
        }
    }
    return ThroughRouted;
}
void Simulate::Deliver(event E, Core CORE){
    unsigned Xd, Yd, Cd, Od; 
    unsigned VinP;
    map<unsigned, unsigned>::iterator InVal;
    KeyTo(E.Kd,Xd, Yd, Cd, Od);
    InVal = CORE.AllCores[Xd][Yd][Cd].LUT.find(E.Ks);
    if(InVal == CORE.AllCores[Xd][Yd][Cd].LUT.end())
        return;     //delivered to disinterested core
    VinP = InVal->second;
    CORE.AllCores[Xd][Yd][Cd].ValuesIn[VinP] = E.Value;
    return;
}
