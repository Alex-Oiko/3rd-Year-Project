//
//  Simulate.cpp
//  Matrix
//
//  Created by Jeff Reeve on 13/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "Simulate.h"

#include "Simulate.h"
#include <iostream>
#include "Simulate.h"
using namespace std;
//map<unsigned,unsigned> aKeys;
void Simulate::LoadFireAll(Task& TASK, Dealer& DEAL, Core& CORE){
    event E;
    TargetTableEntry TTE;
    E.Type = FIREAWAY;
    E.OutLink = 6;
    for(int node = 0; node < TASK.nextPID; node++){ //report initial state
        E.Ks = DEAL.MAP[node];
        auto iC = CORE.LUT.find(E.Ks);
        if(iC == CORE.LUT.cend())
            continue;
        auto iTTE = CORE.CoreEntries.find(E.Ks);
        TTE = iTTE->second;
        if(CORE.mop[TTE.V][16].front() == 3){
            cout<<TTE.Name[0]<<endl;
	    E.Value = TTE.IV;
            E.Kd = E.Ks;
	    CORE.mop[TTE.V][16].push(CORE.mop[TTE.V].at(16).front());
	    CORE.mop[TTE.V][16].pop();
            EventQ.push(E);
        }
	else if(TTE.Name[0]=='A'){
    		matrix_size=TTE.YD;
	}
	//else if(TTE.Name[0]=='A' && TTE.Name[1]=='\0'){
	//	aKeys.insert(map<unsigned,unsigned>::value_type(E.Ks,1));
	//	cout<<"Keys inserted are "<<E.Ks<<endl;
	//}
    }
}
void Simulate::UpdateAll(Task& TASK, Dealer& DEAL){
    event E;
    E.Type = UPDATE;
    E.Kd = -1;
    E.Value = -1;
    for(int node = 0; node < TASK.nextPID; node++){ //report initial state
        E.Ks = DEAL.MAP[node];
        EventQ.push(E);
    };
    cout<<"\nUPDATE ALL IS FINISHED"<<endl;
}

void Simulate::SimBegin(Task& TASK, Dealer& DEAL, Core& CORE, MakeMCTables& MCT, Machine& MAC) {
    event E;
    long loops = 0;
    unsigned Thru=0;
    iterator_counter=0;
    CoreHits = 0;
    CoreMisses = 0;
    case15_counter=0;
    end=0;
    CORE.AllocateStorage(TASK);
    puts("Starting simulation");
    LoadFireAll(TASK, DEAL,CORE);
    while(true){
        if(EventQ.empty()||end==1)
            break;
        E = EventQ.front();
        //printf("Event type = %d, %lu more events to go\n",E.Type, EventQ.size());
        //2cout << "Event type = " << E.Type << " " << EventQ.size() << " more events to go\n";
        EventQ.pop();
        if(E.Type == LOADFIRE)
            LoadFire(E, CORE, DEAL);
        else if(E.Type == UPDATE)
            Update(E, CORE);
        else if(E.Type == FIREAWAY)
            Thru += FireAway(E, MCT,CORE,MAC);
	else if(E.Type==UPALL)
	    UpdateAll(TASK,DEAL);
        else {
            printf("unknown event %d\n",E.Type);
            exit(1);
        }
        loops++;
    }
        printf("%lu events in total\n%u Through routes\n",loops,Thru);
        printf("%lu core hits and %lu core misses\n", CoreHits, CoreMisses);
        cout<<"Number of iterations "<<iterator_counter<<endl;
	cout<<"Matrix/Vector values"<<endl;
	CORE.PrintByOpCode(3,5);
	CORE.PrintByOpCode(2,7);


    
}
void Simulate::LoadFire(event E, Core& CORE, Dealer& DEAL){
    unsigned Xs, Ys, Cs, Os;
    TargetTableEntry TTE;
    KeyTo(E.Ks, Xs, Ys, Cs, Os);
    auto iTTE = CORE.CoreEntries.find(E.Ks);
    if(iTTE == CORE.CoreEntries.cend()){
        cerr << "Missing Target Table entry\n";
        exit(1);
    }
    TTE = iTTE->second;
    E.Value = TTE.V;
    E.Kd = E.Ks; //sourced from here
    E.Type = FIREAWAY;
    E.OutLink = 6; //this is the start
    EventQ.push(E);
    return;
}
void Simulate::Update(event E, Core& CORE){  //this is the timer interrupt
     vector<unsigned>  vTTE;
     event RES;
     TargetTableEntry TTE,kTTE;
     unsigned X,Y,C,O,oc,Offset;
     float Vme,Res;
     auto iTTE = CORE.LUT.find(E.Ks);
     if(iTTE == CORE.LUT.cend()){
     	CoreMisses++;
     	cerr << "Unknown Key "<<hex<<E.Ks<<endl;
     	return;
     }
     CoreHits++;
     auto iSTTE = CORE.CoreEntries.find(E.Ks);
     if(iSTTE == CORE.CoreEntries.cend()){
     	cerr << "SIM:No Core Entry for Ks " << hex << E.Ks<<endl;
     	exit(1);
  
     }
     KeyTo(E.Ks,X,Y,C,Offset);
     cout << "\nSource ";
     CORE.PrintTTE(iSTTE->second,Offset);
     vTTE = iTTE->second;
     kTTE=iSTTE->second;
     switch(kTTE.Name[0]){
     	case 'M':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][1].front()<<endl;
		CORE.mop[kTTE.V][1].pop();
	        cout<<"New OpCode at key 1 is:"<<CORE.mop[kTTE.V][1].front()<<endl;
     	      	break;
     	case 'X':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][16].front()<<endl;
		CORE.mop[kTTE.V][16].pop();
              	cout<<"New OpCode at key 16 is:"<<CORE.mop[kTTE.V][16].front()<<endl;
             	 break;
     	case 'R':
             	if(kTTE.Name[1]=='O'){
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][3].front()<<endl;
	     		CORE.mop[kTTE.V][3].pop();
			cout<<"New OpCode at key 3 is:"<<CORE.mop[kTTE.V][3].front()<<endl;
	     	}
             	else{
	     		cout<<"Size of OpCodes at key 0 is:"<<CORE.mop[kTTE.V][0].size();
		}
             	break;
     	case 'P':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][3].front()<<endl;
		CORE.mop[kTTE.V][3].pop();
	      	cout<<"New OpCode at key 3 is:"<<CORE.mop[kTTE.V][3].front()<<endl;
		RES.Ks=kTTE.Kd;
	      	RES.Kd=RES.Ks;
	      	RES.Type=FIREAWAY;	      
	      	RES.OutLink=6;
	      	RES.Value=CORE.Mstore[kTTE.V];
	      	cout<<"Firing p with value"<<CORE.Mstore[kTTE.V]<<endl;
	      	EventQ.push(RES);
     	      	break;
     	case 'A':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][0].front()<<endl;
		for(int s=0;s<matrix_size;s++){
     	      		CORE.mop[kTTE.V][0].pop();
		}
     	      	cout<<"New OpCode at key 0 is:"<<CORE.mop[kTTE.V][0].front()<<endl;
	     	break;
     	case 'N':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][0].front()<<endl;
		for(int s=0;s<matrix_size;s++){
			CORE.mop[kTTE.V][3].pop();
		}
	      	cout<<"New OpCode at key 3 is:"<<CORE.mop[kTTE.V][0].front()<<endl;
     	      	break;
     	case 'L':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][2].front()<<endl;
		CORE.mop[kTTE.V][2].pop();
	      	cout<<"New OpCode at key 2 is:"<<CORE.mop[kTTE.V][2].front()<<endl;
     	      	break;	
     	case 'B':
		cout<<"Previous OpCode was:"<<CORE.mop[kTTE.V][2].front()<<endl;
		CORE.mop[kTTE.V][2].pop();
	      	cout<<"New OpCode at key 2 is:"<<CORE.mop[kTTE.V][2].front()<<endl;
	      	break;
     }
     cout<<"\n"<<endl;
     /*if(kTTE.Name[0]=='A' || kTTE.Name[0]=='N'){
	cout<<"Opcodes to be deleted are "<<CORE.mop[kTTE.V].front()<<endl;
	CORE.mop[kTTE.V].pop();
	cout<<"and"<<CORE.mop[kTTE.V].front()<<endl;
	cout<<"erasing OpCodes in A or RSNEW"<<endl;
	CORE.ORE.mop[TTE.V][Offset].front()
	cout<<"New first OpCode is "<<CORE.mop[kTTE.V].front()<<endl;
     }
     else{
	cout<<"Opcodes to be deleted are "<<CORE.mop[kTTE.V].front()<<endl;
	cout<<"erasing clssic node"<<endl;
	CORE.mop[kTTE.V].pop();
	cout<<"New first OpCode is "<<CORE.mop[kTTE.V].front()<<endl;
     }
     if(kTTE.Name[0]=='P'){
        RES.Ks = kTTE.Kd;
        RES.Kd = RES.Ks;
        RES.Type = FIREAWAY;
        RES.OutLink = 6;
	RES.Value=CORE.Mstore[kTTE.V];
	cout<<"Firing P with value "<<CORE.Mstore[kTTE.V]<<endl;
	EventQ.push(RES);
     }*/
}

unsigned Simulate::FireAway(event E, MakeMCTables& MCT, Core& CORE, Machine& MAC){
    unsigned DATA, CORES, LINKS, ChipID;
    unsigned Xd,Yd,Cd,Od;
    int XYn,Xn,Yn, XYd;
    unsigned ThroughRouted=0;
    int NextChip;
    //map<unsigned,unsigned> rKeys={{0=0}};
    map<unsigned, unsigned>::iterator CID;
    KeyTo(E.Kd, Xd, Yd, Cd, Od);
    XYd = (Xd<<8) | Yd;
    CID = MAC.XYChip.find(XYd);
    ChipID = CID->second;
    int link, core;
    DATA = MCT.GetMCData(E.Ks, E.Kd);
    LINKS = DATA&0x03f;
    if(DATA == 0){ 
        LINKS = (1<<E.OutLink);//through route
        ThroughRouted++;
    }
    link = 1;
    for(int i = 0;LINKS && i < 6; i++, link = (link<<1)){
        if(link&LINKS){
	    E.OutLink = i;//set for through routing
            NextChip = MAC.ChipLink[ChipID][i];
            XYn = MAC.ChipXY[NextChip];
            Xn = XYn>>8;
            Yn = XYn&0xff;
            E.Kd = KeyFrom(Xn, Yn, Cd, Od); //off to new destination
            EventQ.push(E);
        }
    }
    CORES = (DATA>>7)&0x0ffff; //look only at cores 1 to 16 (0 to 15 in my data structures)
    core = 1;
    for(int i = 0;CORES && i < 16; i++,core = (core<<1)){   //deliver as needed
        if(core&CORES){
            E.Kd = KeyFrom(Xd, Yd, i, 0);
            E.Type = DELIVER;
            Deliver(E, CORE);
        }
    }
    return ThroughRouted;
}
void Simulate::Deliver(event E, Core& CORE){    //this is the MC packet arrival interrupt;
    event RES;
    vector<unsigned>  vTTE;
    TargetTableEntry TTE;
    unsigned Xs,Ys,Cs,Os,X,Y,C,O,oc,Offset,dOffset;
    float Vme,Res;
    auto iTTE = CORE.LUT.find(E.Ks);
    KeyTo(E.Ks,Xs,Ys,Cs,Offset);
    if(iTTE == CORE.LUT.cend()){
        CoreMisses++;
        cerr << "Unknown Key "<<hex<<E.Ks<<endl;
        return;
    }
    CoreHits++;
    auto iSTTE = CORE.CoreEntries.find(E.Ks);
    if(iSTTE == CORE.CoreEntries.cend()){
        cerr << "SIM:No Core Entry for Ks " << hex << E.Ks<<endl;
        exit(1);

    }
    cout << "\nSource "<<Offset;
    CORE.PrintTTE(iSTTE->second,Offset);
    vTTE = iTTE->second;
    for(int v = 0; v < vTTE.size(); v++){
        auto iC = CORE.CoreEntries.find(vTTE[v]);
        if(iC == CORE.CoreEntries.cend()){
            cerr << "SIM:No Core Entry for Kd " << hex << vTTE[v]<<endl;
            exit(1);
        }
        if(E.Kd != (vTTE[v]&0xfffff800))    //get the right core
           continue;
         cout << "Target ";
        TTE = iC->second;
        //put your code here - The values are in Core.Mstore[X][Y][C] (put the Key into "KeyTo" to get X,Y and C
        //KeyTo(E.Ks,X,Y,C,O);
        //Current value is at Core.Mstore[X][Y][C][iC->second.V]
	KeyTo(TTE.Kd,X,Y,C,dOffset);
        CORE.PrintTTE(TTE,Offset);
        RES.Ks = TTE.Kd;
        RES.Kd = RES.Ks;
        RES.Type = FIREAWAY;
        RES.OutLink = 6;
	switch(CORE.mop[TTE.V][Offset].front()){
		case 0:
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			cout<<"opcode is 0 here"<<endl;
			cout<<"new opcode is "<<CORE.mop[TTE.V][Offset].front()<<endl;
			break;
        	case 1://multiplication but no send
            		Vme = CORE.Mstore[TTE.V];
            		Res = Vme*E.Value;
            		RES.Value = Res;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
			CORE.mop[TTE.V][Offset].pop();
			cout<<"new opcode is "<<CORE.mop[TTE.V][Offset].front()<<endl;
			cout<<"Multiplication happened here"<<endl;
            		cout<<"resValue is "<<Res<<endl;
			if(TTE.Name[0]=='A'){
				CORE.Mstore[TTE.V]=0;
			}
			EventQ.push(RES);
			break;
        	case 2://specific for the creation of r
	    		CORE.Mcounter[TTE.V]++;
          		Vme = CORE.Mstore[TTE.V];
            		CORE.Mstore[TTE.V] -=E.Value;
	    		cout<<"COUNTER "<<CORE.Mcounter[TTE.V]<<"\n";
	    		if(CORE.Mcounter[TTE.V]==TTE.YD){
				RES.Value=CORE.Mstore[TTE.V];
	    			CORE.mop[TTE.V][Offset].pop();
				cout<<"new opcode is "<<CORE.mop[TTE.V][Offset].front()<<endl;
				cout<<"R CREATED"<<endl;
				cout<<"Value is "<<CORE.Mstore[TTE.V]<<endl;
				CORE.Mcounter[TTE.V]=0;
				EventQ.push(RES);
	    		}
        		break;
		case 4://assignement
			CORE.Mstore[TTE.V]=E.Value;
			CORE.Mtemp[TTE.V]=E.Value;
			RES.Value=CORE.Mstore[TTE.V];
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	 	   	CORE.mop[TTE.V][Offset].pop();
			cout<<"new opcode is "<<CORE.mop[TTE.V][Offset].front()<<endl;
			cout<<"ASSIGNEMENT DONE HERE"<<endl;
			cout<<CORE.Mstore[TTE.V]<<endl;
			EventQ.push(RES);
			break;
	
		case 5://constant
			CORE.Mcounter[TTE.V]++;
			CORE.Mstore[TTE.V]+=E.Value*E.Value;
			cout<<"Value is "<<CORE.Mstore[TTE.V]<<endl;
			cout<<"matrix_size is:"<<matrix_size<<endl;
			if(CORE.Mcounter[TTE.V]==matrix_size){//no TTE.YD because node size is 1, because it is single node
				RES.Value=CORE.Mstore[TTE.V];
				CORE.Mcounter[TTE.V]=0;
	    			CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    			CORE.mop[TTE.V][Offset].pop();
				cout<<"new opcode is "<<CORE.mop[TTE.V][Offset].front()<<endl;
				cout<<"RSOLD/RSNEW CREATED"<<endl;
				cout<<"Value is "<<CORE.Mstore[TTE.V]<<endl;
				if(TTE.Name[0]=='N')//so it does not have previous value in other iterations
					CORE.Mstore[TTE.V]=0;

				EventQ.push(RES);
			}
			break;
		case 6://to create Ap nodes
			cout<<"numbah 6 here"<<endl;
			CORE.Mcounter[TTE.V]++;
			CORE.Mstore[TTE.V]+=E.Value;
			cout<<"Opcode"<<CORE.mop[TTE.V][Offset].front()<<endl;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			if(CORE.Mcounter[TTE.V]==TTE.YD){
				RES.Value=CORE.Mstore[TTE.V];
				CORE.Mcounter[TTE.V]=0;
				cout<<"new opcode is "<<CORE.mop[TTE.V][Offset].front()<<endl;
				cout<<"value of node is"<<CORE.Mstore[TTE.V]<<endl;
				EventQ.push(RES);
			}
			break;
		case 7://Assignement without send
			CORE.Mstore[TTE.V]=E.Value;
			CORE.Mtemp[TTE.V]=E.Value;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			cout<<"Value is EDWWWWWWWWWWWW "<<CORE.Mstore[TTE.V]<<endl;
			cout<<"ASSIGNEMENT DONE HERE"<<endl;
			break;
		case 8://create alpha node
			if(CORE.Mcounter[TTE.V]==0)
				CORE.Mtemp[TTE.V]=0;

			CORE.Mcounter[TTE.V]++;
			CORE.Mtemp[TTE.V]+=E.Value;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			cout<<"ALPHA node here"<<endl;
		 	cout<<"Bottom happenning here "<<CORE.Mtemp[TTE.V]<<endl;
			if(CORE.Mcounter[TTE.V]==matrix_size){
				cout<<"rsold in Mstore is "<<CORE.Mstore[TTE.V]<<endl;
				CORE.Mstore[TTE.V]=CORE.Mstore[TTE.V]/CORE.Mtemp[TTE.V];
				RES.Value=CORE.Mstore[TTE.V];
				CORE.Mcounter[TTE.V]=0;
				cout<<"Value is "<<CORE.Mstore[TTE.V]<<endl;
				EventQ.push(RES);
			}
			break;
		//case 9://constant*node
		//	RES.Value=CORE.Mstore[TTE.V]*E.Value;
		//	cout<<"RES Value is"<<RES.Value<<endl;
		//	CORE.mop[TTE.V].erase(CORE.mop[TTE.V].begin());
		//	EventQ.push(RES);
		//	break;
		case 10://make new r nodes, basically subtraction
			cout<<"Making r values"<<endl;
			CORE.Mstore[TTE.V]-=E.Value;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			cout<<"R value is "<<CORE.Mstore[TTE.V]<<endl;
			RES.Value=CORE.Mstore[TTE.V];
			EventQ.push(RES);
			break;
		case 11://make new x nodes
			cout<<"MAKING AN X NODE HERE!!"<<endl;
			CORE.Mstore[TTE.V]+=E.Value;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			RES.Value=CORE.Mstore[TTE.V];
			cout<<"Value is "<<CORE.Mstore[TTE.V]<<endl;
			//EventQ.push(RES);
			break;
		case 12://make beta
			cout<<"CREATING BETA HERE"<<endl;
			CORE.Mstore[TTE.V]=E.Value/CORE.Mstore[TTE.V];
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	 	   	CORE.mop[TTE.V][Offset].pop();
			RES.Value=CORE.Mstore[TTE.V];
			CORE.Mstore[TTE.V]=E.Value;//save it as rsold for the next iteration
			cout<<"Value is "<<CORE.Mstore[TTE.V]<<endl;
			EventQ.push(RES);
			break;
		case 13://check end condition
			cout<<"Checking end cond"<<endl;
    			iterator_counter++;
			if(E.Value<1e-10){
				cout<<"END CONDITION TRUE..........TERMINATING"<<endl;
				cout<<"Results are"<<endl;
				CORE.PrintByOpCode(16,3);
				end=1;
				return;
			}
			else{
				cout<<"Condition failed"<<endl;
			}
			break;
		case 14://temp assignement only
			cout<<"ASSIGNING IN TEMP"<<endl;
			CORE.Mtemp[TTE.V]=E.Value;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			break;
		case 15://make new p nodes
			CORE.Mstore[TTE.V]*=E.Value;
			CORE.Mstore[TTE.V]+=CORE.Mtemp[TTE.V];
			cout<<"New p value is "<<CORE.Mstore[TTE.V]<<endl;
	    		CORE.mop[TTE.V][Offset].push(CORE.mop[TTE.V][Offset].front());
	    		CORE.mop[TTE.V][Offset].pop();
			case15_counter++;
			if(case15_counter==matrix_size){//when case15_counter==N 
				RES.Type=UPALL;
				EventQ.push(RES);
			}
			if(case15_counter>matrix_size){//if the first iteration has passed
				RES.Value=CORE.Mstore[TTE.V];
				EventQ.push(RES);
			}
			break;
	}
    }    
    return;
}
