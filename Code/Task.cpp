/*
 * Task.cpp
 *
 *  Created on: Nov 25, 2012
 *      Author: alex
 */

#include "Task.h"
#include <stdio.h>
int Xsize;
unsigned Asize,Bsize;

void Task::printNodes(){

	printf("The nodes for the x vector\n");
	for(int i=0;i<Xsize;i++){
		printf("ID:%d value:%d pnode:%d pvalue:%d \n",
				XNodes[i].ID,XNodes[i].value,XNodes[i].PNode,XNodes[i].PValue);
	}
	printf("The nodes for the matrix A\n");

	for(int i=0;i<Asize*Asize;i++){
			printf("ID:%d value:%d xnode:%d xvalue:%d lastresult:%d\n",ANodes[i].ID,ANodes[i].value,
					ANodes[i].XNode,ANodes[i].Xvalue,ANodes[i].lastResult);
	}

	printf("The nodes for r are\n");
	for(int i=0;i<Bsize;i++){
		printf("ID:%d value:%d\n",RNodes[i].ID,RNodes[i].value);
		for(int f=0;f<Asize;f++){
			printf("A_ID:%d last_result:%d \n",RNodes[i].ANodeID[f],RNodes[i].lastresults[f]);
		}
	}

}

void Task::constructXNodes(int *xVector,unsigned vectorSize){
	nodeNumber=0;
	Xsize = vectorSize;
	XNode node;
	XNodes = new XNode[Xsize];
	for(int i=0;i<Xsize;i++){
		node.ID=nodeNumber;
		node.value=xVector[i];
		node.PNode=0;
		node.PValue=1;
		XNodes[i]=node;
		nodeNumber++;
	}

}

void Task::constructANodes(unsigned size,int **matrix){
	Asize=size;
	ANode node;
	ANodes = new ANode[Asize*Asize];
	unsigned counter=0;
	for(int i=0;i<Asize;i++){
		for(int j=0;j<Asize;j++){
		node.ID = nodeNumber;
		node.value = matrix[i][j];
		node.XNode =XNodes[j].ID;
		node.Xvalue = XNodes[j].value;
		node.lastResult = 0;
		ANodes[counter] = node;
		nodeNumber++;
		counter++;
		}
	}

}

void Task::constructRNodes(unsigned size,unsigned Asize,int *bvector){
	Bsize=size;
	RNode node;
	unsigned counterA=Asize;
	unsigned counter=0;
	RNodes = new RNode[Bsize];
	for(int i=0;i<Bsize;i++){
		node.ID=nodeNumber;
		node.value=bvector[i];
		node.squareValue=0;
		node.ANodeID=new unsigned[Asize];
		node.lastresults=new int[Asize];
		for(int f=counterA-Asize;f<counterA;f++){
			node.ANodeID[counter]=ANodes[f].ID;
			node.lastresults[counter]=ANodes[f].lastResult;
			counter++;
		}
		nodeNumber++;
		counterA+=Asize;
		counter=0;
		RNodes[i]=node;
	}

}
void Task::constructPNodes(unsigned size){
	PNode node;
	PNodes=new PNode[size];
	for(int i=0;i<size;i++){
		node.ID=nodeNumber;
		node.value=0;
		node.RNodeID=RNodes[i].ID;
		node.RValue=RNodes[i].value;
	}
}

int main(){
	Task t;
	const unsigned xSize=2;
	const unsigned aSize=2;
	const unsigned rsize=2;

	int vector[xSize] ={2,1};
	t.constructXNodes(vector,xSize);

	int **A;
	A = new int*[aSize];
	for(int i=0;i<aSize;i++){
		A[i] = new int[aSize];
	}
	A[0][0]=4;
	A[0][1]=1;
	A[1][0]=1;
	A[1][1]=3;
	t.constructANodes(aSize,A);

	int bvector[rsize]={1,2};
	t.constructRNodes(rsize,Asize,bvector);

	t.constructPNodes(rsize);

	t.printNodes();
}

