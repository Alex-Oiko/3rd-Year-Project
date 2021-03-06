#include <iostream>
#include <srdio.h>
#include "Simulate.h"


void Simulate::SimBegin(Task TASK, Dealer DEAL, Core CORE, MakeMCTables MCT) {
	event E;
	int l=0;
	long loops=0;
	puts("Starting Simulation");
	ComputingInX(E,CORE,DEAL);

}

void Simulate::ComputingInX(Event E,Core CORE,Deal DEAL){
	int begin_sim=0;
	int node;
	Event E;

	if(begin_sim==0){
		for(node=0;node<xNodes;node++){
			E.Ks=DEAL.MAP[node];
			LoadFire(E,CORE,DEAL);
		}
		ComputingInA(Core CORE);
	}
	else{}
}

void Simulate::ComputingInA(Core CORE){
	int i;
	unsigned Xd,Yd,Cd,Od;

	for(i=0;i<aNodes;i++){//all the matrix nodes
		KeyTo(KeyQ.front(),Xd,Yd,Cd,Od);
	        if(CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Fixed) //Boundary - don't touch	
			return;
		CORE.AllCores[Xd][Yd][Cd]TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd]TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd]TargetTable[Od].VinPoint;
	}
}

 void Simulate::LoadFire(event E, Core CORE, Dealer DEAL){
 	 unsigned Xs, Ys, Cs, Os;//when are they initialised for use??
	 KeyTo(E.Ks, Xs, Ys, Cs, Os);
	 E.Value = CORE.AllCores[Xs][Ys][Cs].TargetTable[Os].Value;
	 E.Kd = E.Ks;    //start off at source
	 E.Type = FIREAWAY;
	 E.OutLink = 6; //this is the start
	 EventQ.push(E);//IMPORTANT
	 return;
}

unsigned Simulate::FireAway(event E, MakeMCTables MCT, Core CORE){
     unsigned DATA, CORES, LINKS;
     unsigned Xd,Yd,Cd,Od;
     int XYn,Xn,Yn;
     unsigned ThroughRouted=0;
     KeyTo(E.Kd, Xd, Yd, Cd, Od);
     int link, core;
     DATA = MCT.GetMCData(E.Ks, E.Kd);
     LINKS = DATA&0x03f;
     if(DATA == 0){
         LINKS = (1<<E.OutLink);//through route
         ThroughRouted++;
   }
   link = 1;
   for(int i = 0;LINKS && i < 6; i++, link = (link<<1)){//multicasts here to other links
         if(link&LINKS){
             E.OutLink = i;//set for through routing
 
          if(MCT.Wrap)
             XYn = MCT.NeighbourWrap(Xd, Yd, i);
             else
             XYn = MCT.Neighbour(Xd, Yd, i);
             MCT.RevertXY(XYn, Xn, Yn);
             E.Kd = KeyFrom(Xn, Yn, Cd, Od); //off to new destination
             EventQ.push(E);
          }
     }
     CORES = (DATA>>7)&0x0ffff; //look only at cores 1 to 16 (0 to 15 in my data structures)   
     core = 1;
     for(int i = 0;CORES && i < 16; i++,core = (core<<1)){   //deliver as needed to cores
         if(core&CORES){
             E.Kd = KeyFrom(Xd, Yd, i, 0);
             E.Type = DELIVER;
             Deliver(E, CORE);
       	     }
     }
     return ThroughRouted;
}

void Simulate::Deliver(event E, Core CORE){
     unsigned Xd, Yd, Cd, Od;
     unsigned VinP;
     map<unsigned, unsigned>::iterator InVal;
     KeyTo(E.Kd,Xd, Yd, Cd, Od);
     InVal = CORE.AllCores[Xd][Yd][Cd].LUT.find(E.Ks);//find through the source address
     if(InVal == CORE.AllCores[Xd][Yd][Cd].LUT.end())
         return;     //delivered to disinterested core
     VinP = InVal->second;
     CORE.AllCores[Xd][Yd][Cd].ValuesIn[VinP] = E.Value;
     KeyQ.push(E.kd);
     return;
}

