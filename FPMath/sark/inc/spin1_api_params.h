/****a* spinn_api_params.h/spinn_api_params_header
*
* SUMMARY
*  SpiNNaker API internal parameters and function prototypes
*
* AUTHOR
*  Luis Plana   - lap@cs.man.ac.uk
*  Thomas Sharp - thomas.sharp@cs.man.ac.uk
*
* DETAILS
*  Created on       : 03 May 2011
*  Version          : $Revision: 200 $
*  Last modified on : $Date: 2012-07-19 18:15:05 +0100 (Thu, 19 Jul 2012) $
*  Last modified by : $Author: kjd1v07 $
*  $Id: spin1_api_params.h 200 2012-07-19 17:15:05Z kjd1v07 $
*  $HeadURL: https://solem.cs.man.ac.uk/svn/spin1_api/trunk/src/spin1_api_params.h $
*
* COPYRIGHT
*  Copyright (c) The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
*******/

#ifndef __SPINN_API_PARAM_H__
#define __SPINN_API_PARAM_H__


// ------------------------------------------------------------------------
// internal constants and parameters -- not visible in the API
// ------------------------------------------------------------------------
// ---------------------
/* simulation control */
// ---------------------
/* synchronization barrier key and mask codes */
#define GO_KEY                (uint) ((0x1ffff << 11) | 0)
#define RDYGO_KEY             (uint) ((0x1ffff << 11) | 1)
#define RDY1_KEY              (uint) ((0x1ffff << 11) | 2)
#define RDY2_KEY              (uint) ((0x1ffff << 11) | 3)
#define BARRIER_MASK          (uint) (0xffffffff)
#define BARRIER_MAX_WAIT      5000
#define BARRIER_RESEND_WAIT   100
#define RX_READY_PRIORITY     1

/* lock usage */
#define CLEAR_LCK             0
#define RTR_INIT_LCK          0xad
#define RDYGO_LCK             0xbe

/* link orientation codes */
#define EAST                  0
#define NORTH_EAST            1
#define NORTH                 2
#define WEST                  3
#define SOUTH_WEST            4
#define SOUTH                 5

/* print warnings at end of simulation */
#define API_WARN              TRUE
/* print debug messages */
#define API_DEBUG             FALSE
#define API_PRINT_DLY         200

/* internal error/warning return codes */
#define NO_ERROR              0
#define TASK_QUEUE_FULL       1
#define DMA_QUEUE_FULL        2
#define PACKET_QUEUE_FULL     4
#define WRITE_BUFFER_ERROR    8
// --------------------------------

// ----------------
/* data transfer */
// ----------------
// DMA transfer parameters: 4-doubleword bursts
//TODO: may need adjustment for SpiNNaker
#define DMA_BURST_SIZE        2
#define DMA_WIDTH             1
// internal DMA queue size
#define DMA_QUEUE_SIZE        16
// select write buffer use
#define USE_WRITE_BUFFER      FALSE
// ---------------------------

// -----------------
// communications */
// -----------------
#define TX_PACKET_QUEUE_SIZE  16
// TX control register programming
#define TX_TCR_MCDEFAULT      0x00000000
#define TX_FULL_MASK          0x40000000
#define TX_EMPTY_MASK         0x80000000
#define RX_RECEIVED_MASK      0x80000000
// -----------------

// --------------------
/* memory allocation */
// --------------------
// memory space reserved for RTS stacks
#define RTS_STACKS            4096
// --------------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
// callback queue parameters
#define NUM_PRIORITIES    5
#define TASK_QUEUE_SIZE   16
// -----------------------

// -----------------------
// Allocation of entries in MC table
// -----------------------
#define SYS_MC_ENTRIES 24
#define APP_MC_ENTRIES (MC_TABLE_SIZE - SYS_MC_ENTRIES)
// -----------------------

// -----------------------
// exception vectors
// -----------------------
#define EXCEPTION_BASE 0x00000020
#define RESET_VEC      0
#define UNDEF_VEC      1
#define SVC_VEC        2
#define PABT_VEC       3
#define DABT_VEC       4
#define ADDR_VEC       5
#define IRQ_VEC        6
#define FIQ_VEC        7
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// helpful macros
// ------------------------------------------------------------------------
#define CHIP_ADDR(x, y)      ((x << 8) | y)
#define P2P_ROUTE(addr)      (1 << p2p_get(addr))
#define CORE_ROUTE(core)     (1 << (core + NUM_LINKS))

// TODO: need fixing -- works for 4-chip system only!
#define ADDR_TO_CHIP(addr)   ((addr >> 7) | (addr & 0xff))
#define CHIP_TO_ADDR(chip)   (((chip << 7) & 0xff00) | (chip & 1))
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal pre-defined types -- not visible in the API
// ------------------------------------------------------------------------
// ----------------
/* data transfer */
// ----------------
typedef struct
{
  uint id;
  uint tag;
  uint* system_address;
  uint* tcm_address;
  uint description;
} copy_t;


typedef struct
{
  uint start;
  uint end;
  copy_t queue[DMA_QUEUE_SIZE];
} dma_queue_t;
// ----------------

// -----------------
/* communications */
// -----------------
typedef struct
{
  uint key;
  uint data;
  uint load;
} packet_t;

typedef struct
{
  uint start;
  uint end;
  packet_t queue[TX_PACKET_QUEUE_SIZE];
} tx_packet_queue_t;
// -----------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
typedef struct
{
  callback_t cback;
  int priority;
} cback_t;

typedef struct
{
  callback_t cback;
  uint arg0;
  uint arg1;
} task_t;

typedef struct
{
  uint start;
  uint end;
  task_t queue[TASK_QUEUE_SIZE];
} task_queue_t;

extern cback_t callback[NUM_EVENTS];

// interrupt service routine
#ifdef __GNUC__
typedef void (*isr_t) (void);
#else
typedef __irq void (*isr_t) (void);
#endif
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal variables -- not visible in the API
// ------------------------------------------------------------------------
/* VIC vector tables */
static volatile isr_t * const vic_vectors  = (isr_t *) (VIC_BASE + 0x100);
static volatile uint * const vic_controls = (uint *) (VIC_BASE + 0x200);
static isr_t * const exception_table = (isr_t *) EXCEPTION_BASE;
// ------------------------------------------------------------------------

#endif /* __SPINN_API_PARAM_H__ */
