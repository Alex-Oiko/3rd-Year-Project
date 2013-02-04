/*
 * DESCRIPTION
 *   Simple test bed for the SpiNNaker fixed-point maths library.
 *
 * AUTHORS
 *   Kier J. Dugan - (kjd1v07@ecs.soton.ac.uk)
 *
 * DETAILS
 *   Created on       : 12 December 2011
 *   Revision         : $Revision: 31 $
 *   Last modified on : $Date: 2012-01-16 18:15:48 +0000 (Mon, 16 Jan 2012) $
 *   Last modified by : $Author: kjd1v07 $
 *   $Id: fpmathtest.cpp 31 2012-01-16 18:15:48Z kjd1v07 $
 *
 * COPYRIGHT
 *   Copyright (c) 2011 The University of Southampton.  All Rights Reserved.
 *   Electronics and Electrical Engingeering Group,
 *   School of Electronics and Computer Science (ECS)
 *
 */

#include <stdio.h>

#include <fpmath/fixed32.hpp>


/*
 * entry point
 */
int main (int argc, char** argv)
{
    typedef fpmath::fixed32<16> fixed32_q16;

    float fA = 10.5f;
    float fB = 23.7f;
    float fResult = fA * fB;

    fixed32_q16 fixedA (10, 5);
    fixed32_q16 fixedB (23, 7);
    //fixed32_q16 fixedA, fixedB;
    //fixedA = 10.5f;
    //fixedB = 23.7f;
    fixed32_q16 fixedResult = fixedA * fixedB;

    float fFixedResult = fixedResult;

    printf ("Floating point: %3.2f * %3.2f = %3.2f\n", fA, fB, fResult);
    printf ("Fixed point:    %3.2f * %3.2f = %3.2f\n", fixedA.toFloat (),
        fixedB.toFloat (), fFixedResult);

    printf ("Floating point: %3.2f / %3.2f = %3.2f\n", fA, fB, fA / fB);
    printf ("Fixed point:    %3.2f / %3.2f = %3.2f\n", fixedA.toFloat (),
        fixedB.toFloat (), (fixedA / fixedB).toFloat ());

    printf ("Floating point: %3.2f + %3.2f = %3.2f\n", fA, fB, fA + fB);
    printf ("Fixed point:    %3.2f + %3.2f = %3.2f\n", fixedA.toFloat (),
        fixedB.toFloat (), (fixedA + fixedB).toFloat ());

    printf ("Floating point: %3.2f - %3.2f = %3.2f\n", fA, fB, fA - fB);
    printf ("Fixed point:    %3.2f - %3.2f = %3.2f\n", fixedA.toFloat (),
        fixedB.toFloat (), (fixedA - fixedB).toFloat ());

    // test unary negation
    fixed32_q16 negA = -fixedA;
    fixed32_q16 negB = -fixedB;
    printf ("Fixed point: -%3.2f = %3.2f, -%3.2f = %3.2f\n", fixedA.toFloat (),
        negA.toFloat (), fixedB.toFloat (), negB.toFloat ());
    fixed32_q16 exactlyOne (1, 0);
    printf ("Fixed point: 1 = %3.2f\n", exactlyOne.toFloat ());
    fixed32_q16 negOne = -exactlyOne;
    printf ("Fixed point: -1 = %3.2f\n", negOne.toFloat ());

    return 0;
}

