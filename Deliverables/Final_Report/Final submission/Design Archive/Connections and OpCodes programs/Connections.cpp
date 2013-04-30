#include "Connections.h"

int main(){
        void WriteX(unsigned,ofstream&);
	void WriteMX1(unsigned,ofstream&);
	void WriteR(unsigned,ofstream&);
	void WriteRO(ofstream&);
	void WriteP(unsigned,ofstream&);
	void WriteA(unsigned,ofstream&);
	void WriteL(unsigned,ofstream&);
	void WriteN(ofstream&);
	void WriteB(unsigned,ofstream&);
	unsigned dimension;
	string file;
	ofstream sfile;
	cout<<"This file is used to write the connections of the nodes in the .mx file"<<endl;
	cout<<"Give one of the dimensions of the MX1 matrix"<<endl;
	cin>>dimension;
	cout<<"Cool thanks"<<endl;
	cout<<"Please give a file name "<<endl;
	cin>>file;
	file = file + ".mx";
	sfile.open(file,ios::app);
	if(sfile.fail()){
		cerr<<"this file '"<<file<<"' does not exist in the form of .mx"<<endl;
		return false;
	}
	else{
		cout<<"file "<<file<<" is k"<<endl;
		cout<<"On to the processing!!!"<<endl;
		//sfile<<"END CONNECTIONS"<<endl;
		WriteX(dimension,sfile);
		WriteMX1(dimension,sfile);
		WriteR(dimension,sfile);
		WriteRO(sfile);
		WriteP(dimension,sfile);
		WriteA(dimension,sfile);
		WriteL(dimension,sfile);
		WriteN(sfile);
		WriteB(dimension,sfile);
		sfile<<"END CONNECTIONS"<<endl;
		sfile.close();
	}
	cout<<"File closed. TERMINATING....."<<endl;
	return 0;
}

void WriteX(unsigned dimension,ofstream& sfile){
	for(int i=0;i<dimension;i++){
		sfile<<"X["<<i<<"]>";
		for(int f=0;f<dimension;f++){
			if(f==0)
				sfile<<"MX1["<<f<<","<<i<<"]";
			else
				sfile<<" MX1["<<f<<","<<i<<"]";
		}
		sfile<<"\n";
	}
}
void WriteMX1(unsigned dimension,ofstream& sfile){
	for(int i=0;i<dimension;i++){
		for(int f=0;f<dimension;f++){
			sfile<<"MX1["<<i<<","<<f<<"]>R["<<i<<"] A["<<i<<"]"<<endl;
		}
	}
}
void WriteR(unsigned dimension,ofstream& sfile){
	for(int i=0;i<dimension;i++){
		sfile<<"R["<<i<<"]>P["<<i<<"] RO[0] N[0]"<<endl;
	}

}
void WriteRO(ofstream& sfile){
	sfile<<"RO[0]>L[0] B[0]"<<endl;
}
void WriteP(unsigned dimension,ofstream& sfile){
	for(int i=0;i<dimension;i++){
		sfile<<"P["<<i<<"]>";
		for(int f=0;f<dimension;f++){
			sfile<<"MX1["<<f<<","<<i<<"] ";
		}
		sfile<<"X["<<i<<"] L[0]"<<endl;
	}
}
void WriteA(unsigned dimension,ofstream& sfile){
	for(int i=0;i<dimension;i++){
		sfile<<"A["<<i<<"]>R["<<i<<"] P["<<i<<"]"<<endl;
	}

}
void WriteL(unsigned dimension,ofstream& sfile){
	sfile<<"L[0]>";
	for(int i=0;i<dimension;i++){
		if(i==0)
			sfile<<"A["<<i<<"] P["<<i<<"]";
		else
			sfile<<" A["<<i<<"] P["<<i<<"]";
	}
	sfile<<"\n";

}
void WriteN(ofstream& sfile){
	sfile<<"N[0]>B[0] RO[1] CH[1] L[1]"<<endl;

}
void WriteB(unsigned dimension,ofstream& sfile){
	sfile<<"B[0]>";
	for(int i=0;i<dimension;i++){
		if(i==0)
			sfile<<"P["<<i<<"]";
		else
			sfile<<" P["<<i<<"]";
			
	}
	sfile<<"\n";
}
