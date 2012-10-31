//
//  Cores.cpp
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#include <iostream>
#include "Cores.h"

Core::Core(unsigned XD, unsigned YD, unsigned CD){
    AllCores = new CoreData**[XD];
    for(int X = 0; X < XD; X++){
        AllCores[X] = new CoreData*[YD];
        for(int Y = 0; Y< YD;Y++){
            AllCores[X][Y] = new CoreData[CD];
            for(int C = 0; C < CD; C++){
                AllCores[X][Y][C].NodesOnCore=0;
            }
        }
    }
    
}

/*
    Fill in the AllCores data structures.
    XD,YD is the size of the machine grid.
    CD is the number of cores per chip
    TASK is the task graph
    DEAL gives the distribution of nodes
*/
void Core::MakeTargetTables(Task TASK, Dealer DEAL, int XD, int YD, int CD){
    unsigned Xd,Yd,Cd,Od;
    unsigned Kd,Ks,Possie;
    int node,V,NodeCount,B,source,InNode;
    TargetTableEntry *TE;
    map<unsigned, unsigned>::iterator InVal;
    printf("Setting up target tables \n");
    for (Xd=0; Xd < XD; Xd++) {
        for (Yd=0; Yd < YD; Yd++) {
            for (Cd = 0; Cd < CD; Cd++) {
                NodeCount = AllCores[Xd][Yd][Cd].NodesOnCore;
                Possie=0;
                AllCores[Xd][Yd][Cd].TargetTable=new TargetTableEntry[NodeCount];
                for(Od = 0; Od < NodeCount; Od++){
                    TE = &AllCores[Xd][Yd][Cd].TargetTable[Od];
                    TE->Od = Od;
                    Kd = KeyFrom(Xd, Yd, Cd, Od);
                    node = DEAL.PMAP[Kd]; 
                    V = TASK.NodeList[node].Valency;
                    B = TASK.NodeList[node].Fixed;
                    TE->Fixed = B;
                    TE->Valency = V;
                    TE->Arrived = V;
                    TE->Value = TASK.NodeList[node].Value;
                    TE->Weights = new unsigned[V];
                    TE->VinPoint = new unsigned[V];
                    
                    for(source=0; source < V; source++){
                        InNode = TASK.NodeList[node].Nodes[source];
                        Ks = DEAL.MAP[InNode];
                        TE->Weights[source] = TASK.NodeList[node].Weights[source];  
                        InVal = AllCores[Xd][Yd][Cd].LUT.find(Ks);
                        if(InVal == AllCores[Xd][Yd][Cd].LUT.end()){
                            AllCores[Xd][Yd][Cd].LUT.insert(map<unsigned, unsigned>::value_type(Ks,Possie));
                            TE->VinPoint[source] = Possie;
                            Possie++;
                        } 
                        else {
                            TE->VinPoint[source] = InVal->second;                    
                        }
                        
                    }
                }
                AllCores[Xd][Yd][Cd].ValuesIn = new unsigned[Possie]; 
                AllCores[Xd][Yd][Cd].NoValuesIn = Possie;
            }
        }
    }
}


void Core::PrintLUT(int XD, int YD, int CD){
    int Xd, Yd, Cd;
    map<unsigned, unsigned>::iterator InVal;
    puts("Maps of positions and keys for each core");
    for (Xd=0; Xd < XD; Xd++) {
        for (Yd=0; Yd < YD; Yd++) {
            for (Cd = 0; Cd < CD; Cd++) {
                if(AllCores[Xd][Yd][Cd].LUT.size() == 0) continue;
                printf("Possies for (%d, %d, %d)\n",Xd,Yd,Cd);
                for(InVal = AllCores[Xd][Yd][Cd].LUT.begin();InVal != AllCores[Xd][Yd][Cd].LUT.end(); InVal++){
                    printf("Ks = %x, VinP = %d\n",InVal->first,InVal->second);
                } 
            }
        }
    }
    
}


void Core::PrintValues(Dealer DEAL, Task TASK){
    unsigned node, Xd, Yd, Cd, Od, Kd,Value,NodeCount,O,anode;
    bool found = false;
    puts("Node Key Value");
    for(node=0;node<TASK.Nodes;node++){
        anode = DEAL.RAN[node];
        Kd = DEAL.MAP[anode];
        KeyTo(Kd, Xd, Yd, Cd, Od);
        found = false;
        NodeCount = AllCores[Xd][Yd][Cd].NodesOnCore;
        for(O=0;!found && O<NodeCount;O++){
            if(Od == AllCores[Xd][Yd][Cd].TargetTable[O].Od){
                Value =  AllCores[Xd][Yd][Cd].TargetTable[O].Value;
                found = true;
            }
        }
        if(found)
            printf("%u %x(X=%d,Y=%d,C=%d,O=%d) %u\n", anode, Kd,Xd,Yd,Cd,Od, Value);
        else 
            printf("%u %x(X=%d,Y=%d,C=%d,O=%d)\n", anode, Kd,Xd,Yd,Cd,Od);
    }
}

int Core::UsedCores(int X, int Y, int CD){
    int C, Used=0;
    for(C=0; C<CD; C++){
        if(AllCores[X][Y][C].NodesOnCore != 0)
            Used++;
    }
    return Used;
}

int Core::UsedChips(int XD, int YD, int CD){
    int C, Used=0, NoUsedCores;
    int X,Y;
    for(X=0;X<XD;X++){
        for(Y=0;Y<YD;Y++){
            NoUsedCores = 0;
            for(C=0; C<CD; C++){
                NoUsedCores += UsedCores(X, Y, C);
            }
            if(NoUsedCores)
                Used++;
        }
    }
    return Used;
}


static uint32_t Here = 0;
uint32_t Core::MyWalloc(int words, bool Reset){
    
    uint32_t Start;
    if(Reset)
        Here = 0;
    Start = Here;
    Here += (uint32_t)words;
    return Start;
}

uint32_t* Core::FillCoreDump(int X, int Y, int C, uint32_t& WordCount, uint32_t *Dump ){
    CoreData ThisCore = AllCores[X][Y][C];
    TargetTableEntry *TE = ThisCore.TargetTable;
    map<unsigned, unsigned>::iterator iLUT;
    int  p, Gpoints,w;
    uint32_t Place;
    struct _CoreCommon{
        uint32_t pGpoint;
        uint32_t ValueCount;
        uint32_t ValueStart;
        uint32_t LUTCount;
        uint32_t LUTStart;
    } ;
    struct _Gpoint {
        uint32_t Kd;
        uint32_t Fixed;
        uint32_t Valency;
        uint32_t Arrived;
        uint32_t Value;
        uint32_t pWeights;
        uint32_t pVinPoint;
        uint32_t pNextGpoint;
    };
    WordCount = MyWalloc(0, true);
    Place = MyWalloc(sizeof(_CoreCommon)>>2, false);
    Place = MyWalloc(ThisCore.NoValuesIn, false);
    Place = MyWalloc(2*ThisCore.LUT.size(),false);
    Gpoints = ThisCore.NodesOnCore;
    for(p = 0; p < Gpoints; p++){
        Place = MyWalloc(sizeof(_Gpoint)>>2,false);
        Place = MyWalloc(TE[p].Valency, false);    //room for weights
        Place = MyWalloc(TE[p].Valency, false);    //room for VinPoints        
    }
    WordCount = MyWalloc(0,false);
    free(Dump);
    //Dump = (uint32_t*)calloc(WordCount, sizeof(uint32_t));
    Dump = new uint32_t[WordCount*sizeof(uint32_t)];
    Place = MyWalloc(0, true);
    Place = MyWalloc(sizeof(_CoreCommon)>>2, false);
    _CoreCommon *CoreCommon = (_CoreCommon*)&Dump[Place];
    CoreCommon->pGpoint = 0;        //must fill in when can
    CoreCommon->ValueCount = ThisCore.NoValuesIn;    
    Place = MyWalloc(ThisCore.NoValuesIn, false);
    CoreCommon->ValueStart = Place;
    for(p=0;p<ThisCore.NoValuesIn; p++)
        Dump[p+Place] = ThisCore.ValuesIn[p];
    CoreCommon->LUTCount = ThisCore.LUT.size();
    Place = MyWalloc(2*ThisCore.LUT.size(),false);
    CoreCommon->LUTStart = Place;
    p = 0;
    for(iLUT = ThisCore.LUT.begin(); iLUT != ThisCore.LUT.end();iLUT++,p+=2){
        Dump[Place+p] = iLUT->first;
        Dump[Place+p+1] = iLUT->second;
    }
    uint32_t *Ghead = &CoreCommon->pGpoint;
    for(p = 0; p < Gpoints; p++){
        Place = MyWalloc(sizeof(_Gpoint)>>2,false);
        _Gpoint *Gpoint = (_Gpoint*)&Dump[Place];
        *Ghead = Place;
        Ghead = &Gpoint->pNextGpoint;
        *Ghead = 0;
        Gpoint->Kd = KeyFrom(X,Y,C,TE[p].Od);
        Gpoint->Fixed = TE[p].Fixed; 
        Gpoint->Valency = TE[p].Valency;  
        Gpoint->Arrived = TE[p].Arrived;    
        Gpoint->Value = TE[p].Value;           
        Place = MyWalloc(TE[p].Valency, false);    //room for weights
        Gpoint->pWeights = Place;
        for(w=0;w<TE[p].Valency;w++){
            Dump[Place + w] = TE[p].Weights[w];
        }
        Place = MyWalloc(TE[p].Valency, false);    //room for VinPoints 
        Gpoint->pVinPoint = Place;
        for(w=0;w<TE[p].Valency;w++){
            Dump[Place + w] = TE[p].VinPoint[w];
        }        
    }
    
    return Dump;
}
void Core::WriteTC(string ProblemName, int NX, int NY, int CD){
    const char *TCram;
    int Rtype = 0x71;
    int X, Y, C, XY, Tsize, CoreCount;
    uint32_t WordCount, StartAddress = 0x0401000, CoreID, NoUsedChips;;
 
    CoreData XYC;
    FILE *Results;
    uint32_t *CoreDump = NULL;
    TCram = (ProblemName+".SPINNjram").c_str();
    Results = fopen(TCram,"wb");
    if(!Results){
        printf("couldn't open %s file matey\n",TCram);
        return;
    }    
    fwrite(&Rtype,1,1,Results);
    Rtype = 0;
    fwrite(&Rtype,1,3,Results);
    puts("TC RAM sizes");
    NoUsedChips = UsedChips(NX, NY , CD);
    fwrite(&NoUsedChips, 4, 1, Results);
    for(X=0;X<NX; X++){
        for(Y=0;Y<NY;Y++){
            CoreCount = UsedCores(X, Y, CD);
            if( CoreCount == 0)
                continue;
            XY = ((X<<8)+Y)&0x0FFFF;
            fwrite(&XY, 2, 1, Results); 
            fwrite(&CoreCount, 2, 1, Results);
            for (C=0; C<CD; C++) {
                XYC = AllCores[X][Y][C];
                Tsize = XYC.NodesOnCore;
                if(Tsize == 0)
                    continue;
                CoreID = C+1;                           //can't use the monitor
                CoreDump = FillCoreDump(X,Y,C,WordCount, CoreDump);
                printf("Chip&Core (%x, %x, %d) %d Grid Points %d Words\n",X,Y,C,Tsize,WordCount);                
                fwrite(&StartAddress, 4, 1, Results);   //start address
                fwrite(&CoreID, 4, 1, Results);         //CoreID
                fwrite(&WordCount, 4, 1, Results);      //Number of 32 bit words;
                fwrite(CoreDump,4,WordCount,Results);
            }
        }
    }
    fclose(Results);
}