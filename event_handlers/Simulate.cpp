#include <iostream>
#include <srdio.h>
#include "Simulate.h"


void Simulate::Update(Event E,Core CORE,Deal DEAL){
	unsigned Xd,Yd,Cd,Od;
	unsigned *W,*Vp;
	int temp;
 	KeyTo(E.kd,Xd,Yd,Cd,Od);
	W=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Weights[counter];
	if(W==-1){
		//do nothing
	}
	else if(W==0){//multiplication
		Vp=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].VinPoint;
		temp=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value;
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];//which one????
		counter++;
		LoadFire(Event E,Core CORE,Dealer DEAL);
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=temp;
	}
	else if(W==1){//addition
		for(int node=0;node<xNodes;node++){//Valecy care..how many and from where do you start??xNodes is the number of elements of the x vector
			temp+=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];//replace with value or lasres???
		}
		counter++;
		LoadFire(Event E,Core CORE,Dealer DEAL);

	}
	else if(W==2){//subtracting
	

	}
	else if(W==3){//assignement
		CORE.AllCores[Xd][Yd][Cd].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
	}
}
