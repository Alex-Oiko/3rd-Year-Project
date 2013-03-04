//
//  main.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include <iostream>

#include <stdlib.h>
#include "TCRAM.h"
#include "MCTables.h"
#include "Simulate.h"
#include "Machine.h"
#define CD 16
using namespace std;
int main(int argc, const char * argv[])
{
    string MCTableFile, TCDataFile;
    uint32_t XD=0, YD=0;
    string ProblemName;
    int MachineType, MachineSize;
    cout << "This is MatrixRun\nGive me a program root name matey ";
    cin >> ProblemName;
    MCTableFile = (ProblemName+".SPINNmcta");
    TCDataFile = (ProblemName+".SPINNjram");
    
    
    
    printf("Machine is a Rectangle(0), Torus(1) or Board48(2)? ");
    scanf("%d",&MachineType);
    if(MachineType == 0 || MachineType == 1){
        printf("Give me a Grid Size, NX NY ");
        scanf("%d %d",&XD,&YD);
        MachineSize = XD*YD;
    }
    else {
        XD = 8;
        YD = 8;
        MachineSize = 48;
    }
    if(YD>XD)
        swap(XD,YD);
    Machine MAC(MachineType,XD,YD,MachineSize);

    
    TCram TC(XD, YD, CD);
    TC.ReadData(TCDataFile);
    MCLines MC(XD, YD);
    MC.ReadData(MCTableFile);
    MC.PrintMCT();
    TC.PrintTCram();
    Simulate SIM;
    SIM.SimBegin(TC, MC, MAC);
    //TC.PrintTCram();
    return 0;
}



