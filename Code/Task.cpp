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
		printf("number:%d value:%d pnode:%d pvalue:%d \n",
				XNodes[i].number,XNodes[i].value,XNodes[i].PNode,XNodes[i].PValue);
	}
	printf("The nodes for the matrix A\n");

	for(int i=0;i<Asize*Asize;i++){
			printf("number:%d value:%d xnode:%d xvalue:%d lastresult:%d\n",ANodes[i].number,ANodes[i].value,
					ANodes[i].XNode,ANodes[i].Xvalue,ANodes[i].lastResult);
	}

	printf("The nodes for r are");

	for(int i=0;i<Bsize;i++){
		printf("number:%d value:%d",RNodes[i].number,RNodes[i].value);
	}

}

void Task::constructXNodes(int *xVector,unsigned vectorSize){
	nodeNumber=0;
	Xsize = vectorSize;
	XNode node;
	XNodes = new XNode[Xsize];
	for(int i=0;i<Xsize;i++){
		node.number=nodeNumber;
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
		node.number = nodeNumber;
		node.value = matrix[i][j];
		node.XNode =XNodes[j].number;
		node.Xvalue = XNodes[j].value;
		node.lastResult = 0;
		ANodes[counter] = node;
		nodeNumber++;
		counter++;
		}
	}

}

void Task::constructRNodes(unsigned size,int *bvector){
	Bsize=size;
	RNode node;
	RNodes = new RNode[Bsize];
	for(int i=0;i<Bsize;i++){
		node.number=nodeNumber;
		node.value=bvector[i];
		node.ANodes[i] = ANodes[i].number;
		node.lastresults[i] = ANodes[i].lastResult;
		node.squareValue = 0;

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
	//t.constructRNodes(rsize,bvector);
	t.printNodes();
}

