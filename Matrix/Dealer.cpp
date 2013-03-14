//
//  Dealer.cpp
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "Dealer.h"
#include <iostream>
#include <stdlib.h>
#define MaxOff 2048

//max cores per chip
//The algorithm spreads Nodes on cores as thinly as possible
//XD, YD define the extent of the machine grid
//Nodes is the number of nodes (points) in the problem
//Toss - random distribution or not
//seed - for random number generator
//CORE - here to record the number of nodes per core



Dealer::Dealer(const unsigned NoChips,const unsigned Cores){
    vector<unsigned> ChipOff;
    MaxChips = NoChips;
    MaxCores = Cores;
    MaxOffsets = MaxOff;
    NextChip = -1;
    NextCore = 0;
    for(int c = 0; c < MaxCores;c++)
        ChipOff.push_back(-1);
    for(int Chip = 0; Chip < MaxChips; Chip++)
        Offset.push_back(ChipOff);
}



//Updates the values of NextChip, NextCore and Offset[NextChip][NextCore]
//in that order
bool Dealer::StartingPlace(unsigned size){
    NextChip++;
    if (NextChip == MaxChips) {
        NextChip = 0;
        NextCore++;
        if (NextCore == MaxCores) {
            NextCore = 0;
        }
    }
    Offset[NextChip][NextCore] = Offset[NextChip][NextCore]+1;
    if(Offset[NextChip][NextCore]+size >= MaxOffsets){
        return false;
    }
    
 
    return true;
}

void Dealer::Deal(Machine& MAC, Task& TASK, bool Toss, int seed, Core& CORE){
    int X,Y,n, XY;
    unsigned Nodes = TASK.nextPID;
    unsigned K;
    //map<string,unsigned>::iterator iVPID;   //iterate through the list of variables and get the starting PID
    //map<string,string>::iterator iOVER;     //iterate over the overlays so they can be p,aced on the same core as each other
    map<string,bool> Placed;                //placed yet or not

    vector<unsigned> Vstart;                //vector of starting PIDs for each variable and its overlays
    unsigned Vsize;
    RAN.reserve(Nodes);
    MAP.reserve(Nodes);
    TargetTableEntry TTE;
    for(n=0;n<Nodes;n++){
        RAN.push_back(n);
        MAP.push_back(n);   //just to make room
    }
    if(Toss){                               //scramble the PIDs so that placement is randonised
        unsigned TMP,p1,p2;
        srand48(seed);
        for(n=0;n<Nodes;n++){
            p1 = lrand48()%Nodes;
            p2 = lrand48()%Nodes;
            TMP = RAN[p1];
            RAN[p1] = RAN[p2];
            RAN[p2] = TMP;
        }
    }
    MaxCoreNo = new int[MAC.ChipCount];
    for(int i=0; i< MAC.ChipCount;i++)
        MaxCoreNo[i] = -1;
    for(auto iVPID = TASK.pidStart.cbegin();iVPID != TASK.pidStart.cend(); iVPID++){ //mark all variables as not placed
        Placed.insert(map<string, bool>::value_type(iVPID->first,false));
    }
    
    for(auto iVPID = TASK.pidStart.cbegin();iVPID != TASK.pidStart.cend(); iVPID++){ //placement by variable name
        if(Placed[iVPID->first])                //check not done already
            continue;
        Placed[iVPID->first] = true;            //mark as placed as it will be soon
        Vstart.clear();
        Vstart.push_back(RAN[iVPID->second]);
        Vsize = TASK.nameSize[iVPID->first];
        for(auto iOVER = TASK.overlays12.cbegin();iOVER!=TASK.overlays12.cend();iOVER++){
            if(iVPID->first == iOVER->first){
                Vstart.push_back(RAN[TASK.pidStart[iOVER->second]]);
                if(TASK.nameSize[iOVER->second] != Vsize){
                    cerr << "something wrong in placement: sizes don't match"<<iVPID->first<<" and "<< iOVER->second<<endl;
                    exit(0);
                }
                Placed[iOVER->second] = true;   //mark as placed as it will be soon
            }
        }
        for(int s=0;s < Vsize; s++) {
            if(!StartingPlace((unsigned)Vstart.size())){
                cerr<<"run out of room matey - can't place the task\n";
                exit(0);
            }
            
            if((int)NextCore > MaxCoreNo[NextChip])
                MaxCoreNo[NextChip] = NextCore;
            XY = MAC.ChipXY[NextChip];
            X = XY>>8;
            Y = XY&0x0ff;
            for(int ov=0; ov < Vstart.size();ov++){
                n = Vstart[ov] + s;
		TTE = TASK.CoreFlix[RAN[n]];
		switch(TTE.Name[0]){
			case 'M':break;
			case 'X':
				Offset[NextChip][NextCore]++;
				break;
			case 'R':
				if(TTE.Name[1]=='O')
					Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+2;
				else
					Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+3;

				break;
			case 'P':
				Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+4;
				break;
			case 'A':
				Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+5;
				break;
			case 'N':
				Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+6;
				break;
			case 'L':
				Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+7;
				break;
			case 'C':
				Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+8;
				break;
			case 'B':
				Offset[NextChip][NextCore]=Offset[NextChip][NextCore]+9;
				break;

		}
                K = KeyFrom(X,Y,NextCore,Offset[NextChip][NextCore]);
		cout<<"Name:"<<TTE.Name[0]<<" Key:"<<K<<" Offset:"<<Offset[NextChip][NextCore]<<endl;
		MAP[RAN[n]] = K;
                PMAP.insert({K, n});
                TTE.Kd = K;
                CORE.CoreEntries.insert(map<unsigned, TargetTableEntry>::value_type(K,TTE));
            	//Offset[NextChip][NextCore]++;
	    }
        }
    }
}

void Dealer::PrintMappings(Task &TASK){
    unsigned Nodes = TASK.nextPID;
    int node;
    unsigned X,Y,C,O;
    puts("Node  Key KeyFromMap");
    for (node = 0; node < Nodes; node++) {
        KeyTo(MAP[node], X, Y, C, O);
        printf("%u  %x[%d,%d,%d,%d] %s \n",node,MAP[node],X,Y,C,O,TASK.elementFromPID(node).c_str());
    }
    
}
