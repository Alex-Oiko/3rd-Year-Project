//
//  Simulate.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 27/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "Simulate.h"
#include <iostream>
#include <iomanip>
using namespace std;

void Simulate::PrintByOpCode(unsigned OpCode, TCram& TCRAM){
    unsigned X,Y,C,Lup;

    uint32_t *CoreData;
    float *Values;
    TCram::_CoreCommon *CoreCommon;
    TCram::_DTTE *TTE;
    for(X= 0; X < TCRAM.NX; X++)
        for(Y=0; Y < TCRAM.NY; Y++)
            for(C = 0; C< TCRAM.NC; C++){
                CoreData = TCRAM.TCData[X][Y][C];
                if(CoreData == NULL)
                    continue;
                CoreCommon = (TCram::_CoreCommon*)CoreData;
                Values = (float*)&CoreData[CoreCommon->ValuesStart];
                TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
                for(Lup = 0; Lup < CoreCommon->PointCount; Lup++){
                    if(TTE[Lup].OpCode == OpCode){
                        cout << "Kd = "<<hex<<TTE[Lup].Kd<<" OpCode = "<<dec<<TTE[Lup].OpCode<<" IV = "<<TTE[Lup].IV<<" V = "<< Values[TTE[Lup].oV]<<" Expected "<<TTE[Lup].Expected<<" Arrived "<<TTE[Lup].Arrived<<endl;
                    }
                }
            }
}







void Simulate::LoadFireAll(TCram& TCRAM){
    unsigned X,Y,C,Lup;
    event E;
    uint32_t *CoreData;

    TCram::_CoreCommon *CoreCommon;
    TCram::_DTTE *TTE;
    E.Type = FIREAWAY;
    E.OutLink = 6;
    for(X= 0; X < TCRAM.NX; X++)
        for(Y=0; Y < TCRAM.NY; Y++)
            for(C = 0; C< TCRAM.NC; C++){
                CoreData = TCRAM.TCData[X][Y][C];
                if(CoreData == NULL)
                    continue;
                CoreCommon = (TCram::_CoreCommon*)CoreData;
                TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
                for(Lup = 0; Lup < CoreCommon->PointCount; Lup++){
                    if(TTE[Lup].OpCode == 3){
                        E.Value = TTE[Lup].IV;
                        E.Ks = TTE[Lup].Kd;
                        E.Kd = E.Ks;
                        EventQ.push(E);
 
                    }
                }
            }
}

void Simulate::SimBegin(TCram& TCRAM, MCLines& MCT, Machine& MAC) {
    event E;
    long loops = 0;
    unsigned Thru=0;
    CoreHits = 0;
    CoreMisses = 0;
    puts("Starting simulation");
    LoadFireAll(TCRAM);
    while(true){
        if(EventQ.empty())
            break;
        E = EventQ.front();
        EventQ.pop();
        if(E.Type == UPDATE)
            Update(E,TCRAM);
        else if(E.Type == FIREAWAY)
            Thru += FireAway(E, MCT,TCRAM,MAC);
        else {
            printf("unknown event %d\n",E.Type);
            exit(1);
        }
        loops++;
    }
    printf("%lu events in total\n%u Through routes\n",loops,Thru);
    printf("%lu core hits and %lu core misses\n", CoreHits, CoreMisses);
    PrintByOpCode(2, TCRAM);
    
}
void Simulate::Update(event E, TCram& TCRAM){  //this is the timer interrupt
}

unsigned Simulate::FireAway(event E, MCLines& MCT, TCram& TCRAM, Machine& MAC){
    unsigned DATA, CORES, LINKS, ChipID;
    unsigned Xd,Yd,Cd,Od,Kd;
    int XYn,Xn,Yn, XYd;
    unsigned ThroughRouted=0;
    int NextChip;
    map<unsigned, unsigned>::iterator CID;
    KeyTo(E.Kd, Xd, Yd, Cd, Od);
    XYd = (Xd<<8) | Yd;
    CID = MAC.XYChip.find(XYd);
    ChipID = CID->second;
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
            NextChip = MAC.ChipLink[ChipID][i];
            XYn = MAC.ChipXY[NextChip];
            Xn = XYn>>8;
            Yn = XYn&0xff;
            E.Kd = KeyFrom(Xn, Yn, Cd, Od); //off to new destination
            EventQ.push(E);
        }
    }
    CORES = (DATA>>7)&0x0ffff; //look only at cores 1 to 16 (0 to 15 in my data structures)
    core = 1;
    for(int i = 0;CORES && i < 16; i++,core = (core<<1)){   //deliver as needed
        if(core&CORES){
            Kd = KeyFrom(Xd, Yd, i, 0);
            InComing(E.Ks,E.Value,Kd,TCRAM);
        }
    }
    return ThroughRouted;
}

void Simulate::InComing(uint32_t Ks, float Vs, uint32_t Kd, TCram& TCRAM){
    uint32_t Xd, Yd, Cd, Od;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    uint32_t LUTCount = 0, InPoint;
    uint32_t *CoreData;
    float Vme, *Values;
    TCram::_LookUp *LUT;
    TCram::_CoreCommon *CoreCommon;
    TCram::_DTTE *TTE, dTTE;
    bool found = false;
    event RES;
    CoreData = TCRAM.TCData[Xd][Yd][Cd];
    CoreCommon = (TCram::_CoreCommon*)CoreData;
    LUTCount = CoreCommon->LUTCount;
    LUT = (TCram::_LookUp*)&CoreData[CoreCommon->LUTStart];
    TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
    Values = (float*)&CoreData[CoreCommon->ValuesStart];
    for(int k = 0; k < LUTCount ; k++){
        if(Ks == LUT[k].Ks){
            found = true;
            CoreHits++;
            InPoint = LUT[k].idx;
            dTTE = TTE[InPoint];
            if(dTTE.OpCode == 2){
                Vme = Values[dTTE.oV];
                float NewValue = Vme + Vs;
                Values[dTTE.oV] = NewValue;
                TTE[InPoint].Arrived++;
            }
            if(dTTE.OpCode == 1){
                Vme = Values[dTTE.oV];
                float NewValue = Vme*Vs;
                Values[dTTE.oV] = NewValue;
                dTTE.Arrived++;
                TTE[InPoint].Arrived++;
                if(dTTE.Arrived == dTTE.Expected){
                    dTTE.Arrived = 0;
                    TTE[InPoint].Arrived=0;
                    RES.Ks = dTTE.Kd;
                    RES.Kd = dTTE.Kd;
                    RES.OutLink = 6;
                    RES.Value = NewValue;
                    RES.Type = FIREAWAY;
                    EventQ.push(RES);
                }
            }
            
        }
        if(LUT[k].Ks > Ks)
            break;
    }
    if(!found)
        CoreMisses++;
}






