#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

int main(){

	printf("hello\n");
	unsigned *hello={2,3,0,3,4,0,0,11,1,0,1,16,3};
	for(int i=0;i<13;i++){
		printf("value is %u\n",hello[i]);
	}
	unsigned q_size=0,head=0,tail=0;
	for(int i=0;i<OpCodes[0];i++){
		if(i==0)
			q_size=0;
		else
			q_size=OpCodes[1+3*i+q_size];
		if(OpCodes[4+q_size]==offset){
			head=OpCodes[2+2*i+size];
			tail=OpCodes[3+1*i+size];
			head++
			if(head>tail){
				head=0;
			}
			else{
				OpCodes[2+2*i+size]=head;
			}
		}
	}
	return 0;
}
