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
	name=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].name;
	Vp=CORE.AllCores[Xd][Y][Cd].TargetTable[Od].V;

	if(name=='A'){
		if(counter==0){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];//which one????
	                 E.Values=CORE.AllCores[Xd][Yd].TargetTable[Od].lasres;
	                 LoadFire(Event E,Core CORE,Dealer DEAL);
		}
	}
}

void Simulate::LoadFire(event E, Core CORE, Dealer DEAL){
     unsigned Xs, Ys, Cs, Os;
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
void Simulate::SimBegin(Task TASK, Dealer DEAL, Core CORE, MakeMCTables MCT) {
   event E;
   int l = 0;
   long loops = 0;
   unsigned Thru=0;
   puts("Starting simulation");
   counter=0;
   LoadFireAll(TASK, DEAL);
    while(true){
       if(EventQ.empty()){
         if(l == 40) break;
	 if(counter==14){counter=4;}
          UpdateAll(TASK, DEAL);
          counter++;
	  l++;
        }
        E = EventQ.front();
        //printf("Event type = %d, %lu more events to go\n",E.Type, EventQ.size());
        //2cout << "Event type = " << E.Type << " " << EventQ.size() << " more events to go\n";
        EventQ.pop();
        if(E.Type == LOADFIRE)
            LoadFire(E, CORE, DEAL);
        else if(E.Type == UPDATE)
            Update(E, CORE);
        else if(E.Type == FIREAWAY)
            Thru += FireAway(E, MCT,CORE);
        else {
            printf("unknown event %d\n",E.Type);
            exit(1);
        }
        loops++;
   }
   printf("%lu events in total\n%u Through routes\n",loops,Thru);
}


