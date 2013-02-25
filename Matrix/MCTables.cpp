//
//  MCTables.cpp
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "MCTables.h"
using namespace std;

MakeMCTables::MakeMCTables(int XD, int YD, int Chips){
    Key = new vector<unsigned>*[XD];
    Mask = new vector<unsigned>*[XD];
    Data = new vector<unsigned>*[XD];
    for(int i = 0; i < XD; i++){
        Key[i] = new vector<unsigned>[YD];
        Mask[i] = new vector<unsigned>[YD];
        Data[i] = new vector<unsigned>[YD];
    }
    NX=XD;
    NY=YD;
    ChipCount = Chips;
    P2P.reserve(ChipCount);
    for(int y = 0; y< ChipCount; y++)
        P2P.push_back(vector<unsigned>(ChipCount));
}

MakeMCTables::~MakeMCTables(){
}
void MakeMCTables::MakeP2P(Machine &MAC){
    
    int Chip;
    int Cp, l;
    int Cn;
    int OutLine;
    unsigned *Visited;
    int *Link, C;
    unsigned *List, EndList;

    Visited = new unsigned[ChipCount];
    Link = new int[ChipCount];
    List = new unsigned[ChipCount];
    for (Chip=0; Chip<ChipCount; Chip++) {
        P2P[Chip][Chip]=-1;
        for(int x = 0; x < ChipCount; x++){
            Visited[x] = 0;
            Link[x] = -1;
        }
        List[0] = Chip;
        Visited[Chip] = 1;
        EndList = 1;
        for(C=0; C<EndList;C++){
            Cp = List[C];
            OutLine = Link[Cp];
            for(l=0; l<6;l++){
                Cn = MAC.ChipLink[Cp][l];
                if(Cn == -1)
                    continue;
                if(!Visited[Cn]){
                    List[EndList++] = Cn;
                    Visited[Cn] = 1;
                    if(OutLine == -1)
                        Link[Cn] = l;
                    else {
                        Link[Cn] = OutLine;
                    }
                    P2P[Chip][Cn] = Link[Cn];
                }
            }
        }
    }
    
    delete[] List;
    delete[] Link;
    delete[] Visited;
}


void MakeMCTables::PutMCLine(unsigned X, unsigned Y, unsigned KEY, unsigned MASK, unsigned DATA){
    bool found = false;
    unsigned NoEntries = (unsigned)Key[X][Y].size();
    for(int i=0; !found && i< NoEntries; i++){
        if((KEY&MASK) == (Key[X][Y][i]&Mask[X][Y][i])){
            Data[X][Y][i] |= DATA;
            found = true;
        }
    }
    if(!found){
        Key[X][Y].push_back(KEY);
        Mask[X][Y].push_back(MASK);
        Data[X][Y].push_back(DATA);
    }
    
}

void MakeMCTables::MCTEnter(int Xd,int Yd,int Core,unsigned KEY,unsigned MASK,unsigned LINK, int LinkOut){
    unsigned DATA;
    bool found;
    found = false;
    DATA = (1<<(24+LinkOut));   //Keep a record for through routing
    if(LINK == -1)
        LINK = 0;
    else {
        LINK = (1<<LINK);
    }
    if(Core == -1)
        DATA |= LINK;
    else{
        DATA |= ((1<<(Core+7))|LINK);   //remember that Core in Key is 0 to 15 and in data is 1 to 16
    }
    PutMCLine(Xd, Yd, KEY, MASK, DATA);
}
void MakeMCTables::SetRoute(int Xs, int Ys, int Xd, int Yd,int Core, unsigned KEY, unsigned MASK, int LinkOut, Machine &MAC){
    int dX,dY, OutLink;
    int  Cs, Cd,Cn,  XYn, Xn, Yn;
    dX = (Xs-Xd);
    dY = (Ys-Yd);
    if(dX==0 && dY ==0) {//arrive
        int LINK = -1;
        MCTEnter(Xd, Yd, Core, KEY, MASK, LINK, LinkOut);
        return;
    }
    Cs = MAC.XYChip[(Xs<<8)|Ys];
    Cd = MAC.XYChip[(Xd<<8)|Yd];
    OutLink = P2P[Cs][Cd];
    Cn = MAC.ChipLink[Cs][OutLink];
    XYn = MAC.ChipXY[Cn];
    Xn = XYn>>8;
    Yn = XYn&0xff;
    MCTEnter(Xs, Ys,-1, KEY, MASK, OutLink, LinkOut);
    SetRoute(Xn, Yn, Xd, Yd, Core, KEY, MASK, OutLink,MAC);
}

void MakeMCTables::FillTables(Task &TASK, Machine &MAC){
    unsigned Xd,Yd,Xs,Ys, Cs,Cd,Os,Od;
    unsigned SKey,DKey;
    //multimap<unsigned, unsigned>::iterator StoD;
    puts("Filling MC tables") ;
    for(auto StoD = TASK.KsKd.cbegin(); StoD != TASK.KsKd.cend();StoD++){
        SKey = StoD->first;
        DKey = StoD->second;
        KeyTo(SKey,Xs,Ys, Cs, Os);
        KeyTo(DKey,Xd,Yd, Cd, Od);
        SetRoute(Xs, Ys, Xd, Yd, Cd,SKey , ALLMASK,6,MAC);
    }
}

void MakeMCTables::PrintTableSizes(int XD, int YD){
    int X,Y;
    printf("MC Table Sizes\n");
    for(X=0;X<XD;X++){
        for(Y=0;Y<YD;Y++){
            printf("%lu-",Key[X][Y].size());
        }
        puts("");
    }
}
void MakeMCTables::PrintTables(int X, int Y){
    int i;
    printf("MC Table Entries for (X=%d, y=%d)\nKey\tMask\tData\n",X,Y);
    for(i=0;i<Key[X][Y].size();i++){
        printf("%x\t%x\t%x\n",Key[X][Y][i],Mask[X][Y][i],Data[X][Y][i]);
    }
    puts("");
}


void MakeMCTables::PrintTTableSizes(int XD, int YD){
    int X,Y;
    printf("MC Table Sizes\n");
    for(X=0;X<XD;X++){
        for(Y=0;Y<YD;Y++){
            printf("%lu-",TKey[X][Y].size());
        }
        puts("");
    }
}

void MakeMCTables::PrintTTables(int X, int Y){
    int i;
    printf("MC Table Entries for (X=%d, y=%d)\nKey\tMask\tData\n",X,Y);
    for(i=0;i<TKey[X][Y].size();i++){
        printf("%x\t%x\t%x\n",TKey[X][Y][i],TMask[X][Y][i],TData[X][Y][i]);
    }
    puts("");
}

void MakeMCTables::TreeRouteIn(unsigned Cs,unsigned Cd, Machine &MAC){
    unsigned X,Y,XY, link;
    unsigned DATA;
    XY = MAC.ChipXY[Cs];
    X = XY>>8;
    Y = XY&0xff;
    if(Cs != Cd){
        link = P2P[Cs][Cd];          //these are going to (0,0,1)
        DATA = 1<<link;
        PutMCLine(X, Y, STOPKEY, ALLMASK, DATA);
        PutMCLine(X, Y, REPORTKEY, ALLMASK, DATA);
        Cs = MAC.ChipLink[Cs][link];
        TreeRouteIn(Cs, Cd, MAC);   //keep going
    }
    else {                          //arrived
        DATA = (1<<7);              //handled by Core 1 on the Destination Chip
        PutMCLine(X, Y, STOPKEY, ALLMASK, DATA);
        PutMCLine(X, Y, REPORTKEY, ALLMASK, DATA);
    }
    
}

void MakeMCTables::TreeRouteOut(unsigned Cs,unsigned Cd, Machine &MAC, Dealer &DEAL){
    unsigned X,Y,XY, link;
    unsigned DATA;
    XY = MAC.ChipXY[Cs];
    X = XY>>8;
    Y = XY&0xff;
    if(Cs != Cd){
        link = P2P[Cs][Cd];          //these are going to (0,0,1)
        DATA = (1<<link);
        PutMCLine(X, Y, STARTKEY, ALLMASK, DATA);
        Cs = MAC.ChipLink[Cs][link];
        TreeRouteOut(Cs, Cd, MAC, DEAL);   //keep going
    }
    else {                          //arrived
        DATA = 0;              //handled by active Cores on the Destination Chip
        for(int i = 0; i <= DEAL.MaxCoreNo[Cs]; i++)
            DATA = DATA | (1<<(i+7));
        PutMCLine(X, Y, STARTKEY, ALLMASK, DATA);
    }
    
}

void MakeMCTables::FANIO(Machine &MAC, Dealer &DEAL ){
    
    for(unsigned Chip = 0; Chip < ChipCount; Chip++){
        if(DEAL.MaxCoreNo[Chip]<0) continue;
        TreeRouteIn(Chip,0,MAC); //route from anywhere to chip 0,0 core 1
        TreeRouteOut(0, Chip, MAC, DEAL); //route from 0,0 to every other chip
    }
}

void MakeMCTables::ThroughRoute(int XD, int YD, Machine &MAC){
    unsigned X,Y;
    unsigned DATA, LinkOut,LinkIn, Cores;
    vector<unsigned>    V;
    //vector<unsigned>::iterator K,M,D;
    TKey = new vector<unsigned>*[XD];
    TMask = new vector<unsigned>*[XD];
    TData = new vector<unsigned>*[XD];
    for(int i = 0; i < XD; i++){
        TKey[i] = new vector<unsigned>[YD];
        TMask[i] = new vector<unsigned>[YD];
        TData[i] = new vector<unsigned>[YD];
    }
    
    for(X=0;X<XD;X++){
        for(Y=0;Y<YD;Y++){
            V = Data[X][Y];
            for(auto D = V.cbegin(),K = Key[X][Y].cbegin(), M = Mask[X][Y].cbegin(); D != V.cend(); D++,K++,M++){
                DATA = *D;
                LinkOut = DATA&0x3f;
                LinkIn = (DATA >> 24)&0x03f;
                DATA = DATA&0x0ffffff;
                Cores = DATA >> 6;
                if((LinkIn == LinkOut) && (Cores == 0)) continue;
                TKey[X][Y].push_back(*K);
                TMask[X][Y].push_back(*M);
                TData[X][Y].push_back(DATA);
            }
            
        }
    }
}
unsigned MakeMCTables::GetMCData(unsigned Ks, unsigned Kd){//avec through routing
    //Ks is the source key Kd is the core I'm on.
    unsigned Xd, Yd, Cd, Od;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    for(int e = 0; e < TKey[Xd][Yd].size(); e++){
        if ((TKey[Xd][Yd][e]&TMask[Xd][Yd][e]) == (Ks&TMask[Xd][Yd][e])) {
            return TData[Xd][Yd][e];
        }
    }
    return 0;   //0 indicates no Key match - Through routed
}
int MakeMCTables::GetTableCount(){
    int X,Y,Z=0;
    for(X=0;X<NX;X++){
        for (Y=0; Y<NY; Y++) {
            if(Key[X][Y].size() != 0)
                Z++;
        }
    }
    return Z;
}
void MakeMCTables::WriteMC(string ProblemName){
    const char *MCtables;
    FILE *Results;
    int Rtype = 0x7a;
    int Size, X, Y, XY, Tsize,t ;
    MCtables = (ProblemName+".SPINNmcta").c_str();
    Results = fopen(MCtables,"wb");
    if(!Results){
        printf("couldn't open %s file matey\n",MCtables);
        return;
    }
    fwrite(&Rtype,1,1,Results);
    Size = GetTableCount();
    fwrite(&Size,2,1,Results);
    puts("MC table sizes");
    for(X=0;X<NX;X++){
        for(Y=0;Y<NY;Y++){
            Tsize = (int)TKey[X][Y].size();
            printf("Chip(%x,%x) %d\n",X,Y,Tsize);
            if(Tsize == 0) continue;
            XY = ((X<<8)+Y)&0x0FFFF;
            fwrite(&XY,2,1,Results);
            fwrite(&Tsize,2,1,Results);
            for(t=0;t<Tsize;t++){
                fwrite(&TKey[X][Y][t], 4, 1, Results);
                fwrite(&TMask[X][Y][t], 4, 1, Results);
                fwrite(&TData[X][Y][t], 4, 1, Results);
            }
        }
    }
    fclose(Results);
}
