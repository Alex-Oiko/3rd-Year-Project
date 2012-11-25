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
			unsigned number;
			int value;
			unsigned PNode;
			int PValue;
	};

	struct ANode{
		unsigned number;
		int value;
		unsigned XNode;
		int Xvalue;
		int lastResult;

	};

	struct RNode{
		unsigned number;
		int value;
		unsigned *ANodes;
		int *lastresults;
		unsigned squareValue;

	};

	struct PNode{
		unsigned number;
		int value;
		unsigned RNode;
		unsigned Value;
		int alpha;
		int alphaValue;
		long rson;//rsold/rsnew
	};

	unsigned nodeNumber;
	XNode *XNodes;
	ANode *ANodes;
	RNode *RNodes;

	void constructXNodes(int *xVector,unsigned vectorSize);
	void constructANodes(unsigned size,int **matrix);
	void constructRNodes(unsigned size,int *bvector);
	void printNodes();
};

#endif /* TASK_H_ */
