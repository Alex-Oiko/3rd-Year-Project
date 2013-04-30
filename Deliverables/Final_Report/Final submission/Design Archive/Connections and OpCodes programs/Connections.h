#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

class Connections{
public:
	void WriteX(unsigned);
	void WriteMX(unsigned);
	void WriteR(unsigned);
	void WriteP(unsigned);
	void WriteA(unsigned);
	void WriteB(unsigned);
	void WriteN(unsigned);
	void WriteRO(unsigned);
	void WriteL(unsigned);
	unsigned dimension;

};
