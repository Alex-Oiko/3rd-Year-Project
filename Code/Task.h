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
		unsigned PNode;

	};

	struct RNode{
		unsigned ID;
		int value;
		unsigned *ANodeID;
		int *lastresults;
		unsigned squareValue;
		unsigned ApNodeID;

	};

	struct PNode{
		unsigned ID;
		int value;
		unsigned RNodeID;
		int RValue;
		unsigned alpha;
		int alphaValue;
		unsigned rsold;
		unsigned rsnew;
		long rson;//rsold/rsnew
	};

	struct RSoldNode{
		unsigned ID;
		int value;
		unsigned *Rnodes;
		unsigned rsnew;
	};

	struct RSNewNode{
		unsigned ID;
		int value;
		unsigned *Rnodes;
	};

	struct RSOldNode{
		unsigned ID;
		int value;
		unsigned rsNew;
		unsigned *RNodes;
	};

	struct AlphaDenomNode{
		unsigned ID;
		int value;
		unsigned *Ap_addresses;
	};

	struct AlphaNode{
		unsigned ID;
		int value;
		unsigned rsoldID;
		unsigned alpha_denomID;
	};

	struct ApNode{
		unsigned ID;
		int value;
		unsigned *Anodes;
		unsigned pNode;
		int last_result;
		unsigned alpha;
	};


	unsigned nodeNumber;
	XNode *XNodes;
	ANode *ANodes;
	RNode *RNodes;
	PNode *PNodes;
	RSNewNode rsnew;
	RSOldNode rsold;
	AlphaDenomNode alpha_denom;
	AlphaNode alpha;
	ApNode *ApNodes;

	void constructXNodes(int *xVector,unsigned vectorSize);
	void constructANodes(unsigned size,int **matrix);
	void constructRNodes(unsigned size,unsigned Asize,int *bvector);
	void constructPNodes(unsigned size);
	void constructRSNewNode();
	void constructRSOldNode();
	void constructAlphaDenomNode();
	void constructAlphaNode();
	void constructApNodes(unsigned size);
	void printNodes();
};

#endif /* TASK_H_ */
