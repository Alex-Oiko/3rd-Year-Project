/*
 * DESCRIPTION
 *   Very simple simulation of a NOT gate and a monitor to present an attached
 *   PC with an SDP status update.
 *
 * AUTHORS
 *   Kier J. Dugan - (kjd1v07@ecs.soton.ac.uk)
 *
 * DETAILS
 *   Created on       : 27 June 2012
 *   Revision         : $Revision: 202 $
 *   Last modified on : $Date: 2012-07-20 19:55:53 +0100 (Fri, 20 Jul 2012) $
 *   Last modified by : $Author: kjd1v07 $
 *   $Id: inverter.c 202 2012-07-20 18:55:53Z kjd1v07 $
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
extern "C" {

// CRT includes
#include "stdint.h"

// SpiNNaker API includes
#include "spin1_api.h"
#include "spinn_io.h"
#include "spinn_sdp.h"

};

#define FPMATH_NOFLOAT
#define FPMATH_NODOUBLE

#include <fpmath/fixed32.hpp>


/*
 * Global variables
 */

extern "C" uint virt_cpu;

/*
 * Data report structure
 */


//void sdp_callback (uint m, uint port)
//{
//    char        buf[61];
//    int         str_len;
//    sdp_msg_t*  msg  = (sdp_msg_t*)m;
//    
//    io_printf (IO_STD, "SDP packet Rx on port %d (%d bytes)\n", port,
//        msg->length);
//    
//    if (msg->length > 60)
//        str_len = 60;
//    else
//        str_len = (int)msg->length;
//    
//    for (int i = 0; i < (str_len - 1); ++i) {
//        buf[i] = msg->data[i];
//    }
//    buf[str_len] = 0;
//    
//    io_printf (IO_STD, "message: %s\n", buf);
//    
//    if (port == 7) {
//        io_printf (IO_STD, "stopping...\n");
//        spin1_stop ();
//    }
//}

void mc_packet_callback (uint key, uint payload)
{
    io_printf (IO_STD, "in MC packet callback.\n");
    //inthand_handle_mc_packet (key, payload);
}

void timer_callback (uint ticks, uint null)
{
    io_printf (IO_STD, "in timer callback (%d) pre\n", ticks);
    
    typedef fpmath::fixed32<16> fixed32_q16;
    
    fixed32_q16 fixedA (10, 5);
    fixed32_q16 fixedB (23, 7);
    fixed32_q16 fixedResult = fixedA * fixedB;
    
    io_printf (IO_STD, "Simple test:    A=%3.2f - mag: %d, frac: %d\n", fixedA.value (),
        fixedA.magnitudePart (), fixedA.fractionalPart ());
    io_printf (IO_STD, "Simple test 2: -A=%3.2f - mag: %d, frac: %d\n", (-fixedA).value (),
        (-fixedA).magnitudePart (), (-fixedA).fractionalPart ());

    io_printf (IO_STD, "Fixed point: %3.2f * %3.2f = %3.2f\n", fixedA.value (),
        fixedB.value (), fixedResult.value ());
    io_printf (IO_STD, "Fixed point: %3.2f / %3.2f = %3.2f\n", fixedA.value (),
        fixedB.value (), (fixedA / fixedB).value ());
    io_printf (IO_STD, "Fixed point: %3.2f + %3.2f = %3.2f\n", fixedA.value (),
        fixedB.value (), (fixedA + fixedB).value ());
    io_printf (IO_STD, "Fixed point: %3.2f - %3.2f = %3.2f\n", fixedA.value (),
        fixedB.value (), (fixedA - fixedB).value ());

    // test unary negation
    fixed32_q16 negA = -fixedA;
    fixed32_q16 negB = -fixedB;
    io_printf (IO_STD, "Fixed point: -%3.2f = %3.2f, -%3.2f = %3.2f\n", fixedA.value (),
        negA.value (), fixedB.value (), negB.value ());
    fixed32_q16 exactlyOne (1, 0);
    io_printf (IO_STD, "Fixed point: 1 = %3.2f\n", exactlyOne.value ());
    fixed32_q16 negOne = -exactlyOne;
    io_printf (IO_STD, "Fixed point: -1 = %3.2f\n", negOne.value ());
    
    spin1_stop ();
}



// Main program just sets up callbacks and then starts the API

extern "C" void c_main ()
{
    io_printf (IO_STD, "fixed point test program start!\n");

    spin1_set_timer_tick (1000000);

    spin1_callback_on (TIMER_TICK, timer_callback, 3);
    spin1_callback_on (MC_PACKET_RECEIVED, mc_packet_callback, 0);
    //spin1_callback_on (SDP_PACKET_RX, sdp_callback, 2);

    spin1_start ();

    io_printf (IO_STD, "\nfixed point test program done\n");
}

