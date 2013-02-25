//
//  Cores.cpp
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//
#include <iostream>
#include <iomanip>
#include "Cores.h"

using namespace std;
void Core::MakeLUT(Task& TASK){
    unsigned Ks = -1;
    vector<unsigned> vLUT;
    for(auto iC = TASK.KsKd.cbegin(); iC != TASK.KsKd.end();iC++){
        if (Ks != iC->first) {
            if(Ks != -1)
                LUT.insert(map<unsigned, vector<unsigned>>::value_type(Ks,vLUT));
            vLUT.clear();
            Ks = iC->first;
        }
        vLUT.push_back(iC->second);
    }
    if (vLUT.size() != 0) {
        LUT.insert(map<unsigned, vector<unsigned>>::value_type(Ks,vLUT));          
    }
}

void Core::PrintTTE(TargetTableEntry& TTE){
    string sName = TTE.Name;
    cout << "{K:"<<hex<<TTE.Kd<<" "<<sName<< dec <<" OpCode "<< TTE.OpCode<< " XD:"<<TTE.XD<<" YD:"<<TTE.YD<<" X:"<<TTE.X<<" Y:"<<TTE.Y<<" IV:"<< TTE.IV <<" O:" <<TTE.V << "}\n";
}

void Core::PrintByOpCode(unsigned OpCode){
    TargetTableEntry TTE;
    unsigned X,Y,C,O;
    for(auto iTTE = CoreEntries.cbegin(); iTTE!=CoreEntries.cend();iTTE++){
        TTE = iTTE->second;
        if(TTE.OpCode != OpCode) continue;
        string sName = TTE.Name;
        KeyTo(TTE.Kd, X, Y, C, O);
        float value = Mstore[TTE.V];
        cout << "{K:"<<hex<<TTE.Kd<<" "<<sName<<"[" <<dec<<TTE.X<<"]["<<TTE.Y<<"]="<<value <<"}\n";
    }
}



void Core::PrintLUT(){
    cout.setf(ios::hex);
    cout << "Look up Tables\n";
    for(auto iL = LUT.cbegin(); iL != LUT.cend(); iL++){
        cout << "Source Key: "<< hex << iL->first<< " ";
        auto iS = CoreEntries.find(iL->first);
        PrintTTE(iS->second);
        cout << "\nDestination Keys: ";
        for(int v = 0; v < iL->second.size(); v++){
            cout << hex << iL->second[v]<< " ";
            auto iC = CoreEntries.find(iL->second[v]);
            if(iC == CoreEntries.cend()){
                cerr << "No Core Entry for Ks " << hex << iL->second[v]<<endl;
                exit(1);
            }
            PrintTTE(iC->second);
        }
        cout << endl;
    }
}

void Core::AllocateStorage(Task& TASK){
    TargetTableEntry TTE;
    //firstly create the storage needed
    for(int i = 0; i < TASK.nextPID; i++)
        Mstore.push_back((float)-1.0);
 
    for(auto iS = CoreEntries.cbegin(); iS != CoreEntries.cend(); iS++){
        TTE = iS->second;
        Mstore[TTE.V] = TTE.IV;
    }

}











