#include <iostream>
#include <stdio.h>
#include "Simulate.h"

//Remember special start
void Simulate::Update(Event E,Core CORE,Deal DEAL){
	unsigned Xd,Yd,Cd,Od;
	unsigned *W,*Vp;
	int temp,rsold;
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
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value-CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].ValuesIn[Vp[0]];
		LoadFire(Event E,Core CORE,Dealer DEAL);

	}
	else if(W==3){//assignement
		CORE.AllCores[Xd][Yd][Cd].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		CORE.AllCores[Xd][Yd][Cd].lasres=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
	}
	else if(W==4){//constants
		for(int node=0;i<xNodes;i++){
			AllCores[Xd][Yd][Cd].Value+=CORE.AllCores[Xd][Yd][Cd].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]]^2;
			LoadFire(Event E,Core CORE,Deal DEAL);	
		}
	}
	else if(W==5){//constant*value
			AllCores[Xd][Yd][Cd].Value=CORE.AllCores[Xd][Yd][Cd].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			LoadFire(Event E,Core CORE,Deal DEAL);
	}
	else if(W==6){//calculate r
		for(int node=0;node<xNodes;node++){//Valecy care..how many and from where do you start??xNodes is the number of elements of the x vector
			temp+=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];//replace with value or lasres???
		}
		CORE.AllCores[Xd][Yd][Cd].Value=CORE.AllCores[Xd][Yd][Cd].Value-CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		LoadFire(Event E,Cpre CORE,Deal DEAL);
	

	}
}
