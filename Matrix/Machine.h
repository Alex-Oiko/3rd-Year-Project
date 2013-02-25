//
//  Machine.h
//  Matrix
//
//  Created by Jeff Reeve on 05/01/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#ifndef Matrix_Machine_h
#define Matrix_Machine_h
#include <map>
#include <vector>
#include <iostream>
using namespace std;

class Machine{
private:
    int Neighbour(const int ,const int ,const int )const ;
    int UNeighbour(const int ,const int ,const int ) const ;
    int NeighbourWrap(const int ,const int ,const int ) const;
    int ConvertXY(const int,const int) const;
    void RevertXY(const int, int&, int&)const ;
    void B48Revert(const int, int&, int&) const;

public:
    int NX, NY;
    ~Machine();
    Machine(int, int, int, int);
    unsigned ChipCount;
    vector<vector<unsigned>> ChipLink;  //ChipLink[Chip][l] = Attached chip to Chip from link l
    vector<unsigned>    ChipXY;                 //ChipXY[Chip] = XY value of Chip (X<<8 || Y)
    map<unsigned,unsigned> XYChip;            //XYChip[XY] = Chip - inverse of above
};




#endif
