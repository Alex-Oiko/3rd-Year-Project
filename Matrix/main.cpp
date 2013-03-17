//
//  main.cpp
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//
#include <iostream>
using namespace std;
#include "Task.h"
#include "Machine.h"
#include "Dealer.h"
#include "MCTables.h"
#include "Cores.h"
#include "Simulate.h"
//#include "WriteCore.h"
#define CD 16
int main(int argc, const char * argv[])
{
    Task TASK;
    
    string inFile;
    unsigned MachineType, MachineSize, XD,YD;
    unsigned Seed = 0,SIMrun=0,WriteOut=0;
    bool Toss = false;
    //Ask for the task
    cout << "Matrix handling on SpiNNaker\nA root file name if you would matey: ";
    cin >> inFile;
    if(!TASK.readTask(inFile)){
        cerr << "file reading not good matey\n";
        return 1;
    }
    else {
        cerr << "file "+inFile+" read ok matey\n";
    }
    TASK.printConnections();
    TASK.printMatrixValues();
    //Ask for the machine
    printf("Machine is a Rectangle(0), Torus(1) or Board48(2)? ");
    cin >> MachineType;
    if(MachineType == 0 || MachineType == 1){
        cout << "Give me a Grid Size, NX NY ";
        cin >> XD >> YD;
        MachineSize = XD*YD;
    }
    else {
        XD = 8;
        YD = 8;
        MachineSize = 48;
    }
    if(YD>XD)
        swap(XD,YD);
    Machine MAC(MachineType, XD, YD, MachineSize);
    //Get ready
    MakeMCTables MCT(XD,YD,MAC.ChipCount);
    MCT.MakeP2P(MAC);
    Core CORE;
    //Ask about the placement.
    cout << "Placement:- Random(TASK seed) or Deal off the deck(TASK 0)  ";
    cin >> Seed;
    Toss = (Seed !=0);
    Dealer DEAL(MachineSize, CD);
    DEAL.Deal(MAC, TASK, Toss, Seed, CORE);
    DEAL.PrintMappings(TASK);
    //prepare keys for constructing the MC-Tables
    TASK.FillKeyMaps(DEAL);
    //construct the MC-Tables
    MCT.FillTables(TASK, MAC);
    MCT.FANIO(MAC, DEAL);
    MCT.ThroughRoute(XD, YD, MAC);
    CORE.MakeLUT(TASK);
    CORE.PrintLUT();
    MCT.PrintTTables(1, 1);
    cout << "Run simulation(1) or not(0)? ";
    cin >> SIMrun;
    if(SIMrun){
        Simulate SIM;
        SIM.SimBegin(TASK, DEAL, CORE, MCT, MAC);
    }
    //cout << "Write binaries(1) or not(0)? ";
    //cin >> WriteOut;
    //if(WriteOut){
    //	MCT.WriteMC(inFile);
    //	WriteCore WC;
    //	WC.BinaryDump(inFile, TASK, DEAL, CORE, MCT);
    //}

    return 0;
}

