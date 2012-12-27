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

	printf("rsnew node is\n");
	printf("ID:%d value:%d\n",rsnew.ID,rsnew.value);
	printf("The r nodes saved in rsnew are\n");
	for(int i=0;i<Xsize;i++){
		printf("r_ID:%d\n",rsnew.Rnodes[i]);
	}

	printf("rsold node is\n");
	printf("ID:%d value:%d rsew_address:%d\n",rsold.ID,rsold.value,rsold.rsNew);
	printf("The r nodes saved in rsnew are\n");
	for(int i=0;i<Xsize;i++){
		printf("r_ID:%d\n",rsold.RNodes[i]);
	}

	printf("alpha_denom node is\n");
	printf("ID:%d value:%d\n",alpha_denom.ID,alpha_denom.value);
	printf("Ap id's in alpha_denom are\n");
	for(int i=0;i<Xsize;i++){
		printf("Ap_ID:%d\n",alpha_denom.Ap_addresses[i]);
	}

	printf("alpha node is\n");
	printf("ID:%d value:%d rsoldID:%d alpha_denomID:%d\n",alpha.ID,alpha.value,alpha.rsoldID,alpha.alpha_denomID);

	printf("The nodes for the p vector\n");
	for(int i=0;i<Xsize;i++){
		printf("ID:%d value:%d alpha:%d alphaValue:%d rsold:%d rsnew:%d rsold/rsnew:%d rnode:%d\n",
				PNodes[i].ID,PNodes[i].value,PNodes[i].alpha,PNodes[i].alphaValue,PNodes[i].rsold,
				PNodes[i].rsnew,PNodes[i].rson,PNodes[i].RNodeID);
	}

	printf("The nodes for the Ap vector are\n");
	for(int i=0;i<Xsize;i++){
		printf("ID:%d value:%d alpha:%d pNode:%d last_result:%d\n",ApNodes[i].ID,ApNodes[i].value,
				ApNodes[i].alpha,ApNodes[i].pNode,ApNodes[i].last_result);
	}
	printf("The corresponding A nodes that go into Ap are\n");
	for(int i=0;i<Xsize;i++){
		printf("ID:%d\n",ApNodes[i].ID);
		for(int f=0;f<Xsize;f++){
			printf("A_ID:%d\n",ApNodes[i].Anodes[f]);
		}
	}

	printf("The nodes for the x vector\n");
	for(int i=0;i<Xsize;i++){
		printf("ID:%d value:%d pnode:%d pvalue:%d \n",
				XNodes[i].ID,XNodes[i].value,XNodes[i].PNode,XNodes[i].PValue);
	}
	printf("The nodes for the matrix A\n");

	for(int i=0;i<Asize*Asize;i++){
			printf("ID:%d value:%d xnode:%d xvalue:%d pNode:%d lastresult:%d\n",ANodes[i].ID,ANodes[i].value,
					ANodes[i].XNode,ANodes[i].Xvalue,ANodes[i].PNode,ANodes[i].lastResult);
	}

	printf("The nodes for r are\n");
	for(int i=0;i<Bsize;i++){
		printf("ID:%d value:%d ApNodeID:%d\n",RNodes[i].ID,RNodes[i].value,RNodes[i].ApNodeID);
		for(int f=0;f<Asize;f++){
			printf("A_ID:%d last_result:%d\n",RNodes[i].ANodeID[f],RNodes[i].lastresults[f]);
		}
	}

}

void Task::constructXNodes(int *xVector,unsigned vectorSize){
	Xsize = vectorSize;
	XNode node;
	XNodes = new XNode[Xsize];
	for(int i=0;i<Xsize;i++){
		node.ID=nodeNumber;
		node.value=xVector[i];
		node.PNode=PNodes[i].ID;
		node.PValue=PNodes[i].value;
		XNodes[i]=node;
		nodeNumber++;
	}

}

void Task::constructANodes(unsigned size,int **matrix){
	Asize=size;
	ANode node;
	ANodes = new ANode[Asize*Asize];
	unsigned counter=0;
	for(unsigned i=0;i<Asize;i++){
		for(unsigned j=0;j<Asize;j++){
		node.ID = nodeNumber;
		node.value = matrix[i][j];
		node.XNode =XNodes[j].ID;
		node.Xvalue = XNodes[j].value;
		node.PNode=PNodes[j].ID;
		node.lastResult = 0;
		ApNodes[i].Anodes[j]=node.ID;
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
	rsold.RNodes=new unsigned[Bsize];
	rsnew.Rnodes=new unsigned[Bsize];
	for(int i=0;i<Bsize;i++){
		node.ID=nodeNumber;
		node.value=bvector[i];
		node.squareValue=0;
		node.ANodeID=new unsigned[Asize];
		node.lastresults=new int[Asize];
		node.ApNodeID=ApNodes[i].ID;
		for(int f=counterA-Asize;f<counterA;f++){
			node.ANodeID[counter]=ANodes[f].ID;
			node.lastresults[counter]=ANodes[f].lastResult;
			counter++;
		}
		rsold.RNodes[i]=node.ID;
		rsnew.Rnodes[i]=node.ID;
		PNodes[i].RNodeID=node.ID;
		PNodes[i].RValue=node.value;
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
		node.ID=nodeNumber++;
		node.value=0;
		node.alpha=alpha.ID;
		node.alphaValue=alpha.value;
		node.rsnew=rsnew.ID;
		node.rsold=rsold.ID;
		node.rson=0;
		PNodes[i]=node;
	}
}

void Task::constructRSNewNode(){
	nodeNumber=0;
	rsnew.ID=nodeNumber++;
	rsnew.value=0;

}
void Task::constructRSOldNode(){
	rsold.ID=nodeNumber++;
	rsold.value=0;
	rsold.rsNew=rsnew.ID;
}

void Task::constructAlphaDenomNode(){
	alpha_denom.ID=nodeNumber++;
	alpha_denom.value=0;
}

void Task::constructAlphaNode(){
	alpha.ID=nodeNumber++;
	alpha.value=0;
	alpha.rsoldID=rsold.ID;
	alpha.alpha_denomID=alpha_denom.ID;
}

void Task::constructApNodes(unsigned size){
	ApNode node;
	ApNodes = new ApNode[size];
	alpha_denom.Ap_addresses=new unsigned[size];
	for(int i=0;i<size;i++){
		node.ID=nodeNumber++;
		node.value=0;
		node.alpha=alpha.ID;
		node.last_result=0;
		node.pNode=PNodes[i].ID;
		node.Anodes=new unsigned[size];
		ApNodes[i]=node;
		alpha_denom.Ap_addresses[i]=node.ID;
	}

}

int main(){
	Task t;

	//construct helper nodes(Nodes that exist only once)
	t.constructRSNewNode();
	t.constructRSOldNode();
	t.constructAlphaDenomNode();
	t.constructAlphaNode();

	const unsigned xSize=2;
	const unsigned aSize=2;
	const unsigned rsize=2;
	//construct nodes with multiple entries
	t.constructPNodes(rsize);
	t.constructApNodes(rsize);

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

	t.printNodes();
}

