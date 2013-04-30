//
//  Utils.h
//  MatrixRun
//
//  Created by Jeff Reeve on 25/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef MatrixRun_Utils_h
#define MatrixRun_Utils_h


using namespace std;

static void KeyTo(unsigned,unsigned&, unsigned&, unsigned&, unsigned&);
static unsigned KeyFrom(unsigned, unsigned, unsigned, unsigned);
static void KeyTo(unsigned Key,unsigned& X, unsigned& Y, unsigned& C, unsigned& O){
    X = Key>>24;
    Y = (Key>>16)&0x0ff;
    C = (Key >> 11)&0x0f;
    O = (Key&0x07FF);
}
static unsigned KeyFrom(unsigned X, unsigned Y, unsigned C, unsigned O){
    return (unsigned)((X<<24)+(Y<<16)+(C<<11)+O);
}

struct TargetTableEntry{        //There are CoreData.NodesOnCore of these
    unsigned Kd;                //identifies this particular node (Key actually)
    unsigned OpCode;            //identifies what function to use on MC message arrival
    char Name[4];              //Name of the node as from the input file
    unsigned XD;                //X dimension of the matrix element
    unsigned YD;                //Y dimension of the matrix element
    unsigned X;                 //X position of element Name[X][Y]
    unsigned Y;                 //X position of element Name[X][Y]
    float IV;                //initial value
    unsigned V;                //array offset pointer to my value = pid for simulation here.
};


#endif
