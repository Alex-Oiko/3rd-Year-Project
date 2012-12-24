/*
 * Task.h
 *
 *  Created on: Nov 25, 2012
 *      Author: alex
 */

#ifndef TASK_H_
#define TASK_H_

using namespace std;

class Task {

public:
	struct XNode{
			unsigned ID;
			int value;
			unsigned PNode;
			int PValue;
	};

	struct ANode{
		unsigned ID;
		int value;
		unsigned XNode;
		int Xvalue;
		int lastResult;

	};

	struct RNode{
		unsigned ID;
		int value;
		unsigned *ANodeID;
		int *lastresults;
		unsigned squareValue;

	};

	struct PNode{
		unsigned ID;
		int value;
		unsigned RNodeID;
		unsigned RValue;
		int alpha;
		int alphaValue;
		long rson;//rsold/rsnew
	};


	unsigned nodeNumber;
	XNode *XNodes;
	ANode *ANodes;
	RNode *RNodes;
	PNode *PNodes;

	void constructXNodes(int *xVector,unsigned vectorSize);
	void constructANodes(unsigned size,int **matrix);
	void constructRNodes(unsigned size,unsigned Asize,int *bvector);
	void constructPNodes(unsigned size);
	void printNodes();
};

#endif /* TASK_H_ */
