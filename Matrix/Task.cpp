//
//  Task.cpp
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include "Task.h"
/*
Task::Task(){

}
Task::Task(const Task&){
    
}//Copy constructor. Why you might ask!
Task::~Task(){
    
}
 */
void Task::lprint(){
    cout << "line "<<lineNumber<<":"<<inLine<<endl;
}
bool Task::readTask(string inFile){
    bool good = false;
    nextPID = 0;
    inFile = inFile + ".mx";
    inTask.open(inFile);
    if(inTask.fail()){
        cerr << "failed to open "+inFile<< endl;
        return false;
    }
    lineNumber = 0;
    good = parseTask();
    if(!good)
        cerr << "error on line number " << lineNumber << endl;
    inTask.close();
    CoreFiller();
    return good;
}
bool Task::parseTask(){
    state STATE = BEGIN_VALUES;
    while(STATE != END && STATE != ERROR){
        getline(inTask,inLine);
        lprint();
        lineNumber++;
        STATE = interpret(STATE);
    }
    if(STATE == END)
        return true;
    return false;
}

Task::state Task::interpret(state State){
    state newState = END;
    string::size_type idx;
    bool found = false;
    switch (State){
        case BEGIN_VALUES:
        {
            idx = inLine.find("BEGIN VALUES");
            if(idx == string::npos)
                newState = ERROR;
            else
                newState  = END_VALUES;
        }
            break;
        case END_VALUES:
        {
            idx = inLine.find("END VALUES");
            if(idx == string::npos){
                found = readMatrix();
                newState = found?END_VALUES:ERROR;
            }
            else
                newState  = BEGIN_OVERLAYS;
        }
            break;
        case BEGIN_OVERLAYS:
        {
            idx = inLine.find("BEGIN OVERLAYS");
            if(idx == string::npos)
                newState = ERROR;
            else
                newState  = END_OVERLAYS;
        }
            break;
        case END_OVERLAYS:
        {
            idx = inLine.find("END OVERLAYS");
            if(idx == string::npos){
                found = readOverlay();
                newState = found?END_OVERLAYS:ERROR;
            }
            else
                newState  = BEGIN_CONNECTIONS;
        }
            break;
        case BEGIN_CONNECTIONS:
        {
            idx = inLine.find("BEGIN CONNECTIONS");
            if(idx == string::npos)
                newState = ERROR;
            else
                newState  = END_CONNECTIONS;
        }
            break;
        case END_CONNECTIONS:
        {
            idx = inLine.find("END CONNECTIONS");
            if(idx == string::npos){
                found = readConnection();
                newState = found?END_CONNECTIONS:ERROR;
            }
            else
                newState  = END;
        }
            break;
            
        case END:
        {
            newState  = END;
        }
            break;
        case ERROR:
        {
            newState  = ERROR;
        }
            break;
            
            
    }
    return newState;
}

bool Task::readMatrix(){
    string::size_type idx, sp;
    string name;
    string NewLine, sOpCode;
    unsigned OpCode;
    
    vector<float> fpValues;
    vector<string> SubStrings;
    //vector<float>::iterator iFP;
    dimension vType = ROGUE;
    float **matrix;
    bool good = false;
    unsigned D1=1,D2=1;
    split(inLine, ' ', SubStrings);
    NewLine = SubStrings[0];
    sOpCode = SubStrings[1];
    OpCode = stoi(sOpCode);

    idx = NewLine.find('[');
    name = NewLine.substr(0,idx);    
    OpCodes.insert(map<string, unsigned>::value_type(name,OpCode)); 
    if(idx == string::npos){ // must be a scalar
        vType = SCALAR;
    }
    else {
        sp = idx;
        idx = NewLine.find(',');
        if(idx != string::npos){    //must be a 2D matrix
            vType = MATRIX2;
            D1 = stoi(NewLine.substr(sp+1,idx-1));
            sp = idx;
            idx = NewLine.find(']');
            D2 = stoi(NewLine.substr(sp+1,idx-1));
        }
        else{                       //must be a vector
            vType = VECTOR;
            idx = NewLine.find(']');
            D2 = stoi(NewLine.substr(sp+1,idx-1));
        }
    }
    good = allocatePIDs(name, D1, D2);
    if(!good)
        return good;
    good = readFloats(fpValues,D1);
    if(!good)
        return good;
    if(fpValues.size() == 0)
        good = false;
    else {
        makeMatrix(name, D1, D2);
        matrix = mStore[name];
        for(int i=0,l=0;i<D1;i++)
            for(int j=0;j<D2;j++,l++)
                matrix[i][j] = fpValues[l];
        cout << "read values for dimension "<<vType<<" "<< name << endl;
        for(auto iFP = fpValues.cbegin(); iFP != fpValues.cend(); iFP++)
            cout << *iFP << " ";
        cout << endl;
    }
    
    return good;
}

void Task::makeMatrix(string name, unsigned D1, unsigned D2){
    float **matrix = new float*[D1];
    for(int i=0;i<D1;i++)
        matrix[i] = new float[D2];
    mStore.insert(map<string, float**>::value_type(name,matrix));
    return;
}

bool Task::readFloats(vector<float> &fpValues, int D1){
    float fpvalue;
    //vector<string>::iterator iS;
    vector<string> subFields;
    int line;
    for(line = 0; line < D1; line++){
        getline(inTask,inLine);
        lprint();
        lineNumber++;
        subFields.clear();
        split(inLine,' ',subFields);
        for(auto iS = subFields.cbegin(); iS != subFields.cend(); iS++) {
            try {
                fpvalue = stof(*iS);
                fpValues.push_back(fpvalue);
            } catch (exception &e) {
                cerr << "exception "<<e.what()<<endl;
                return false;
            }
            
        }
    }
    return true;
}

void Task::split(const string& s, char c, vector<string>& vs ){
    string::size_type i = 0;
    string::size_type j = s.find(c);
    if(j == string::npos){
        vs.push_back(s);
        return;
    }
    while (j != string::npos){
        vs.push_back(s.substr(i,j-i));
        i = ++j;
        j = s.find(c,j);
        if(j == string::npos)
            vs.push_back(s.substr(i,s.length()));
    }
}

bool Task::allocatePIDs(string name, unsigned D1, unsigned D2){
    //map<string,unsigned>::iterator iName;
    unsigned size;
    auto iName = pidStart.find(name);
    if(iName != pidStart.end()){
        cerr << "name "<<name<<" is duplicated"<<endl;
        return false;
    }
    pidStart.insert(map<string,unsigned>::value_type(name,nextPID));
    pidName.insert(map<unsigned,string>::value_type(nextPID,name));
    size = D1*D2;
    nextPID += size;
    nameSize.insert(map<string,unsigned>::value_type(name,size));
    nameD1.insert(map<string,unsigned>::value_type(name,D1));
    nameD2.insert(map<string,unsigned>::value_type(name,D2));
    return true;
}

bool Task::pidFromName(string element, unsigned& pid){
    string::size_type idx, sp;
    string name;
    unsigned i,j,D2=0, D1=0,basePID, scale;
    //map<string, unsigned>::iterator iPID;
    idx = element.find('[');
    name = element.substr(0,idx);
    auto iPID = nameD1.find(name);
    D1 = iPID->second;
    iPID = nameD2.find(name);
    D2 = iPID->second;
    iPID = pidStart.find(name);
    
    if(iPID == pidStart.end()){
        cerr << name <<" not a variable in string "<<element<<" at line:"<< lineNumber << endl;
        return false;
    }
    else
        basePID = iPID->second;
    if(D1==1 && D2==1){ // must be a scalar
        pid = basePID;
        return true;
    }
    else {
        sp = idx;
        idx = element.find(',');
        if(idx != string::npos){    //must be a 2D matrix
            i = stoi(element.substr(sp+1,idx-1));
            sp = idx;
            idx = element.find(']');
            j = stoi(element.substr(sp+1,idx-1));
            
            scale = (D1>D2)?D1:D2;
            pid = basePID + i*scale + j;
            return true;
        }
        else  {                       //must be a vector
            idx = element.find(']');
            j = stoi(element.substr(sp+1,idx-1));
            pid = basePID + j;
            return true;
        }
        
    }
    return false;
}

bool Task::readOverlay(){
    vector<string> vs;
    unsigned D1,D2;
    bool good = true;
    //map<string,unsigned>::iterator iPID;
    string nameNew, nameOld;
    split(inLine, '=', vs);
    cerr << "overlaying "<<vs[1]<<" and "<<vs[0]<<endl;
    if(vs.size() == 0){
        cerr << "error reading overlays line: "<<lineNumber<<endl;
        return false;
    }
    nameOld = vs[1];
    auto iPID = pidStart.find(nameOld);
    if(iPID == pidStart.end()){
        cerr << "undeclared variable "<<nameOld<<" on line: "<<lineNumber<<endl;
        return false;
    }
    nameNew = vs[0];
    D1 = nameD1[nameOld];
    D2 = nameD2[nameOld];
    good = allocatePIDs(nameNew, D1, D2);
    if(!good){
        cerr<<"failed to allocate PID to "<<nameNew<<" line:"<<lineNumber<<endl;
    }
    overlays21.insert(map<string,string>::value_type(nameNew,nameOld));
    overlays12.insert(map<string,string>::value_type(nameOld,nameNew));
    return good;
}

bool Task::readConnection(){
    string::size_type idx;
    string LHS,RHS;
    bool good = true;
    vector<string> vs;
    unsigned pidLHS, pidRHS;
    //vector<string>::iterator iVS;
    idx = inLine.find('>');
    good = !(idx == string::npos);
    if(!good){
        cerr << "invalid LHS in CONNECTIONS line:"<<lineNumber<<endl;
        return good;
    }
    LHS = inLine.substr(0,idx);
    good = pidFromName(LHS, pidLHS);
    if(!good)
        return good;
    RHS = inLine.substr(idx+1,inLine.length());
    split(RHS,' ', vs);
    good = !(vs.size()==0);
    if(!good){
        cerr << "invalid RHS in CONNECTIONS line:"<<lineNumber<<endl;
        return good;
    }
    for(auto iVS = vs.cbegin(); iVS != vs.cend(); iVS++){
        good = pidFromName(*iVS, pidRHS);
        if(!good){
            cerr << "invalid LHS in CONNECTIONS line:"<<lineNumber<<endl;
            return good;
        }
        Connections.insert(multimap<unsigned, unsigned>::value_type(pidLHS,pidRHS));
        
    }
    return good;
}

void Task::printConnections() {
    //multimap<unsigned, unsigned>::iterator iTask;
    unsigned lhs = 0xffffffff;
    cout << "List of Connections";
    for (auto iTask = Connections.begin(); iTask != Connections.end(); iTask++) {
         if(lhs != iTask->first){
            lhs = iTask->first;
            cout << endl<< elementFromPID(iTask->first)<<" --> ";
        }
        cout <<elementFromPID(iTask->second) << " ";
    }
    cout << endl;
}

string Task::elementFromPID(const unsigned pid){
    map<string,unsigned>::iterator iPIDs,iSize, iD1, iD2;
    stringstream ss;
    unsigned D1=0,D2=0,scale, i,j,k;
    string name;
    for(iPIDs = pidStart.begin(), iSize = nameSize.begin(), iD2 = nameD2.begin(), iD1 = nameD1.begin();iPIDs != pidStart.cend(); iPIDs++,iSize++,iD2++,iD1++){
        if((pid >= iPIDs->second) && (pid < iPIDs->second+iSize->second)){
            D1 = iD1->second;
            D2 = iD2->second;
            name = iPIDs->first;
            break;
        }
    }
    scale = (D1>D2)?D1:D2;
    if (D1==1 && D2 == 1) {      //this is a scalar
        ss<<name;
    }
    else if(D1 == 1){        //this is a vector
        i = pid - iPIDs->second;
        ss << name<<"["<<i<<"]";
    }
    else {
        k = pid - iPIDs->second;
        i = k/scale;
        j = k%scale;
        ss<<name<<"["<<i<<","<<j<<"]";
    }
    return ss.str();
}
void Task::printMatrixValues(){
    //map<string, float**>::iterator iMat;
    string name;
    float**matrix;
    unsigned D1,D2,i,j;
    for(auto iMat = mStore.begin(); iMat != mStore.end(); iMat++){
        name = iMat->first;
        matrix = iMat->second;
        cout << "values of "<<name<<endl;
        D1 = nameD1[name];
        D2 = nameD2[name];
        for(i=0;i<D1;i++){
            for(j=0;j<D2;j++)
                cout << matrix[i][j]<<" ";
            cout << endl;
        }
    }
}

void Task::FillKeyMaps(Dealer& DEAL){
    multimap<unsigned,unsigned>::iterator iCon;
    unsigned Kd,Ks;
    for(iCon = Connections.begin(); iCon != Connections.end(); iCon++){
        Ks = DEAL.MAP[iCon->first];
        Kd = DEAL.MAP[iCon->second];
        KsKd.insert(multimap<unsigned, unsigned>::value_type(Ks,Kd));
    }
}

void Task::CoreFiller(){
    string name,oname;
    int XD,YD;
    float** M;
    unsigned pid = 0;
    int i;
    unsigned long namesize;
    TargetTableEntry CoreEntry, OCoreEntry;
    map<string, float**>::iterator iDF;
    multimap<string, TargetTableEntry>::iterator iTTE;
    CoreEntry.Kd = 0;
    CoreEntry.V = NULL;
    for (auto iS = pidName.cbegin();iS != pidName.cend(); iS++) {
        name = iS->second;
        namesize = name.size();
        auto iO = overlays21.find(name);
        if (iO != overlays21.cend()) continue;  //store all original declarations first
        for(i = 0;i < 4 && i < namesize; i++)
            CoreEntry.Name[i] = name[i];
        for(;i<4;i++)
            CoreEntry.Name[i] = '\0';
        auto iOp = OpCodes.find(name);
        CoreEntry.OpCode = iOp->second;
        auto iD1 = nameD1.find(name);
        XD = iD1->second;
        CoreEntry.XD = XD;
        auto iD2 = nameD2.find(name);
        YD = iD2->second;
        CoreEntry.YD = YD;            
        iDF = mStore.find(name);
        M = iDF->second;
        for(unsigned x=0;x<XD;x++){
            CoreEntry.X = x;
            for(unsigned y=0;y<YD;y++){
                CoreEntry.Y = y;
                CoreEntry.IV = M[x][y];
                CoreEntry.V = pid++;
                CoreFill.insert(multimap<string, TargetTableEntry>::value_type(name,CoreEntry));
                CoreFlix.push_back(CoreEntry);
            }
        }
    }
    DataSize = pid;
    //now handle overlays
    for (auto iS = pidName.cbegin();iS != pidName.cend(); iS++) {
        name = iS->second;
        auto iO = overlays21.find(name);
        if (iO == overlays21.cend()) continue;  //if not an overlay not interested
        oname = iO->second;
        iTTE = CoreFill.find(oname);
        for (; iTTE != CoreFill.cend(); iTTE++) {
            if(iTTE->first != oname)
                break;
            OCoreEntry = iTTE->second;
            CoreEntry = OCoreEntry;
            CoreEntry.Name[0] = name[0];
            CoreEntry.Name[1] = name[1];
            CoreEntry.Name[2] = name[2];
            CoreEntry.Name[3] = name[3];
            CoreFill.insert(multimap<string, TargetTableEntry>::value_type(name,CoreEntry));
            CoreFlix.push_back(CoreEntry);
        }
    }
}
