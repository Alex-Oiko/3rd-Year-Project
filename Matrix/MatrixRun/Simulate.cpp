//
//  Simulate.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 27/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "Simulate.h"
#include <iostream>
#include <iomanip>
using namespace std;

void Simulate::PrintByOpCode(unsigned OpCode, unsigned Q,TCram& TCRAM){
    unsigned X,Y,C,Lup;

    uint32_t *CoreData;
    float *Values;
    TCram::_CoreCommon *CoreCommon;
    TCram::_DTTE *TTE;
    for(X= 0; X < TCRAM.NX; X++)
        for(Y=0; Y < TCRAM.NY; Y++)
            for(C = 0; C< TCRAM.NC; C++){
                CoreData = TCRAM.TCData[X][Y][C];
                if(CoreData == NULL)
                    continue;
                CoreCommon = (TCram::_CoreCommon*)CoreData;
                Values = (float*)&CoreData[CoreCommon->ValuesStart];
                TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
		for(Lup = 0; Lup < CoreCommon->PointCount; Lup++){
		    if(OpCodesA[TTE[Lup].V][Q].front() == OpCode){
                        cout << "Kd = "<<hex<<TTE[Lup].Kd<<" OpCode = "<<dec<<TTE[Lup].OpCode<<" IV = "<<TTE[Lup].IV<<" V = "<< Values[TTE[Lup].oV]<<" Expected "<<TTE[Lup].Expected<<" Arrived "<<TTE[Lup].Arrived<<endl;
                    }
                }
            }
}

void Simulate::UpdateAll(TCram& TCRAM){
    unsigned X,Y,C,Lup,node_counter;
    event E;
    uint32_t *CoreData;
    TCram::_CoreCommon *CoreCommon;
    TCram::_DTTE *TTE;
    E.Type = UPDATE;
    E.OutLink = 6;
    for(X= 0; X < TCRAM.NX; X++){
        for(Y=0; Y < TCRAM.NY; Y++){
            for(C = 0; C< TCRAM.NC; C++){
                CoreData = TCRAM.TCData[X][Y][C];
                if(CoreData == NULL)
                    continue;
                CoreCommon = (TCram::_CoreCommon*)CoreData;
                TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
		//for(Lup = 0; Lup < CoreCommon->PointCount; Lup++){
                E.Ks=TTE[Lup].Kd;
		EventQ.push(E);
		node_counter++;
		//}
            }
	}
    }
    cout<<"UpdateAll finished. Node counter is "<<node_counter<<endl;
}

void Simulate::LoadFireAll(TCram& TCRAM){
    unsigned X,Y,C,Lup;
    event E;
    uint32_t *CoreData;

    TCram::_CoreCommon *CoreCommon;
    TCram::_DTTE *TTE;
    E.Type = FIREAWAY;
    E.OutLink = 6;
    for(X= 0; X < TCRAM.NX; X++)
        for(Y=0; Y < TCRAM.NY; Y++)
            for(C = 0; C< TCRAM.NC; C++){
                CoreData = TCRAM.TCData[X][Y][C];
                if(CoreData == NULL)
                    continue;
                CoreCommon = (TCram::_CoreCommon*)CoreData;
                TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
		for(Lup = 0; Lup < CoreCommon->PointCount; Lup++){
		    if(OpCodesA[TTE[Lup].V][16].front() == 3){
                        E.Value = TTE[Lup].IV;
                        E.Ks = TTE[Lup].Kd;
                        E.Kd = E.Ks;
			EventQ.push(E);
                    }
		    else if(TTE[Lup].Name=='A'){
			matrix_size=TTE[Lup].YD;
		    }
                }
            }
}

void Simulate::SimBegin(TCram& TCRAM, MCLines& MCT, Machine& MAC) {
    event E;
    long loops = 0;
    unsigned Thru=0;
    CoreHits = 0;
    CoreMisses = 0;
    OpCodesA=TCRAM.OpCodesA;
    counters = TCRAM.counters;
    Temps = TCRAM.Temps;
    puts("Starting simulation");
    LoadFireAll(TCRAM);
    while(true){
        if(EventQ.empty())
            break;
        E = EventQ.front();
        EventQ.pop();
        if(E.Type == UPDATE)
            Update(E,TCRAM);
        else if(E.Type == FIREAWAY)
            Thru += FireAway(E, MCT,TCRAM,MAC);
	else if(E.Type=UPALL)
	    UpdateAll(TCRAM);
        else {
            printf("unknown event %d\n",E.Type);
            exit(1);
        }
        loops++;
    }
    printf("%lu events in total\n%u Through routes\n",loops,Thru);
    printf("%lu core hits and %lu core misses\n", CoreHits, CoreMisses);
    PrintByOpCode(7, 2,TCRAM);
    
}
void Simulate::Update(event E, TCram& TCRAM){  //this is the timer interrupt
     uint32_t Xs,Ys,Cs,Os;
     KeyTo(E.Ks,Xs,Ys,Cs,Os);
     uint32_t LUTCount = 0, InPoint;
     uint32_t *CoreData,*CoreDatas;
     float Vme,Res, *Values;
     TCram::_LookUp *LUT;
     TCram::_CoreCommon *CoreCommon;
     TCram::_DTTE *TTE, dTTE;
     bool found = false;
     event RES;
     CoreData = TCRAM.TCData[Xs][Ys][Cs];
     CoreCommon = (TCram::_CoreCommon*)CoreData;
     LUTCount = CoreCommon->LUTCount;
     LUT = (TCram::_LookUp*)&CoreData[CoreCommon->LUTStart];
     TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
     Values = (float*)&CoreData[CoreCommon->ValuesStart];
     cout<<"Source {Kd="<<E.Ks<<",Name="<<TTE[InPoint].Name<<",Offset="<<Os<<"}"<<endl;
     //for(int k = 0; k < LUTCount ; k++){
     found = true;
     CoreHits++;
     dTTE = TTE[InPoint];
	switch(dTTE.Name){
     		case 'M':
			cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][1].front()<<endl;
			OpCodesA[dTTE.V][1].pop();
		        cout<<"New OpCode at key 1 is:"<<OpCodesA[dTTE.V][1].front()<<endl;
     	      		break;
	     	case 'X':
			//dont delete anything
        	     	 break;
	     	case 'R':
        	     	if(Os==2){
				cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][3].front()<<endl;
		     		OpCodesA[dTTE.V][3].pop();
				cout<<"New OpCode at key 3 is:"<<OpCodesA[dTTE.V][3].front()<<endl;
	     		}
             		else{
	     			cout<<"Size of OpCodes at key 0 is:"<<OpCodesA[dTTE.V][0].size();
			}
             		break;
	     	case 'P':
			cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][3].front()<<endl;
			OpCodesA[dTTE.V][3].pop();
		      	cout<<"New OpCode at key 3 is:"<<OpCodesA[dTTE.V][3].front()<<endl;
			RES.Ks=dTTE.Kd;
	      		RES.Kd=RES.Ks;
		      	RES.Type=FIREAWAY;	      
		      	RES.OutLink=6;
	      		RES.Value=Values[dTTE.oV];
		      	cout<<"Firing p with value"<<Values[dTTE.oV]<<endl;
		      	EventQ.push(RES);
     	      		break;
	     	case 'A':
			cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][0].front()<<endl;
			for(int s=0;s<matrix_size;s++){
     	      			OpCodesA[dTTE.V][0].pop();
			}
     		      	cout<<"New OpCode at key 0 is:"<<OpCodesA[dTTE.V][0].front()<<endl;
	     		break;
	     	case 'N':
			cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][0].front()<<endl;
			for(int s=0;s<matrix_size;s++){
				OpCodesA[dTTE.V][3].pop();
			}
		      	cout<<"New OpCode at key 3 is:"<<OpCodesA[dTTE.V][0].front()<<endl;
     		      	break;
	     	case 'L':
			cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][2].front()<<endl;
			OpCodesA[dTTE.V][2].pop();
		      	cout<<"New OpCode at key 2 is:"<<OpCodesA[dTTE.V][2].front()<<endl;
     		      	break;	
	     	case 'B':
			cout<<"Previous OpCode was:"<<OpCodesA[dTTE.V][2].front()<<endl;
			OpCodesA[dTTE.V][2].pop();
		      	cout<<"New OpCode at key 2 is:"<<OpCodesA[dTTE.V][2].front()<<endl;
		      	break;
	}
     	cout<<"\n"<<endl;
	//}
    //}
}
unsigned Simulate::FireAway(event E, MCLines& MCT, TCram& TCRAM, Machine& MAC){
    unsigned DATA, CORES, LINKS, ChipID;
    unsigned Xd,Yd,Cd,Od,Kd;
    int XYn,Xn,Yn, XYd;
    unsigned ThroughRouted=0;
    int NextChip;
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
            Kd = KeyFrom(Xd, Yd, i, 0);
            InComing(E.Ks,E.Value,Kd,TCRAM);
        }
    }
    return ThroughRouted;
}

void Simulate::InComing(uint32_t Ks, float Vs, uint32_t Kd, TCram& TCRAM){
    uint32_t Xd, Yd, Cd, Od,Xs,Ys,Cs,Os;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    KeyTo(Ks,Xs,Ys,Cs,Os);
    uint32_t LUTCount = 0, InPoint;
    uint32_t *CoreData,*CoreDatas;
    float Vme,Res, *Values;
    TCram::_LookUp *LUT;
    TCram::_CoreCommon *CoreCommon,*CoreCommons;
    TCram::_DTTE *TTE, dTTE,*TTEs;
    bool found = false;
    event RES;
    CoreData = TCRAM.TCData[Xd][Yd][Cd];
    CoreDatas = TCRAM.TCData[Xs][Ys][Cs];
    CoreCommon = (TCram::_CoreCommon*)CoreData;
    CoreCommons = (TCram::_CoreCommon*)CoreDatas;
    LUTCount = CoreCommon->LUTCount;
    LUT = (TCram::_LookUp*)&CoreData[CoreCommon->LUTStart];
    TTE = (TCram::_DTTE*)&CoreData[CoreCommon->TTStart];
    TTEs = (TCram::_DTTE*)&CoreDatas[CoreCommons->TTStart];
    Values = (float*)&CoreData[CoreCommon->ValuesStart];
    cout<<"Source {Kd="<<Kd<<",Name="<<TTEs[InPoint].Name<<",Offset="<<Os<<"}"<<endl;
    cout<<"Target {"<<"Ks="<<Ks<<",Name="<<TTE[InPoint].Name<<",Offset="<<Od<<"}"<<endl;
    RES.Ks=TTE[InPoint].Kd;
    RES.Kd=RES.Ks;
    RES.Type=FIREAWAY;
    RES.OutLink=6;
    cout<<"Offset "<<Od<<endl;
    for(int k = 0; k < LUTCount ; k++){
        if(Ks == LUT[k].Ks){
            found = true;
            CoreHits++;
            InPoint = LUT[k].idx;
            dTTE = TTE[InPoint];
	    switch(OpCodesA[dTTE.V][Os].front()){
		case 0:
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			cout<<"opcode is 0 here"<<endl;
			cout<<"new opcode is "<<OpCodesA[dTTE.V][Os].front()<<endl;
			break;
        	case 1://multiplication but no send
            		Vme = Values[dTTE.oV];
            		Res = Vme*Vs;
            		RES.Value = Res;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
			OpCodesA[dTTE.V][Os].pop();
			cout<<"new opcode is "<<OpCodesA[dTTE.V][Os].front()<<endl;
			cout<<"Multiplication happened here"<<endl;
            		cout<<"resValue is "<<Res<<endl;
			if(dTTE.Name=='A'){
				Values[dTTE.oV]=0;
			}
			EventQ.push(RES);
			break;
        	case 2://specific for the creation of r
	    		counters[dTTE.V]++;
          		Vme = Values[dTTE.oV];
            		Values[dTTE.oV] -=Vs;
	    		cout<<"COUNTER "<<counters[dTTE.V]<<"\n";
	    		if(counters[dTTE.V]==dTTE.YD){
				RES.Value=Values[dTTE.oV];
	    			OpCodesA[dTTE.V][Os].pop();
				cout<<"new opcode is "<<OpCodesA[dTTE.V][Os].front()<<endl;
				cout<<"R CREATED"<<endl;
				counters[dTTE.V]=0;
				EventQ.push(RES);
	    		}
        		break;
		case 4://assignement
			Values[dTTE.oV]=Vs;
			Temps[dTTE.V]=Vs;
			RES.Value=Values[dTTE.oV];
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	 	   	OpCodesA[dTTE.V][Os].pop();
			cout<<"new opcode is "<<OpCodesA[dTTE.V][Os].front()<<endl;
			cout<<"ASSIGNEMENT DONE HERE"<<endl;
			cout<<Values[dTTE.oV]<<endl;
			EventQ.push(RES);
			break;
	
		case 5://constant
			counters[dTTE.V]++;
			Values[dTTE.oV]+=Vs*Vs;
			cout<<"Value is "<<Values[dTTE.oV]<<endl;
			cout<<"matrix_size is:"<<matrix_size<<endl;
			if(counters[dTTE.V]==matrix_size){//no TTE.YD because node size is 1, because it is single node
				RES.Value=Values[dTTE.oV];
				counters[dTTE.V]=0;
	    			OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    			OpCodesA[dTTE.V][Os].pop();
				cout<<"new opcode is "<<OpCodesA[dTTE.V][Os].front()<<endl;
				cout<<"RSOLD/RSNEW CREATED"<<endl;
				cout<<"Value is "<<Values[dTTE.oV]<<endl;
				if(dTTE.Name=='N')//so it does not have previous value in other iterations
					Values[dTTE.oV]=0;

				EventQ.push(RES);
			}
			break;
		case 6://to create Ap nodes
			cout<<"numbah 6 here"<<endl;
			counters[dTTE.V]++;
			Values[dTTE.oV]+=Vs;
			cout<<"Opcode"<<OpCodesA[dTTE.V][Os].front()<<endl;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			if(counters[dTTE.V]==dTTE.YD){
				RES.Value=Values[dTTE.oV];
				counters[dTTE.V]=0;
				cout<<"new opcode is "<<OpCodesA[dTTE.V][Os].front()<<endl;
				cout<<"value of node is"<<Values[dTTE.oV]<<endl;
				EventQ.push(RES);
			}
			break;
		case 7://Assignement without send
			Values[dTTE.oV]=Vs;
			Temps[dTTE.V]=Vs;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			cout<<"Value is EDWWWWWWWWWWWW "<<Values[dTTE.oV]<<endl;
			cout<<"ASSIGNEMENT DONE HERE"<<endl;
			break;
		case 8://create alpha node
			if(counters[dTTE.V]==0)
				Temps[dTTE.V]=0;

			counters[dTTE.V]++;
			Temps[dTTE.V]+=Vs;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			cout<<"ALPHA node here"<<endl;
		 	cout<<"Bottom happenning here "<<Temps[dTTE.V]<<endl;
			if(counters[dTTE.V]==matrix_size){
				cout<<"rsold in Mstore is "<<Values[dTTE.oV]<<endl;
				Values[dTTE.oV]=Values[dTTE.oV]/Temps[dTTE.V];
				RES.Value=Values[dTTE.oV];
				counters[dTTE.V]=0;
				cout<<"Value is "<<Values[dTTE.oV]<<endl;
				EventQ.push(RES);
			}
			break;
		//case 9://constant*node
		//	RES.Value=Values[dTTE.oV]*Vs;
		//	cout<<"RES Value is"<<RES.Value<<endl;
		//	CORE.mop[TTE.V].erase(CORE.mop[TTE.V].begin());
		//	EventQ.push(RES);
		//	break;
		case 10://make new r nodes, basically subtraction
			cout<<"Making r values"<<endl;
			Values[dTTE.oV]-=Vs;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			cout<<"R value is "<<Values[dTTE.oV]<<endl;
			RES.Value=Values[dTTE.oV];
			EventQ.push(RES);
			break;
		case 11://make new x nodes
			cout<<"MAKING AN X NODE HERE!!"<<endl;
			Values[dTTE.oV]+=Vs;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			RES.Value=Values[dTTE.oV];
			cout<<"Value is "<<Values[dTTE.oV]<<endl;
			//EventQ.push(RES);
			break;
		case 12://make beta
			cout<<"CREATING BETA HERE"<<endl;
			Values[dTTE.oV]=Vs/Values[dTTE.oV];
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	 	   	OpCodesA[dTTE.V][Os].pop();
			RES.Value=Values[dTTE.oV];
			Values[dTTE.oV]=Vs;//save it as rsold for the next iteration
			cout<<"Value is "<<Values[dTTE.oV]<<endl;
			EventQ.push(RES);
			break;
		case 13://check end condition
			cout<<"Checking end cond"<<endl;
    			iterator_counter++;
			if(Vs<1e-10){
				cout<<"END CONDITION TRUE..........TERMINATING"<<endl;
				cout<<"Results are"<<endl;
				PrintByOpCode(3,16,TCRAM);
				exit(EXIT_SUCCESS);	
			}
			else{
				cout<<"Condition failed"<<endl;
			}
			break;
		case 14://temp assignement only
			cout<<"ASSIGNING IN TEMP"<<endl;
			Temps[dTTE.V]=Vs;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			break;
		case 15://make new p nodes
			Values[dTTE.oV]*=Vs;
			Values[dTTE.oV]+=Temps[dTTE.V];
			cout<<"New p value is "<<Values[dTTE.oV]<<endl;
	    		OpCodesA[dTTE.V][Os].push(OpCodesA[dTTE.V][Os].front());
	    		OpCodesA[dTTE.V][Os].pop();
			case15_counter++;
			if(case15_counter==matrix_size){//when case15_counter==N 
				RES.Type=UPALL;
				EventQ.push(RES);
			}
			//if(case15_counter>matrix_size){//if the first iteration has passed
			//	RES.Value=Values[dTTE.oV];
			//	EventQ.push(RES);
			//}
			break;
		}

        }
            
        if(LUT[k].Ks > Ks)
            break;
    }
    if(!found)
        CoreMisses++;
  cout<<"\n"<<endl;
}




