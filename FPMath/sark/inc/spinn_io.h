/****a* spinn_io.h/spinn_io_header
*
* SUMMARY
*  Implementation of output printing streams for SpiNNaker
*
* AUTHOR
*  Steve Temple - temples@cs.man.ac.uk
*
* DETAILS
*  Created on       : 03 May 2011
*  Version          : $Revision: 200 $
*  Last modified on : $Date: 2012-07-19 18:15:05 +0100 (Thu, 19 Jul 2012) $
*  Last modified by : $Author: kjd1v07 $
*  $Id: spinn_io.h 200 2012-07-19 17:15:05Z kjd1v07 $
*  $HeadURL: https://solem.cs.man.ac.uk/svn/spin1_api/trunk/src/spinn_io.h $
*
* COPYRIGHT
*  Copyright (c) The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
*******/

//------------------------------------------------------------------------------
//
// spinn_io.h	    Simple character I/O library for Spinnaker
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#ifndef SPINN_IO_H
#define SPINN_IO_H


#define IO_STD 		((char *) 0)		// Stream numbers
#define IO_DBG   	((char *) 1)
#define IO_xxx_2   	((char *) 2)
#define IO_NULL    	((char *) 3)


void io_put_char (char *stream, uint c);

void io_put_str (char *stream, char *s, int d);

void io_put_int (char *stream, int n, uint d, uint pad); // pad not used!

void io_put_uint (char *stream, uint n, uint d, uint pad);

void io_put_zhex (char *stream, uint n, uint d);

void io_put_hex (char *stream, uint n, uint d, uint pad);

void io_put_nl (char *stream);

void io_put_mac (char *stream, uchar *s);

void io_put_ip (char *stream, uchar *s);

void io_printf (char *stream, char *f, ...);

#endif
