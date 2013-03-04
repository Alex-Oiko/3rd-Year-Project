//
//  MCTables.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include <iostream>
#include <sys/stat.h>
#include "MCTables.h"
using namespace std;
MCLines::MCLines(uint32_t XD, uint32_t YD){
    Key = new vector<uint32_t>*[XD];
    Mask = new vector<uint32_t>*[XD];
    Data = new vector<uint32_t>*[XD];
    for(uint32_t i = 0; i < XD; i++){
        Key[i] = new vector<uint32_t>[YD];
        Mask[i] = new vector<uint32_t>[YD];
        Data[i] = new vector<uint32_t>[YD];
    }
    NX = XD;
    NY = YD;
}
uint32_t MCLines::GetMCData(uint32_t Ks, uint32_t Kd){//avec through routing
    //Ks is the source key Kd is the core I'm on.
    uint32_t Xd, Yd, Cd, Od;
    uint32_t MASK = 0xfffff800;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    for(uint32_t e = 0; e < Key[Xd][Yd].size(); e++){
        if ((Key[Xd][Yd][e]&Mask[Xd][Yd][e]) == (Ks&MASK)) {
            return Data[Xd][Yd][e];
        }
    }
    return 0;   //0 indicates no Key match - Through routed
}

bool MCLines::ReadData(string MCfile){
    uint32_t Rtype = 0, XY = 0, X = 0, Y = 0, Chip, ChipCount = 0, TableSize=0,C, KEY=0,MASK=0,DATA=0;
    struct stat stat_record;
    if(stat(MCfile.c_str(), &stat_record)){
        cerr << "Can't find MC table file " << MCfile <<"\n";
        return false;
    }
    if(!stat_record.st_size){
        cerr << "Nothing in MC table file " << MCfile <<"\n";
        return false;
    }

    FILE *MCFile;
    MCFile = fopen(MCfile.c_str(), "rb");
    if(!MCFile){
        cout << "Couldn't open " + MCfile + " file matey\n";
        return false;
    }
    fread(&Rtype, 1, 1, MCFile);
    if(Rtype != 0x7a){
        cout <<MCfile + " is the wrong file type matey\n";
        return false;
    }
    fread(&ChipCount,2,1,MCFile);
    for(Chip = 0; Chip < ChipCount; Chip++) {
        fread(&XY,2,1,MCFile);
        X = XY >> 8;
        Y = XY&0x0FF;
        fread(&TableSize, 2, 1, MCFile);
        for(C=0;C<TableSize;C++) {
            fread(&KEY, 4, 1, MCFile);
            Key[X][Y].push_back(KEY);
            fread(&MASK, 4, 1, MCFile);
            Mask[X][Y].push_back(MASK);
            fread(&DATA, 4, 1, MCFile);
            Data[X][Y].push_back(DATA);
        }
    }
    return true;
}

void MCLines::PrintMCT(){
    uint32_t X,Y,Z,z;
    puts("MC tables");
    for(X=0;X<NX;X++){
        for(Y=0;Y<NY;Y++){
            Z = (uint32_t)Key[X][Y].size();
            printf("X=%x, Y=%x, Size=%d\n",X,Y,Z);
            for(z=0;z<Z; z++){
                printf("Key = %x, Mask = %x Data = %x\n",Key[X][Y][z],Mask[X][Y][z], Data[X][Y][z]);
            }
        }
    }
}