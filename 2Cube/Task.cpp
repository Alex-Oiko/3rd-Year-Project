//
//  Task.cpp
//  TimeMarch
//
//  Created by Jeff Reeve on 13/08/2012.
//  Copyright (c) 2012 ecs. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Task.h"
/*
 note to all.
 The unsigned arithmetic is a 32 bit representation ie scaling by 2^31
 all real numbers are between 0 and 1
 */
#define BUFFER 32768

static int p;
int Task::GetNextInt(){
	int l=0;
	while(!isdigit(buffer[p++]));
	p--;
	while(isdigit(buffer[p])){
		number[l++] = buffer[p++];
	}
	number[l] = '\0';
	return(atoi(number));
}

void Task::GetLine(){
	int c,i = 0;
	p=0;
	while(c != EOF) {
		c = fgetc(TaskFile);
		buffer[i++] = c;
		if(c == '\n' || c == EOF){
			buffer[i-1] = '\0';
			return;
		}
	}
}


void Task::PrintNodeList(){
    int i,j,V;
    Node *Next;
    for(i=0;i<Nodes;i++){
        Next = &NodeList[i];
        V = Next->Valency;       
        printf("%d: T=%u B=%d V=%d ",i,Next->Value,Next->Fixed,V);
        for(j=0;j<V;j++)
            printf("%u %u %u ",Next->Nodes[j],Next->Weights[j],Next->Values[j]);    
        puts("");       
    }
}

void Task::ReadNodeList(const char *GridFile){
    int i,c,V;
    Node *Next;
	TaskFile = fopen(GridFile,"r");
	if(!TaskFile){
	    printf("Couldn't open %s file\n",GridFile);
	    exit(0);
	}
    p=0;
	GetLine();
	Nodes = GetNextInt();
	GetNextInt();  //the maximum valency that can be skipped
    NodeList = new Node[Nodes];
	for(i=0;i<Nodes;i++){
		GetLine();
		GetNextInt();  /*don't need the node number */	
        Next = &NodeList[i];
		Next->Value = GetNextInt();
		Next->Fixed = GetNextInt();
        if(Next->Fixed)
            Next->Fixed = 1;
        V = GetNextInt();
		Next->Valency = V;
        
        Next->Nodes = new unsigned[V];
        Next->Weights = new unsigned[V];
        Next->Values = new unsigned[V];        
        Next->Arrived = GetNextInt();
		for(c=0;c<V;c++){
			Next->Nodes[c] = GetNextInt();      //node number   
			Next->Weights[c] = GetNextInt();    //weight        
			Next->Values[c] = GetNextInt();     //temperature?   	            
		}
	}
	fclose(TaskFile);
    return;
    
}
void Task::FillKeyMaps(Dealer DEAL){
    int i,j,V;
    Node *Next;
    unsigned Kd,Ks;
    for(i=0;i<Nodes;i++){
        Next = &NodeList[i];
        Kd = DEAL.MAP[i];
        V = Next->Valency;
        for(j=0;j<V;j++){
            Ks = DEAL.MAP[Next->Nodes[j]];
            KdKs.insert(multimap<unsigned, unsigned>::value_type(Kd,Ks));
            KsKd.insert(multimap<unsigned, unsigned>::value_type(Ks,Kd));
        }    
    }
    
}


void Task::PrintKeyMaps(){
    multimap<unsigned,unsigned>::iterator KDKS;
    printf("The MAP of Kd to Ks \n");
    for(KDKS = KdKs.begin(); KDKS != KdKs.end(); KDKS++){
        printf("%x %x\n",KDKS->first,KDKS->second);
    }
    
}



