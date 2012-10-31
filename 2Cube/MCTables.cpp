//
//  MCTables.cpp
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#include <iostream>
#include <vector>
#include "MCTables.h"

using namespace std;
MakeMCTables::MakeMCTables(int XD, int YD){
    Key = new vector<unsigned>*[XD];
    Mask = new vector<unsigned>*[XD];
    Data = new vector<unsigned>*[XD]; 
    for(int i = 0; i < XD; i++){
        Key[i] = new vector<unsigned>[YD];
        Mask[i] = new vector<unsigned>[YD];
        Data[i] = new vector<unsigned>[YD];        
    }
    NX = XD;
    NY = YD;
}
int MakeMCTables::ConvertXY(int X, int Y){
    return X*NY + Y;
}

void MakeMCTables::RevertXY(int XY, int &X, int &Y){
    X = XY/NY;
    Y = XY%NY;
}
int MakeMCTables::GetLink(int Xf, int Yf, int Xt, int Yt){
    return P2P[ConvertXY(Xf, Yf)][ConvertXY(Xt, Yt)];
}


int MakeMCTables::NeighbourWrap(int X, int Y, int l){
    int Xn=X, Yn=Y;
    switch (l) {
        case 0:
            Xn = (X+1)%NX;
            break;
            
        case 1:
            Xn = (X+1)%NX; Yn = (Y+1)%NY;
            break;
            
        case 2:
            Yn = (Y+1)%NY;
            break;
            
        case 3:
            Xn = (X-1+NX)%NX;
            break;            
            
        case 4:
            Xn = (X-1+NX)%NX;Yn = (Y-1+NY)%NY;
            break;
            
        case 5:
            Yn = (Y-1+NY)%NY;
            break;
            
        default:
            break;
    }
    return ConvertXY(Xn, Yn);
}

int MakeMCTables::Neighbour(int X, int Y, int l){
    int Xn=X, Yn=Y, found = 0;
    switch (l) {
        case 0:
            if(X<NX-1)
                Xn = (X+1);
            else {
                found = -1;
            }
            break;
            
        case 1:
            if(X<NX-1 &&Y<NY-1){
                Xn = (X+1); Yn = (Y+1);
            }
            else {
                found = -1;
            }            
            break;
            
        case 2:
            if(Y<NY-1)
                Yn = (Y+1);
            else {
                found = -1;
            }            
            break;
            
        case 3:
            if(X>0)
                Xn = (X-1);
            else {
                found = -1;
            }            
            break;            
            
        case 4:
            if(X>0 && Y>0){
                Xn = (X-1);Yn = (Y-1);
            }
            else {
                found = -1;
            }           
            break;
            
        case 5:
            if(Y>0)
                Yn = (Y-1);
            else {
                found = -1;
            }            
            break;
            
        default:
            break;
    }
    if(found == 0)
        found = ConvertXY(Xn,Yn);
    return found;
}
void MakeMCTables::MakeP2P(int XD, int YD, bool Period){
    int NXY,X,Y,XY;
    int XYp, Xp, Yp, l;
    int XYn;
    int OutLine;
    unsigned *Visited;
    int *Link, C;
    unsigned *List, EndList;
    NXY = NX*NY;
    Wrap = Period;
    Visited = new unsigned[NXY];
    Link = new int[NXY];
    List = new unsigned[NXY];
    P2P = new unsigned*[NXY];
    for(int x = 0; x < NXY; x++){
        P2P[x] = new unsigned[NXY];
        Visited[x] = 0;
    }  
    for(X=0;X<NX;X++){
        for (Y=0; Y<NY; Y++) {
            for(int x = 0; x < NXY; x++){
                Visited[x] = 0;
                Link[x] = -1;
            }  
            XY = ConvertXY(X, Y);
            List[0] = XY;
            Visited[XY] = 1;
            EndList = 1;
            for(C=0; C<EndList;C++){
                XYp = List[C];
                OutLine = Link[XYp];
                RevertXY(XYp, Xp, Yp);
                for(l=0; l<6;l++){
                    if(Wrap)
                        XYn = NeighbourWrap(Xp, Yp, l);
                    else 
                        XYn = Neighbour(Xp, Yp, l);
                    if(XYn == -1)
                        continue;
                    if(!Visited[XYn]){
                        List[EndList++] = XYn;
                        Visited[XYn] = 1;
                        if(OutLine == -1)
                            Link[XYn] = l;
                        else {
                            Link[XYn] = OutLine;
                        }
                        P2P[XY][XYn] = Link[XYn];
                    }
                }
            }
        }
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
    int NoEntries = Key[Xd][Yd].size();
    for(int i=0; !found && i< NoEntries; i++){
        if((KEY&MASK) == (Key[Xd][Yd][i]&Mask[Xd][Yd][i])){
            Data[Xd][Yd][i] |= DATA;
            found = true;
        }
    }
    if(!found){
        Key[Xd][Yd].push_back(KEY);
        Mask[Xd][Yd].push_back(MASK);
        Data[Xd][Yd].push_back(DATA);        
    }
}
void MakeMCTables::SetRoute(int Xs, int Ys, int Xd, int Yd,int Core, unsigned KEY, unsigned MASK, int LinkOut){
    int dX,dY, OutLink; 
    int LINK, XYs, XYd, XYn, Xn, Yn;
    dX = (Xs-Xd);
    dY = (Ys-Yd);
    if(dX==0 && dY ==0) {//arrive
        LINK=-1;
        MCTEnter(Xd, Yd, Core, KEY, MASK, LINK, LinkOut);
        return;
    }
    XYs = ConvertXY(Xs, Ys);
    XYd = ConvertXY(Xd, Yd);  
    OutLink = P2P[XYs][XYd];
    if(Wrap)
        XYn = NeighbourWrap(Xs, Ys, OutLink);
    else
        XYn = Neighbour(Xs, Ys, OutLink);
    RevertXY(XYn, Xn, Yn);
    MCTEnter(Xs, Ys,-1, KEY, MASK, OutLink, LinkOut);
    SetRoute(Xn, Yn, Xd, Yd, Core, KEY, MASK, OutLink);
}

void MakeMCTables::FillTables(Task TASK){
    unsigned Xd,Yd,Xs,Ys, Cs,Cd,Os,Od;
    unsigned SKey,DKey;
    multimap<unsigned, unsigned>::iterator StoD;
    puts("Filling MC tables");
    for(StoD = TASK.KsKd.begin(); StoD != TASK.KsKd.end();StoD++){
        SKey = StoD->first;
        DKey = StoD->second;
        KeyTo(SKey,Xs,Ys, Cs, Os);
        KeyTo(DKey,Xd,Yd, Cd, Od);       
        SetRoute(Xs, Ys, Xd, Yd, Cd,SKey , 0xfffff800,6);        
    }
}

void MakeMCTables::PrintCommsTables(Task TASK, unsigned *MAP,unsigned XD, unsigned YD){
	int i,j,V;
    unsigned site,Cs,Os,Cd,Od;
    unsigned Xd,Yd,Xs,Ys,Ts,Td,XYD = XD*YD;
    int **Table;
    Task::Node *Next;
    Table = new int*[XYD];
    for(i=0;i<XYD;i++){
        Table[i] = new int[XYD];
        for(j=0;j<XYD;j++)
            Table[i][j] = 0;
    }
	for(i=0;i<TASK.Nodes;i++){
        KeyTo(MAP[i],Xs,Ys,Cs,Os);
        Ts = YD*Xs+Ys;
        Next = &TASK.NodeList[i];
	    V = Next->Valency;
	    for(j=0;j<V;j++){
            site = Next->Nodes[j];
            KeyTo(MAP[site],Xd,Yd,Cd,Od);       
            Td = YD*Xd+Yd;
            Table[Ts][Td]++;
	    }
	}
    
    printf("Inter Chip communications\n");
    for(i=0;i<XYD;i++){
        for(j=0;j<XYD;j++){
            printf("%d\t",Table[i][j]);
        }
        puts("");
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


void MakeMCTables::ThroughRoute(int XD, int YD){
    int X,Y;
    unsigned DATA, LinkOut,LinkIn, Cores;
    vector<unsigned>    V;
    vector<unsigned>::iterator K,M,D;
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
            for(D = V.begin(),K = Key[X][Y].begin(), M = Mask[X][Y].begin(); D != V.end(); D++,K++,M++){
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
    unsigned MASK = 0xfffff800;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    for(int e = 0; e < TKey[Xd][Yd].size(); e++){
        if ((TKey[Xd][Yd][e]&TMask[Xd][Yd][e]) == (Ks&MASK)) {
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
            Tsize = TKey[X][Y].size();
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
