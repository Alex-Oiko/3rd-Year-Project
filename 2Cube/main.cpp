//
//  main.cpp
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "Task.h"
#include "MCTables.h"
#include "Dealer.h"
#include "Cores.h"
#include "Simulate.h"
#define CD 16
int main(int argc, const char * argv[])
{
    int XD,YD,Toss=false,Seed, Period, sim, write;
    const char *GridFile;
    bool Wrap = false;
    string ProblemName;
    Task TASK;
    Dealer DEAL;
    Simulate SIM;
    cout << "This is TimeMarch\nGive me a program root name matey ";
    cin >> ProblemName;
    GridFile = (ProblemName+".task").c_str();
    TASK.ReadNodeList(GridFile);    
    printf("Give me a Grid Size, NX NY ");     
    scanf("%d %d",&XD,&YD);    
    if(YD>XD)
        swap(XD,YD);
    MakeMCTables MCT(XD,YD);  
    MCT.MakeP2P(XD, YD, Wrap);  

    Core CORE(XD,YD,CD);
   
    printf("With Wrapped(input 1) or open(input 0) boundaries ");    
    scanf("%d",&Period);  
    if(Period != 0)
        Wrap = true;
    printf("Placement:- Random(input seed) or Deal off the deck(input 0)  ");    
    scanf("%d",&Seed);  
    if(Seed != 0)
        Toss = true;
    DEAL.Deal(XD, YD, TASK.Nodes, Toss, Seed, CORE);    
    TASK.FillKeyMaps(DEAL);  
    MCT.FillTables(TASK);    
    MCT.ThroughRoute(XD, YD);    
    CORE.MakeTargetTables(TASK, DEAL,XD,YD,CD);    
  
    printf("Run simualtion(input 1) or not(input 0) ");    
    scanf("%d",&sim);  
    if(sim != 0){
        SIM.SimBegin(TASK, DEAL, CORE, MCT);
        CORE.PrintValues(DEAL, TASK);

    }
    printf("Write output files(input 1) or not(input 0) ");    
    scanf("%d",&write);  
    if(write != 0){
        CORE.WriteTC(ProblemName,XD,YD,CD);
        MCT.WriteMC(ProblemName);
    }
    return 0;
}


