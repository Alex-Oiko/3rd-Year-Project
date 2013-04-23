//
//  Handler.c
//  MatrixHandler
//
//  Created by Jeff Reeve on 28/02/2013.
//  Copyright (c) 2013 Jeff Reeve. All rights reserved.
//

#include <stdio.h>
/*
 * DESCRIPTION
 *   Very simple simulation of a Diffusion Equation (Jacobi style) and a monitor to present an attached
 *   PC with an SDP status update.

 *
 * AUTHORS
 *   Kier J. Dugan - (kjd1v07@ecs.soton.ac.uk) + Jeff Reeve
 *
 * DETAILS
 *   Created on       : 27 June 2012
 *   Revision         : $Revision: 201 $
 *   Last modified on : $Date: 2012-09-03 22:53:28 +0100 (Mon, 3rd September 2012) $
 *   Last modified by : $Author: jsr $
 *   $Id: inverter.c 201 2012-07-19 21:53:28Z kjd1v07 $
 *
 * COPYRIGHT
 *   Copyright (c) 2012 The University of Southampton.  All Rights Reserved.
 *   Electronics and Electrical Engingeering Group,
 *   School of Electronics and Computer Science (ECS)
 *
 */

/*
 * Required headers
 */

// CRT includes
#include "stdint.h"

// SpiNNaker API includes
#include "spin1_api.h"
#include "spinn_io.h"
#include "spinn_sdp.h"

// IntHand API includes
#include "inthand.h"
#include "intdefs.h"    // generated by the Uploader
#include "intalias.h"   // generated by the Uploader

/*
 * Global variables
 */

extern uint virt_cpu;
uint32_t matrix_size=2,case15_counter=0;
/*
 *Structure of the TCRAM
 */

typedef struct {
    uint32_t PointCount;
    uint32_t LUTStart;
    uint32_t LUTCount;
    uint32_t TTStart;
    uint32_t ValuesStart;
    uint32_t CounterStart;
    uint32_t TempStart;
    uint32_t OpCodesStart;
} _CoreCommon;
typedef struct {
    uint32_t Kd;
    uint32_t OpCode;
    char Name;
    uint32_t YD;
    uint32_t Y;
    uint32_t X;
    float temp;
    uint32_t counter;
    float IV;
    uint32_t oV;
    uint32_t V;
    uint32_t Expected;
    uint32_t Arrived;
} _DTTE; 
typedef struct {
    uint32_t Ks;
    uint32_t idx;
} _LookUp;


uint32_t front(uint32_t offset,uint32_t* OpCodesA){
	uint32_t pq_size=1,head=0,tail=0,ihead=0;
	for(int i=0;i<OpCodesA[0];i++){
		if(OpCodesA[3+pq_size]==offset){
			head=OpCodesA[1+pq_size];
			tail=OpCodesA[2+pq_size];
			ihead=4+pq_size+head;
			return OpCodesA[ihead];
		}
		else
			pq_size+=OpCodesA[pq_size]+4;
	}
	return -1;
}
uint32_t pop(uint32_t offset,uint32_t* OpCodesA){
	uint32_t pq_size=1,head=0,tail=0,ihead=0;
	for(int i=0;i<OpCodesA[0];i++){
		if(OpCodesA[3+pq_size]==offset){
			head=OpCodesA[1+pq_size];
			tail=OpCodesA[2+pq_size];
			ihead=4+pq_size+head;
			head++;
		        tail--;	
			OpCodesA[1+pq_size]=head;
			OpCodesA[2+pq_size]=tail;
			return 0;
		}
		else
			pq_size+=OpCodesA[pq_size]+4;
	}
	return -1;
}
uint32_t pushandpop(uint32_t offset,uint32_t* OpCodesA){
	uint32_t pq_size=1,head=0,tail=0;
	for(int i=0;i<OpCodesA[0];i++){
		if(OpCodesA[3+pq_size]==offset){
			head=OpCodesA[1+pq_size];
			tail=OpCodesA[2+pq_size];
			head++;
			if(head==OpCodesA[pq_size]){
				head=OpCodesA[pq_size]-tail;
			}
			OpCodesA[1+pq_size]=head;
			return 0;
		}
		else
			pq_size+=OpCodesA[pq_size]+4;
	}
	return -1;
}
static void KeyTo(unsigned Key,unsigned& X, unsigned& Y, unsigned& C, unsigned& O){
    X = Key>>24;
    Y = (Key>>16)&0x0ff;
    C = (Key >> 11)&0x0f;
    O = (Key&0x07FF);
}
void RemoveOps(uint32_t ticks, uint32_t unused){
     uint32_t *CoreData = (uint32_t *)TCDRAM_MIN_ADDR;
     uint32_t *OpCodes,Xs,Ys,Cs,Os;
     float *Values,RES;
     static uint32_t done = 0;
     _CoreCommon *CoreCommon;
     _DTTE *TTE,dTTE;
     if(done)
         return;
     done = 1;
     CoreCommon = (_CoreCommon*)CoreData;
     TTE = (_DTTE*)&CoreData[CoreCommon->TTStart];
     Values = (float*)&CoreData[CoreCommon->ValuesStart];
     OpCodes = (uint32_t*)&CoreData[CoreCommon->OpCodesStart];
     for(uint32_t Lup = 0; Lup < CoreCommon->PointCount; Lup++){
	dTTE=TTE[Lup];
     	switch(dTTE.Name){
     			case 'M':
				pop(1,OpCodes);
     	      			break;
		     	case 'X':
				//dont delete anything
        		     	 break;
		     	case 'R':
				KeyTo(dTTE.Kd,Xs,Ys,Cs,Os);
        		     	if(Os==2){
			     		pop(3,OpCodes);
	     			}
             			else{
	     				//cout<<"Size of OpCodes at key 0 is:"<<front(0,OpCodes);
				}
             			break;
		     	case 'P':
				pop(3,OpCodes);
	      			RES=Values[dTTE.oV];
				spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
	     	      		break;
		     	case 'A':
				for(int s=0;s<matrix_size;s++){
     	      				pop(0,OpCodes);
				}
		     		break;
	     		case 'N':
				for(int s=0;s<matrix_size;s++){
					pop(3,OpCodes);
				}
     		      		break;
		     	case 'L':
				pop(2,OpCodes);
     			      	break;	
	     		case 'B':
				pop(2,OpCodes);
			      	break;
	}
   }
}

void InComing(uint32_t Ks, float Vs){
    uint32_t LUTCount = 0, InPoint,Xs,Ys,Cs,Os;
    uint32_t *CoreData = (uint32_t *)TCDRAM_MIN_ADDR;
    float Vme, *Values,RES,Res,*Temps;
    uint32_t *counters,*OpCodes;
    KeyTo(Ks,Xs,Ys,Cs,Os);
    _LookUp *LUT;
    _CoreCommon *CoreCommon;
    _DTTE *TTE, dTTE;
    CoreCommon = (_CoreCommon*)CoreData;
    LUTCount = CoreCommon->LUTCount;
    LUT = (_LookUp*)&CoreData[CoreCommon->LUTStart];
    TTE = (_DTTE*)&CoreData[CoreCommon->TTStart];
    Values = (float*)&CoreData[CoreCommon->ValuesStart];
    counters = (uint32_t*)&CoreData[CoreCommon->CounterStart];
    Temps = (float*)&CoreData[CoreCommon->TempStart];
    OpCodes = (uint32_t*)&CoreData[CoreCommon->OpCodesStart];
    for(int k = 0; k < LUTCount ; k++){
        if(Ks == LUT[k].Ks){
            InPoint = LUT[k].idx;
            dTTE = TTE[InPoint];
	    switch(front(Os,OpCodes)){
		case 0:
			pushandpop(Os,OpCodes);
			break;
        	case 1://multiplication but no send
            		Vme = Values[dTTE.oV];
            		Res = Vme*Vs;
            		RES = Res;
			pushandpop(Os,OpCodes);
			if(dTTE.Name=='A'){
				Values[dTTE.oV]=0;
			}
			spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			break;
        	case 2://specific for the creation of r
	    		counters[dTTE.oV]++;
          		Vme = Values[dTTE.oV];
            		Values[dTTE.oV] -=Vs;
			if(counters[dTTE.oV]==dTTE.YD){
				RES=Values[dTTE.oV];
	    			pop(Os,OpCodes);
				counters[dTTE.oV]=0;
				spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
	    		}
        		break;
		case 4://assignement
			Values[dTTE.oV]=Vs;
			Temps[dTTE.oV]=Vs;
			RES=Values[dTTE.oV];
	    		pushandpop(Os,OpCodes);
			spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			break;
	
		case 5://constant
			counters[dTTE.oV]++;
			Values[dTTE.oV]+=Vs*Vs;
			if(counters[dTTE.oV]==matrix_size){//no TTE.YD because node size is 1, because it is single node
				RES=Values[dTTE.oV];
				counters[dTTE.oV]=0;
	    			pushandpop(Os,OpCodes);
				if(dTTE.Name=='N')//so it does not have previous value in other iterations
					Values[dTTE.oV]=0;

				spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			}
			break;
		case 6://to create Ap nodes
			counters[dTTE.oV]++;
			Values[dTTE.oV]+=Vs;
	    		pushandpop(Os,OpCodes);
			if(counters[dTTE.oV]==dTTE.YD){
				RES=Values[dTTE.oV];
				counters[dTTE.oV]=0;
				spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			}
			break;
		case 7://Assignement without send
			Values[dTTE.oV]=Vs;
			Temps[dTTE.oV]=Vs;
	    		pushandpop(Os,OpCodes);
			break;
		case 8://create alpha node
			if(counters[dTTE.oV]==0)
				Temps[dTTE.oV]=0;

			counters[dTTE.oV]++;
			Temps[dTTE.oV]+=Vs;
	    		pushandpop(Os,OpCodes);
			if(counters[dTTE.oV]==matrix_size){
				Values[dTTE.oV]=Values[dTTE.oV]/Temps[dTTE.oV];
				RES=Values[dTTE.oV];
				counters[dTTE.oV]=0;
				spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			}
			break;
		case 10://make new r nodes, basically subtraction
			Values[dTTE.oV]-=Vs;
	    		pushandpop(Os,OpCodes);
			RES=Values[dTTE.oV];
			spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			break;
		case 11://make new x nodes
			Values[dTTE.oV]+=Vs;
	    		pushandpop(Os,OpCodes);
			RES=Values[dTTE.oV];
			io_printf (IO_STD, "X Value here. Value is %f",Values[dTTE.oV]);
			//spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			break;
		case 12://make beta
			Values[dTTE.oV]=Vs/Values[dTTE.oV];
	    		pushandpop(Os,OpCodes);
			RES=Values[dTTE.oV];
			Values[dTTE.oV]=Vs;//save it as rsold for the next iteration
			spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			break;
		case 13://check end condition
			if(Vs<1e-10){
				io_printf (IO_STD, "END CONDITION TRUE....TERMINATING");
				//exit(EXIT_SUCCESS);	
			}
			else{
			}
			break;
		case 14://temp assignement only
			Temps[dTTE.oV]=Vs;
	    		pushandpop(Os,OpCodes);
			break;
		case 15://make new p nodes
			Values[dTTE.oV]*=Vs;
			Values[dTTE.oV]+=Temps[dTTE.oV];
	    		pushandpop(Os,OpCodes);
			case15_counter++;
			if(case15_counter==matrix_size){//when case15_counter==N 
    				spin1_callback_on (TIMER_TICK, RemoveOps, 3);
			}
			if(case15_counter>matrix_size){//if the first iteration has passed
				RES=Values[dTTE.oV];
				spin1_send_mc_packet(dTTE.Kd,RES,WITH_PAYLOAD);
			}
			break;
		}
        }
    }

}

void Update(uint32_t ticks, uint32_t unused){
    uint32_t *CoreData = (uint32_t *)TCDRAM_MIN_ADDR;
    uint32_t *OpCodes;
    static uint32_t done = 0;
    _CoreCommon *CoreCommon;
    _DTTE *TTE;
    if(done)
        return;
    done = 1;
    CoreCommon = (_CoreCommon*)CoreData;
    TTE = (_DTTE*)&CoreData[CoreCommon->TTStart];
    OpCodes = (uint32_t*)&CoreData[CoreCommon->OpCodesStart];
    for(uint32_t Lup = 0; Lup < CoreCommon->PointCount; Lup++){
        if(TTE[Lup].Name=='X'){
            spin1_send_mc_packet(TTE[Lup].Kd,TTE[Lup].IV,WITH_PAYLOAD); /* Start the Matrix Vector multiply  */
            io_printf (IO_STD, "sent %f from %x\n",TTE[Lup].IV,TTE[Lup].Kd);
        }
    }
}

// Main program just sets up callbacks and then starts the API

void c_main ()
{
    io_printf (IO_STD, "Jacobi test program start!\n");
    
    inthand_init (0);
    
    spin1_set_timer_tick (1000000);
    
    spin1_callback_on (TIMER_TICK, Update, 3);
    spin1_callback_on (MC_PACKET_RECEIVED, (callback_t)InComing, 0);
    
    spin1_start ();
    
    io_printf (IO_STD, "\nJacobi test program done\n");
}

