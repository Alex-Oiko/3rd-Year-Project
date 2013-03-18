//
//  Machine.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "Machine.h"
#include "Utils.h"
Machine::Machine(int MachineType,int XD, int YD, int MachineSize){
    int Chip, X, Y, l;
    ChipCount = MachineSize;
    NX = XD;
    NY = YD;
    
    
    
    if(MachineType == 0 || MachineType == 1){ //open rectangle or torus
        for(Chip = 0; Chip < ChipCount; Chip++){
            RevertXY(Chip, X, Y);
            ChipXY.push_back((X<<8) | Y);
            XYChip.insert(pair<unsigned, unsigned>((X<<8) | Y,Chip));
        }
        
    }
    if(MachineType == 2){               //Board48
        for(Chip = 0; Chip < ChipCount; Chip++){
            B48Revert(Chip, X, Y);
            ChipXY.push_back( (X<<8) | Y);
            XYChip.insert(pair<unsigned, unsigned>((X<<8) | Y,Chip));
        }
    }
    
    
    for(Chip = 0; Chip < ChipCount; Chip++){
        X = ChipXY[Chip]>>8;
        Y = ChipXY[Chip]&0xff;
        vector<unsigned> CON;
        for(l=0; l<6; l++){
            if(MachineType == 0)
                CON.push_back(Neighbour(X, Y, l));
            else if(MachineType == 1)
                CON.push_back(NeighbourWrap(X, Y, l));
            else if (MachineType == 2)
                CON.push_back(UNeighbour(X, Y, l));
        }
        ChipLink.push_back(CON);
    }
}
Machine::~Machine(){
    /*
     delete[] ChipXY;
     for(int Chip = 0; Chip < ChipCount; Chip++){
     delete[] ChipLink[Chip];
     }
     delete[] ChipLink;
     */
}
int Machine::ConvertXY(const int X,const int Y) const{
    return X*NY + Y;
}

void Machine::RevertXY(const int XY,int &X,int &Y) const{
    X = XY/NY;
    Y = XY%NY;
}
void Machine::B48Revert(const int Chip, int& X, int& Y) const{
    int XA[48] = {0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7};
    int YA[48] = {0,1,2,3,0,1,2,3,4,0,1,2,3,4,5,0,1,2,3,4,5,6,0,1,2,3,4,5,6,7,1,2,3,4,5,6,7,2,3,4,5,6,7,3,4,5,6,7};
    X = XA[Chip];
    Y = YA[Chip];
    return;
}

int Machine::NeighbourWrap(const int X,const int Y,const  int l)const {
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

int Machine::Neighbour(const int X,const  int Y,const int l) const{
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

int Machine::UNeighbour(const int X,const int Y,const int l)const {
    int Xn=X, Yn=Y;
    //map<unsigned, unsigned>::iterator iT;
    switch (l) {
        case 0:
            Xn = (X+1);
            break;
            
        case 1:
            Xn = (X+1); Yn = (Y+1);
            break;
            
        case 2:
            Yn = (Y+1);
            break;
            
        case 3:
            Xn = (X-1);
            break;
            
        case 4:
            Xn = (X-1);Yn = (Y-1);
            break;
            
        case 5:
            Yn = (Y-1);
            break;
            
        default:
            break;
    }
    auto iT = XYChip.find((Xn<<8) | Yn);
    if(iT == XYChip.end())
        return -1;
    return iT->second;
}



