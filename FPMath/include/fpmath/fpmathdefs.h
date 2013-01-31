/*
 * DESCRIPTION
 *   Platform independent definitions for the rest of the library.
 *
 * AUTHORS
 *   Kier J. Dugan - (kjd1v07@ecs.soton.ac.uk)
 *
 * DETAILS
 *   Created on       : 12 December 2011
 *   Revision         : $Revision: 31 $
 *   Last modified on : $Date: 2012-01-16 18:15:48 +0000 (Mon, 16 Jan 2012) $
 *   Last modified by : $Author: kjd1v07 $
 *   $Id: fpmathdefs.h 31 2012-01-16 18:15:48Z kjd1v07 $
 *
 * COPYRIGHT
 *   Copyright (c) 2011 The University of Southampton.  All Rights Reserved.
 *   Electronics and Electrical Engingeering Group,
 *   School of Electronics and Computer Science (ECS)
 *
 */
#ifndef __FPM_FPMATHDEFS_H
#define __FPM_FPMATHDEFS_H


/*
 * Compiler detection.
 */


/*
 * Namespacing
 */
#define FPMATH_NAMESPACE            fpmath

#ifndef FPMATH_NO_NAMESPACE
#define FPMATH_NAMESPACE_OPEN()     namespace FPMATH_NAMESPACE {
#define FPMATH_NAMESPACE_CLOSE()    };
#else   /* !FPMATH_NO_NAMESPACE */
#define FPMATH_NAMESPACE_OPEN()
#define FPMATH_NAMESPACE_CLOSE()
#endif  /* !FPMATH_NO_NAMESPACE */


/*
 * End of fpmathdefs.h
 */
#endif  /* !__FPM_FPMATHDEFS_H */

