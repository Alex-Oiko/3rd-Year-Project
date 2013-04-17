//
//  TCRAM.cpp
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//
#include <iostream>
#include <sys/stat.h>
#include "TCRAM.h"
#include "Utils.h"
TCram::TCram(uint32_t XD, uint32_t YD, uint32_t CD){
    TCData = new uint32_t***[XD];
    NX = XD;
    NY = YD;
    NC = CD;
    for(int X = 0; X < XD; X++){
        TCData[X] = new uint32_t**[YD];
        for(int Y = 0; Y< YD;Y++){
            TCData[X][Y] = new uint32_t*[CD];
            for(int C = 0; C < CD; C++){
                TCData[X][Y][C] = NULL;
            }
        }
    }
    
}

/*
uint32_t TCram::GetValue(uint32_t Ks){
    unsigned Xs, Ys, Cs, Os;
    uint32_t Value = 0;
    uint32_t *CoreData;
    TCram::_CoreCommon *CoreCommon;
    TCram::_Gpoint *Gpoint;
    uint32_t GPindex;
    KeyTo(Ks, Xs, Ys, Cs, Os);
    CoreData = TCData[Xs][Ys][Cs];
    CoreCommon = (_CoreCommon*)CoreData;
    GPindex = CoreCommon->pGpoint;
    while (GPindex) {
        Gpoint = (_Gpoint*)&CoreData[GPindex];
        if(Ks == Gpoint->Kd){
            Value = Gpoint->Value;
            break;
        }
        GPindex = Gpoint->pNextGpoint;
    }
    return Value;
}


void TCram::GetAllKeys(vector<uint32_t>& ValidKeys){
    uint32_t *CoreData;
    _CoreCommon *CoreCommon;
    uint32_t GPindex;
    _Gpoint *Gpoint;
    for(int X = 0; X< NX;X++){
        for(int Y = 0; Y < NY; Y++) {
            for (int C = 0; C < NC; C++) {
                CoreData = TCData[X][Y][C];
                if (!CoreData)
                    continue;
                CoreCommon = (_CoreCommon*)CoreData;
                GPindex = CoreCommon->pGpoint;
                while (GPindex) {
                    Gpoint = (_Gpoint*)&CoreData[GPindex];
                    ValidKeys.push_back(Gpoint->Kd);
                    GPindex = Gpoint->pNextGpoint;
                }
            }
        }
    }
}
*/
/*
uint32_t TCram::GetNewValue(_Gpoint* Gpoint, uint32_t* CoreData){
    uint32_t Wplace = Gpoint->pWeights, Vplace = Gpoint->pVinPoint, Count = Gpoint->Valency;
    _CoreCommon *CoreCommon = (_CoreCommon*)CoreData;
    uint32_t *CoreValues = &CoreData[CoreCommon->ValueStart];
    long long Wt,Vt, WVt;
    if(Gpoint->Fixed)
        return Gpoint->Value;
    WVt = 0;
    for(int node = 0; node < Count; node++){
        Wt = (long long)CoreData[node+Wplace];
        Vt = (long long)CoreValues[CoreData[Vplace+node]];
        WVt += Wt*Vt;
    }
    return (uint32_t)(WVt>>31);
}

uint32_t TCram::Update(uint32_t Kd){
    uint32_t Xd, Yd, Cd, Od;
    uint32_t Value = 0;
    uint32_t *CoreData;
    
    _CoreCommon *CoreCommon;
    _Gpoint *Gpoint;
    uint32_t GPindex;
    KeyTo(Kd, Xd, Yd, Cd, Od);
    CoreData = TCData[Xd][Yd][Cd];
    CoreCommon = (_CoreCommon*)CoreData;
    GPindex = CoreCommon->pGpoint;
    while (GPindex) {
        Gpoint = (_Gpoint*)&CoreData[GPindex];
        if(Kd == Gpoint->Kd){
            Value = GetNewValue(Gpoint, CoreData);
            if(Gpoint->Value == Value)
                printf("%u %u %u %u Value = %u\n",Xd,Yd,Cd,Od,Value);
            Gpoint->Value = Value;
            Gpoint->Arrived = 0;
            break;
        }
        GPindex = Gpoint->pNextGpoint;
    }
    return Value;
    i
}

*/



bool TCram::ReadData(string TCDataFile){
    uint32_t Rtype = 0, XY = 0, X = 0, Y = 0, Chip, ChipCount = 0, CoreID = 0, StartAddress = 0,CoreCount = 0, WordCount = 0, C;
    struct stat stat_record;
    if(stat(TCDataFile.c_str(), &stat_record)){
        cerr << "Can't find TCRAM table file " << TCDataFile <<"\n";
        return false;
    }
    if(!stat_record.st_size){
        cerr << "Nothing in TCRAM file " << TCDataFile <<"\n";
        return false;
    }

    FILE *TCFile;
    TCFile = fopen(TCDataFile.c_str(), "rb");
    if(!TCFile){
        cout << "Couldn't open " + TCDataFile + " file matey\n";
        return false;
    }
    fread(&Rtype, 1, 1, TCFile);
    if(Rtype != 0x71){
        cout <<TCDataFile + " is the wrong file type matey\n";
        return false;
    }
    holder_size=17;
    OpCodesA = new uint32_t*[holder_size];
    //for(int y=0;y<holder_size;y++){
//	OpCodesA[y]=new uint32_t[holder_size];
  //  }
    fread(&Rtype,1,3,TCFile);
    fread(&ChipCount,4,1,TCFile);
    for(Chip = 0; Chip < ChipCount; Chip++) {
        fread(&XY,2,1,TCFile);
        X = XY >> 8;
        Y = XY&0x0FF;
	fread(&CoreCount, 2, 1, TCFile);
        for(C=0;C<CoreCount;C++) {
	    cout<<"New node here"<<endl;
            fread(&StartAddress, 4, 1, TCFile);
            fread(&CoreID, 4, 1, TCFile);
            fread(&WordCount, 4, 1, TCFile);
	    cout<<"WordCount "<<WordCount<<endl;
            TCData[X][Y][CoreID-1] = new uint32_t[WordCount];
            fread(TCData[X][Y][CoreID-1], 4, WordCount, TCFile);
            uint32_t  size=0,valid_qs=0;
	    fread(&size,sizeof(uint32_t),1,TCFile);
	    cout<<"size is "<<size<<endl;
	    uint32_t oV=0;
	    fread(&oV,sizeof(uint32_t),1,TCFile);
	    cout<<"V number is "<<oV<<endl;
	    uint32_t OpCodes[size];
	    //OpCodes = new uint32_t[size];
	    fread(&OpCodes[0],sizeof(uint32_t),size,TCFile);
	    //queue<uint32_t> data;
	    //queue<uint32_t> *holder = new queue<uint32_t>[holder_size];
	    for(int p=0;p<size;p++){
		cout<<"Value is "<<OpCodes[p]<<endl;
	    }
	    OpCodesA[oV] = new uint32_t[size];
	    uint32_t counter=0;
	    uint32_t index=0;
	    uint32_t q_size=0;
	    /*for(int k=0;k<valid_qs;k++){	
		q_size=OpCodes[counter];
		cout<<"q_size "<<q_size<<endl;
		counter++;
		index=OpCodes[counter];
		cout<<"index "<<index<<endl;
		counter++;
		for(int f=0;f<q_size;f++){
			cout<<"value is "<<OpCodes[counter]<<endl;
			data.push(OpCodes[counter]);
			counter++;
		}
		holder[index]=data;
		while(!data.empty()){
			data.pop();
		}
	    }*/
	    for(int p=0;p<size;p++){
	   	OpCodesA[oV][p]=OpCodes[p];
	   	cout<<"front at key "<<p<<" is "<<OpCodesA[oV][p]<<endl;
	   }
	}
    }
    /*for(int i=0;i<holder_size;i++){
    	cout<<"HERERERERERERERERERER.For Node with oV "<<i<<endl;
	for(int k=0;k<holder_size;k++){
		cout<<"Value at key "<<k<<" is "<<OpCodesA[i][k].front()<<endl;
	}
    }*/
    fclose(TCFile);
    return true;
}

void TCram::PrintTCram(){
    uint32_t *CoreData;
    _CoreCommon *CoreCommon;
    _LookUp *LUT;
    uint32_t PointCount;
    _DTTE *TTE;
    //Temps = new float[holder_size];
    //counters = new uint32_t[holder_size];
    float *Values,*Temps;
    uint32_t *counters;
    for(int X = 0; X< NX;X++){
        for(int Y = 0; Y < NY; Y++) {
            for (int C = 0; C < NC; C++) {
                CoreData = TCData[X][Y][C];
                if (!CoreData)
                    continue;
                printf("TCData for (X=%x, Y=%x, C=%d)\n",X,Y,C);
                CoreCommon = (_CoreCommon*)CoreData;
                printf("LUTStart = %d, LUTCount = %d\nTTStart = %d, ValuesStart = %d\n",CoreCommon->LUTStart,CoreCommon->LUTCount, CoreCommon->TTStart, CoreCommon->ValuesStart);
		//cout<<"Counter Start "<<CoreCommon->CounterStart<<" TempStart "<<CoreCommon->TempStart<<endl;
                PointCount = CoreCommon->PointCount;
                puts("LUT");
                LUT = (_LookUp*)&CoreData[CoreCommon->LUTStart];
                for(int i = 0; i < CoreCommon->LUTCount;i++)
                    printf("%x,%x  ",LUT[i].Ks,LUT[i].idx);
                puts("");
                TTE = (_DTTE*)&CoreData[CoreCommon->TTStart];
                Values = (float*)&CoreData[CoreCommon->ValuesStart];
                counters = (uint32_t*)&CoreData[CoreCommon->CounterStart];
		Temps = (float*)&CoreData[CoreCommon->TempStart];
		for(int i = 0; i < PointCount; i++){
		        //Temps[TTE[i].V]=Values[TTE[i].oV];
	      		cout<<"Specs for this node are"<<endl;
			cout<<"Kd="<<TTE[i].Kd<<",TTE OpCode="<<TTE[i].OpCode<<",Name="<<TTE[i].Name<<",oV="<<TTE[i].oV<<",YD="<<TTE[i].YD<<",Y="<<TTE[i].Y<<",X="<<TTE[i].X<<",TTE temp="<<TTE[i].temp<<",TTE counter="<<TTE[i].counter<<",IV="<<TTE[i].IV<<",Expected="<<TTE[i].Expected<<",Arrived="<<TTE[i].Arrived<<",counter="<<counters[TTE[i].oV]<<",Temps="<<Temps[TTE[i].oV]<<",Values="<<Values[TTE[i].oV]<<endl;

			cout<<"The OpCodes for this node are "<<TTE[i].V<<endl;
	      		for(int k=0;k<10;k++){
				cout<<"Value is "<<OpCodesA[TTE[i].V][k]<<endl;	
                	}
	      }
            }
        }
    }
}
