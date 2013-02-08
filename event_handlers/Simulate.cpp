#include <iostream>
#include <stdio.h>
#include "Simulate.h"

//counter++ when Q is empty,<--NO! When an update session is over
//Possie,VinPoint and ValueIn confusion
void Simulate::Update(Event E,Core CORE,Deal DEAL){
	unsigned Xd,Yd,Cd,Od;
	unsigned *W,*Vp;
	int temp,rsold;
 	KeyTo(E.kd,Xd,Yd,Cd,Od);
	W=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Weights[counter];
	Vp=CORE.AllCores[Xd][Y][Cd].TargetTable[Od].VinPoint;
	if(W==-1){
		//do nothing
	}
	else if(W==0){//multiplication
		temp=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value;
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];//which one????
		LoadFire(Event E,Core CORE,Dealer DEAL);
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=temp;//not sure if this will work
	}
	else if(W==1){//addition
		for(int node=0;node<xNodes;node++){//Valecy care..how many and from where do you start??xNodes is the number of elements of the x vector
			temp+=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];//replace with value or lasres???
		}
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=temp;
		LoadFire(Event E,Core CORE,Dealer DEAL);

	}
	else if(W==2){//subtracting
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value-CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].ValuesIn[Vp[0]];
		LoadFire(Event E,Core CORE,Dealer DEAL);

	}
	else if(W==3){//assignement. Not sure for both
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
	}
	else if(W==4){//constants
		for(int node=0;i<xNodes;i++){
			AllCores[Xd][Yd][Cd].TargetTable[Od].Value+=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]]^2;
		}
			LoadFire(Event E,Core CORE,Deal DEAL);	
	}
	else if(W==5){//constant*value
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			E.Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres;
			LoadFire(Event E,Core CORE,Deal DEAL);
	}
	else if(W==6){//calculate r & p'Ap
		for(int node=0;node<xNodes;node++){//Valecy care..how many and from where do you start??xNodes is the number of elements of the x vector
			temp+=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];//replace with value or lasres???
		}
		if(from Ap){//think about this
		CORE.AllCores[Xd][Yd][Cd].TargetTable.lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		E.Value=lasres;
		LoadFire(Event E,Core CORE,Deal DEAL);
		}
		else{
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value-CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		LoadFire(Event E,Core CORE,Deal DEAL);
		}
	}
	else if(W==7){//division
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value/CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		LoadFire(Event E,Core CORE,Deal DEAL);
	}
	else if(W==8 && AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]]<1e-10){//end condition
		return;
	}
	else if(W==9){//beta division
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]]/CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value
		LoadFire(Event E,Core CORE,Deal DEAL);	
	}
	else if(W==10){//last addition
		E.Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od]=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value+CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		LoadFire(Event E,Core CORE,Deal DEAL);	
	}

}
void Simulate::LoadFire(event E, Core CORE, Dealer DEAL){
     unsigned Xs, Ys, Cs, Os;//when are they initialised for use??
     KeyTo(E.Ks, Xs, Ys, Cs, Os);
     if(E.Value==NULL){
     	E.Value = CORE.AllCores[Xs][Ys][Cs].TargetTable[Os].Value;
     }
     E.Kd = E.Ks;    //start off at source
     E.Type = FIREAWAY;
     E.OutLink = 6; //this is the start
     EventQ.push(E);//IMPORTANT
     return;
}

