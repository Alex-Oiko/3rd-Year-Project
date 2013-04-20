#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

void ForM(ofstream& wfile,string line){
	wfile<<line<<" 1/1;4/1/0/0"<<endl;
}
void ForX(ofstream& wfile,string line){
	wfile<<line<<" 4/0/0/11;16/3"<<endl;
}
void ForR(ofstream& wfile,string line,unsigned dimension){
	wfile<<line<<" 0/2/";
	for(int i =0;i<dimension;i++){
		wfile<<"0/";
	}
	wfile<<"0;5/0/10"<<endl;
}
void ForRO(ofstream& wfile,string line,unsigned dimension){
	wfile<<line<<" 3/5";
	for(int i=0;i<dimension;i++){
		wfile<<"/0";
	}
	wfile<<";6/7"<<endl;
}
void ForP(ofstream& wfile,string line){
	wfile<<line<<" 3/4/14;5/1/0;7/1;9/15"<<endl;
}
void ForA(ofstream& wfile,string line,unsigned dimension){
	wfile<<line<<" 0";
	for(int i=0;i<dimension;i++){
		wfile<<"/0";
	}
	for(int i=0;i<dimension;i++){
		wfile<<"/6";
	}
	wfile<<";7/1"<<endl;
}
void ForN(ofstream& wfile,string line,unsigned dimension){
	wfile<<line<<" 3/";
	for(int i=0;i<dimension;i++){
		wfile<<"0/";
	}
	wfile<<"5"<<endl;
}
void ForCH(ofstream& wfile,string line){
	wfile<<line<<" 6/13"<<endl;
}
void ForB(ofstream& wfile,string line){
	wfile<<line<<" 2/7;6/12"<<endl;
}
void ForL(ofstream& wfile,string line,unsigned dimension){
	wfile<<line<<" 2/7;4";
	for(int i=0;i<dimension;i++){
		wfile<<"/0";
	}
	for(int i=0;i<dimension;i++){
		wfile<<"/8";
	}
	for(int i=0;i<dimension;i++){
		wfile<<"/0";
	}
	wfile<<";6/7"<<endl;

}


int main(){
	string file,line,wsfile;
	ifstream ifile;
	ofstream wfile;
	unsigned dimension;
	cout<<"This file is used to write the OpCodes of the nodes in the .mx file"<<endl;
        cout<<"Give one of the dimensions of the MX1 matrix"<<endl;
        cin>>dimension;
        cout<<"Cool thanks"<<endl;
        cout<<"Please give a file name "<<endl;
        cin>>file;
	wsfile = file +"1.mx";
        file = file + ".mx";
	ifile.open(file,ios::in);
        if(ifile.fail()){
             cerr<<"this file '"<<file<<"' does not exist in the form of .mx"<<endl;
             return false;
        }
        else{
		 wfile.open(wsfile,ios::app);
		 wfile<<"BEGIN VALUES"<<endl;
		 while(!ifile.eof()){
		 	getline(ifile,line);
		 	if(isdigit(line[0])){
				wfile<<line<<endl;
			}
			else{
				switch(line[0]){
					case 'M':
						ForM(wfile,line);
						break;
					case 'X':
						ForX(wfile,line);
						break;
					case 'R':
						if(line[1]=='O'){
							ForRO(wfile,line,dimension);
						}
						else{
							ForR(wfile,line,dimension);
						}
					break;
					case 'P':
						ForP(wfile,line);
						break;
					case 'A':
						ForA(wfile,line,dimension);
						break;
					case 'N':
						ForN(wfile,line,dimension);
						break;
					case 'C':
						ForCH(wfile,line);
						break;
					case 'L':
						ForL(wfile,line,dimension);
						break;
					case 'B':
						ForB(wfile,line);
						break;
				}
		 	}
		 }
		 wfile<<"END VALUES"<<endl;
		 wfile<<"BEGIN OVERLAYS"<<endl;
		 wfile<<"END OVERLAYS"<<endl;
		 wfile<<"BEGIN CONNECTIONS"<<endl;
                 ifile.close();
        }
        cout<<"File closed. TERMINATING....."<<endl;
	return 0;
}
