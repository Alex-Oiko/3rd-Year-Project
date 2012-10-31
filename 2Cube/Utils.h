//
//  Utils.h
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#ifndef TimeMarch_Utils_h
#define TimeMarch_Utils_h
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
#endif
