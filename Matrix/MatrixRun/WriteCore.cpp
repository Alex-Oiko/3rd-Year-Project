//
//  WriteCore.cpp
//  Matrix
//
//  Created by Jeff Reeve on 23/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "WriteCore.h"
const unsigned CoreMask = 0xfffff800;
const unsigned ChipMask = 0xffff0000;
void WriteCore::BinaryDump(string RootName, Task& TASK, Dealer& DEAL, Core& CORE, MakeMCTables& MCT){
    unsigned oKey = (-1)&CoreMask, Key, oChip = (-1)&ChipMask, CoreKey, ChipKey;
    uint32_t NoUsedChips, CoreCount;
    uint32_t *memal;
    const char *TCram;
    const uint32_t StartAddress = 0x0401000;
    uint32_t Rtype = 0x71, XY, CoreID, WordCount;
    _CoreCommon HeadCore;
    unsigned HeadSize;
    HeadSize = (sizeof(_CoreCommon));
    TTArray = NULL;
    Values = NULL;
    LUTSize = NULL;
    FILE *Results;
    
    TCram = (RootName+".SPINNjram").c_str();
    Results = fopen(TCram,"wb");
    if(!Results){
        printf("couldn't open %s file matey\n",TCram);
        return;
    }
    fwrite(&Rtype,1,1,Results);
    Rtype = 0;
    fwrite(&Rtype,1,3,Results);
    puts("TC RAM sizes");
    NoUsedChips = UsedChips(DEAL);
    fwrite(&NoUsedChips, 4, 1, Results);
    for (auto iKey=DEAL.PMAP.cbegin(); iKey!=DEAL.PMAP.cend(); iKey++) {
        Key = iKey->first;
        ChipKey = Key&ChipMask;
        if((Key&ChipMask) != oChip){    //once per Chip
            oChip = (Key&ChipMask);
            CoreCount = UsedCores(Key,DEAL);
            if( CoreCount == 0)
                continue;
            XY = (uint32_t)Key>>16;
            fwrite(&XY, 2, 1, Results);
            fwrite(&CoreCount, 2, 1, Results);
            for(auto iCore=iKey; iCore!= DEAL.PMAP.cend(); iCore++){ //once per core
                Key = iCore->first;
                
                if ((Key&ChipMask)>ChipKey) 
                    break;                      //don't go past current chip
                CoreKey = Key&CoreMask;
                if (CoreKey != oKey) {
                    DumpCore(Key, CORE, DEAL, TASK);
                    oKey = CoreKey;
                    CoreID = ((Key&0x0000ffff)>>11) + 1;   //?
                    fwrite(&StartAddress, 4, 1, Results);
                    fwrite(&CoreID,4,1,Results);
                    WordCount = LUTSize*sizeof(_LookUp) + 4*PointCount + PointCount*sizeof(_DTTE)+HeadSize;
                    WordCount = (WordCount>>2);
                    fwrite(&WordCount, 4, 1, Results);
                    HeadCore.PointCount = PointCount;
                    HeadCore.LUTStart = HeadSize>>2;
                    HeadCore.LUTCount = LUTSize;
                    HeadCore.TTStart = (HeadSize + (LUTSize*sizeof(_LookUp)))>>2;
                    HeadCore.ValuesStart = (HeadSize + LUTSize*sizeof(_LookUp) + PointCount*sizeof(_DTTE))>>2;
                    fwrite(&HeadCore, HeadSize, 1, Results);
                    fwrite(LUT,sizeof(_LookUp), LUTSize, Results);
                    fwrite(TTArray, sizeof(_DTTE), PointCount, Results);
                    fwrite(Values, sizeof(float), PointCount, Results);
                    fwrite(Temps, sizeof(float), PointCount, Results);
                    fwrite(counters, sizeof(uint32_t), PointCount, Results);
                    fwrite(Fronts,sizeof(uint32_t),PointCount, Results);
		    fwrite(OpCodesA, sizeof(memal), PointCount, Results);
		}
            }

        }
    }
    fclose(Results);
}

unsigned WriteCore::UsedCores(unsigned Key, Dealer& DEAL){
    unsigned  used = 0, Keyo, CoreKey;
    auto iP = DEAL.PMAP.find(Key);
    CoreKey = Key&ChipMask;
    for(; iP!=DEAL.PMAP.cend();iP++){
        Keyo = (iP->first)&ChipMask;
        if(CoreKey == Keyo){
            used++;
        }
        else if(Keyo > CoreKey)
            break;
    }
    return used;
}

unsigned WriteCore::UsedChips( Dealer& DEAL){
    unsigned  used = 0, Key, Keyo = -1;
    for(auto iP=DEAL.PMAP.cbegin(); iP!=DEAL.PMAP.cend();iP++){
        Key = (iP->first)&ChipMask;
        if(Key != Keyo){
            used++;
            Keyo = Key;
        }
    }
    return used;
}

//get the TCRAM data for one core.
void WriteCore::DumpCore(unsigned Key, Core& CORE, Dealer& DEAL, Task& TASK){
    unsigned Kd = Key, Kt, idx = 0;
    multimap<unsigned, unsigned> TMPLUT;
    if(TTArray != NULL) {
        delete[] TTArray;
        delete[] Values;
        delete[] LUT;
    }
    vector<_DTTE>   vTargetTable;
    _DTTE dTTE;
    TargetTableEntry TTE;
    Key = Key&CoreMask;
    auto iKt = DEAL.PMAP.find(Kd);
    for(; iKt!=DEAL.PMAP.cend(); iKt++){
        Kt = iKt->first;
        if ((Kt&CoreMask) > Key)        //PMAP in Key order
            break;
        //deal with core entries
        auto iTTE = CORE.CoreEntries.find(Kt);
        TTE = iTTE->second;
	dTTE.Name = TTE.Name[0];
        dTTE.Kd = TTE.Kd;
        dTTE.OpCodes = TTE.OpCodes;
	dTTE.IV = TTE.IV;
        dTTE.oV = TTE.V;
	dTTE.YD = TTE.YD;
	dTTE.X = TTE.X;
	dTTE.Y = TTE.Y;
	dTTE.ArSize = TTE.OpCodes.size();
	dTTE.front=0;
	dTTE.counter = TTE.counter;
	dTTE.Temp = TTE.Temp;
        dTTE.Expected = 0;
        dTTE.Arrived = 0;

        //now deal with the lookup table
        auto iKdKs = TASK.KdKs.find(Kt);
        for(;iKdKs!= TASK.KdKs.cend();iKdKs++){
            if(iKdKs->first > Kt)
                break;
            TMPLUT.insert({iKdKs->second,idx});
            dTTE.Expected++;
        }
        vTargetTable.push_back(dTTE);
        idx++;
    }
    PointCount = idx;
    TTArray = new _DTTE[PointCount];
    Values = new float[PointCount];
    OpCodesA = new queue<uint32_t>[PointCount];
    for(idx = 0; idx < PointCount; idx++){
	TTArray[idx] = vTargetTable[idx];
        Values[idx] = TTArray[idx].IV;
        Temps[idx] = TTArray[idx].Temp;
        counters[idx] = TTArray[idx].counter;
	cout<<TTE.Name[0]<<endl;
	for(int i=0;i<TTArray[idx].OpCodes.size();i++){
		uint32_t fr=TTArray[idx].OpCodes.front();
		OpCodesA[idx].push(fr);
		TTArray[idx].OpCodes.pop();
		TTArray[idx].OpCodes.push(fr);
	}
	TTArray[idx].oV = idx;    }
    LUTSize = (unsigned)TMPLUT.size();
    LUT = new _LookUp[LUTSize];
    _LookUp tLUT;
    idx = 0;
    for(auto iTMPLUT = TMPLUT.cbegin(); iTMPLUT!= TMPLUT.cend(); iTMPLUT++){
        tLUT.Ks = iTMPLUT->first;
        tLUT.idx = iTMPLUT->second;
        LUT[idx] = tLUT;
        idx++;
    }
}
