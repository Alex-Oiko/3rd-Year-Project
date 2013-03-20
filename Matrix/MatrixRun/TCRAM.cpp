//
//  TCRAM.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//
#include <iostream>
#include <sys/stat.h>
#include "TCRAM.h"
#include "Utils.h"
TCram::TCram(uint32_t XD, uint32_t YD, uint32_t CD){
    TCData = new uint32_t***[XD];
    NX = XD;
    NY = YD;
    NC = CD;
    for(int X = 0; X < XD; X++){
        TCData[X] = new uint32_t**[YD];
        for(int Y = 0; Y< YD;Y++){
            TCData[X][Y] = new uint32_t*[CD];
            for(int C = 0; C < CD; C++){
                TCData[X][Y][C] = NULL;
            }
        }
    }
    
}

/*
uint32_t TCram::GetValue(uint32_t Ks){
    unsigned Xs, Ys, Cs, Os;
    uint32_t Value = 0;
    uint32_t *CoreData;
    TCram::_CoreCommon *CoreCommon;
    TCram::_Gpoint *Gpoint;
    uint32_t GPindex;
    KeyTo(Ks, Xs, Ys, Cs, Os);
    CoreData = TCData[Xs][Ys][Cs];
    CoreCommon = (_CoreCommon*)CoreData;
    GPindex = CoreCommon->pGpoint;
    while (GPindex) {
        Gpoint = (_Gpoint*)&CoreData[GPindex];
        if(Ks == Gpoint->Kd){
            Value = Gpoint->Value;
            break;
        }
        GPindex = Gpoint->pNextGpoint;
    }
    return Value;
}


void TCram::GetAllKeys(vector<uint32_t>& ValidKeys){
    uint32_t *CoreData;
    _CoreCommon *CoreCommon;
    uint32_t GPindex;
    _Gpoint *Gpoint;
    for(int X = 0; X< NX;X++){
        for(int Y = 0; Y < NY; Y++) {
            for (int C = 0; C < NC; C++) {
                CoreData = TCData[X][Y][C];
                if (!CoreData)
                    continue;
                CoreCommon = (_CoreCommon*)CoreData;
                GPindex = CoreCommon->pGpoint;
                while (GPindex) {
                    Gpoint = (_Gpoint*)&CoreData[GPindex];
                    ValidKeys.push_back(Gpoint->Kd);
                    GPindex = Gpoint->pNextGpoint;
                }
            }
        }
    }
}
*/
/*
uint32_t TCram::GetNewValue(_Gpoint* Gpoint, uint32_t* CoreData){
    uint32_t Wplace = Gpoint->pWeights, Vplace = Gpoint->pVinPoint, Count = Gpoint->Valency;
    _CoreCommon *CoreCommon = (_CoreCommon*)CoreData;
    uint32_t *CoreValues = &CoreData[CoreCommon->ValueStart];
    long long Wt,Vt, WVt;
    if(Gpoint->Fixed)
        return Gpoint->Value;
    WVt = 0;
    for(int node = 0; node < Count; node++){
        Wt = (long long)CoreData[node+Wplace];
        Vt = (long long)CoreValues[CoreData[Vplace+node]];
        WVt += Wt*Vt;
    }
    return (uint32_t)(WVt>>31);
}

uint32_t TCram::Update(uint32_t Kd){
    uint32_t Xd, Yd, Cd, Od;
    uint32_t Value = 0;
    uint32_t *CoreData;
    
    _CoreCommon *CoreCommon;
    _Gpoint *Gpoint;
    uint32_t GPindex;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    CoreData = TCData[Xd][Yd][Cd];
    CoreCommon = (_CoreCommon*)CoreData;
    GPindex = CoreCommon->pGpoint;
    while (GPindex) {
        Gpoint = (_Gpoint*)&CoreData[GPindex];
        if(Kd == Gpoint->Kd){
            Value = GetNewValue(Gpoint, CoreData);
            if(Gpoint->Value == Value)
                printf("%u %u %u %u Value = %u\n",Xd,Yd,Cd,Od,Value);
            Gpoint->Value = Value;
            Gpoint->Arrived = 0;
            break;
        }
        GPindex = Gpoint->pNextGpoint;
    }
    return Value;
    
}

*/



bool TCram::ReadData(string TCDataFile){
    uint32_t Rtype = 0, XY = 0, X = 0, Y = 0, Chip, ChipCount = 0, CoreID = 0, StartAddress = 0,CoreCount = 0, WordCount = 0, C;
    struct stat stat_record;
    if(stat(TCDataFile.c_str(), &stat_record)){
        cerr << "Can't find TCRAM table file " << TCDataFile <<"\n";
        return false;
    }
    if(!stat_record.st_size){
        cerr << "Nothing in TCRAM file " << TCDataFile <<"\n";
        return false;
    }

    FILE *TCFile;
    TCFile = fopen(TCDataFile.c_str(), "rb");
    if(!TCFile){
        cout << "Couldn't open " + TCDataFile + " file matey\n";
        return false;
    }
    fread(&Rtype, 1, 1, TCFile);
    if(Rtype != 0x71){
        cout <<TCDataFile + " is the wrong file type matey\n";
        return false;
    }
    fread(&Rtype,1,3,TCFile);
    fread(&ChipCount,4,1,TCFile);
    for(Chip = 0; Chip < ChipCount; Chip++) {
        fread(&XY,2,1,TCFile);
        X = XY >> 8;
        Y = XY&0x0FF;
        fread(&CoreCount, 2, 1, TCFile);
        for(C=0;C<CoreCount;C++) {
            fread(&StartAddress, 4, 1, TCFile);
            fread(&CoreID, 4, 1, TCFile);
            fread(&WordCount, 4, 1, TCFile);
            TCData[X][Y][CoreID-1] = new uint32_t[WordCount];
            fread(TCData[X][Y][CoreID-1], 4, WordCount, TCFile);
        }
    }
    fclose(TCFile);
    return true;
}

void TCram::PrintTCram(){
    uint32_t *CoreData;
    _CoreCommon *CoreCommon;
    _LookUp *LUT;
    uint32_t PointCount;
    _DTTE *TTE;
    float *Values;
    for(int X = 0; X< NX;X++){
        for(int Y = 0; Y < NY; Y++) {
            for (int C = 0; C < NC; C++) {
                CoreData = TCData[X][Y][C];
                if (!CoreData)
                    continue;
                printf("TCData for (X=%x, Y=%x, C=%d)\n",X,Y,C);
                CoreCommon = (_CoreCommon*)CoreData;
                printf("LUTStart = %d, LUTCount = %d\nTTStart = %d, ValuesStart = %d\n",CoreCommon->LUTStart,CoreCommon->LUTCount, CoreCommon->TTStart, CoreCommon->ValuesStart);
                PointCount = CoreCommon->PointCount;
                puts("LUT");
                LUT = (_LookUp*)&CoreData[CoreCommon->LUTStart];
                for(int i = 0; i < CoreCommon->LUTCount;i++)
                    printf("%x,%x  ",LUT[i].Ks,LUT[i].idx);
                puts("");
                TTE = (_DTTE*)&CoreData[CoreCommon->TTStart];
                Values = (float*)&CoreData[CoreCommon->ValuesStart];
                for(int i = 0; i < PointCount; i++){
                    printf("Kd = %x, OpCode = %d, IV = %f, Value = %f, Expected = %d, Arrived = %d\n",TTE[i].Kd,TTE[i].OpCode, TTE[i].IV,Values[TTE[i].oV], TTE[i].Expected, TTE[i].Arrived);
                }
            }
        }
    }
}
