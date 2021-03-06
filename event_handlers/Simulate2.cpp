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
		if(counter==0 || counter==4){
			 CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];//which one????
	                 E.Values=CORE.AllCores[Xd][Yd].TargetTable[Od].lasres;
	                 LoadFire(Event E,Core CORE,Dealer DEAL);
		}
		else{//do nothing

		}
	}
	else if(name=='R'){
		if(counter==1){
			for(int node=0;node<CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].XD;node++){
	                        temp+=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];
	                 }
	                 CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value-temp;
	                 LoadFire(Event E,Core CORE,Deal DEAL);
		}
		else if(counter==8){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=-CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];
			LoadFire(Event E,Core CORE,Deal DEAL);
		}
		else if(counter==11){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value+CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			E.Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres;
			LoadFire(Event E,Core CORE,Deal DEAL);	
		}
	}
	else if(name=='P'){
		if(counter==2 || counter==12){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			LoadFire(Event E,Core CORE,Dealer DEAL);
		}
		else if(counter==7 || counter==10){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			E.Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres;
			LoadFire(Event E,Core CORE,Deal DEAL);
		}
	}
	else if(name=='rsol'){
		if(counter==2){
			for(int node=0;node<CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].X;node++){
				CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value+=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]]^2;
			}
				LoadFire(Event E,Core CORE,Deal DEAL);
		}	
	}
	else if(name='alph'){
		if(counter==3){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];	
		}
		else if(counter==6){
			for(int node=0;node<xNodes;node++){
				temp=+CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];
			}
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=/temp
			LoadFire(Event E,Core CORE,Dealer DEAL);
		}	
	}
	else if(name=='beta'){
		if(counter==3){		
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		}
		if(counter==9){
			CORE.AllCores[Xd][Yd][Od].TargeTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]]/CORE.AllCores[Xd][Yd][Od].TargeTable[Od].Value;
			LoadFire(Event E,Core CORE,Dealer DEAL);	
		}	
	}
	else if(name=='Ap'){
		if(counter==4){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
		}
		else if(counter==5){
			for(int node=0;node<CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].X;node++){
                         	temp+=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];
                	}
                        CORE.AllCores[Xd][Yd][Cd].TargetTable.Value=temp;
                        CORE.AllCores[Xd][Yd][Cd].TargetTable.lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*temp;
                        E.Value=lasres;
                        LoadFire(Event E,Core CORE,Deal DEAL);
        	}
		else if(counter==7){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value*CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[0]];
			E.Value=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].lasres;
			LoadFire(Event E,Core CORE,Deal DEAL);
		}
	}
	else if(name=='x'){
		CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=+CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]];
		LoadFire(Event E,Core CORE,Deal DEAL);		
	}
	else if(name=='rsn'){
		for(int node=0;node<CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].X;node++){
			CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value+=CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value=CORE.AllCores[Xd][Yd][Cd].ValuesIn[Vp[node]]^2;
		}
		if(CORE.AllCores[Xd][Yd][Cd].TargetTable[Od].Value<1e-10)
			return;		
		else 
			LoadFire(Event E,Core CORE,Deal DEAL);	
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
	 if(counter==4){LoadFireAll();}
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


