/*
 * DESCRIPTION
 *   Signed 32bit fixed-point number with variable-sized fractional part.
 *
 * AUTHORS
 *   Kier J. Dugan - (kjd1v07@ecs.soton.ac.uk)
 *
 * DETAILS
 *   Created on       : 12 December 2011
 *   Revision         : $Revision: 31 $
 *   Last modified on : $Date: 2012-01-16 18:15:48 +0000 (Mon, 16 Jan 2012) $
 *   Last modified by : $Author: kjd1v07 $
 *   $Id: fixed32.hpp 31 2012-01-16 18:15:48Z kjd1v07 $
 *
 * COPYRIGHT
 *   Copyright (c) 2011 The University of Southampton.  All Rights Reserved.
 *   Electronics and Electrical Engingeering Group,
 *   School of Electronics and Computer Science (ECS)
 *
 */
#ifndef __FPM_FIXED32_HPP
#define __FPM_FIXED32_HPP


/*
 * Interface required headers.
 */
#if !defined(FPMATH_NOFLOAT) || !defined(FPMATH_NODOUBLE)
#include <cmath>
#endif  /* !FPMATH_NOFLOAT || !FPMATH_NODOUBLE */

#include <stdint.h>

#include <fpmath/fpmathdefs.h>


/*
 * Optional namespace start
 */
FPMATH_NAMESPACE_OPEN()

/*
 * Foward declarations
 */
template <int32_t F> class fixed32;


/*
 * fixed32<F> - a signed 32bit Q(32-F).F fixed point number
 */
template <int32_t F>
class fixed32
{
public:
    typedef int32_t     value_type;
    typedef int64_t     wide_type;
    typedef fixed32<F>  this_type;

    static const value_type value_bits = 8 * sizeof(value_type);
    static const value_type mag_bits   = value_bits - (value_type)F;
    static const value_type frac_bits  = (value_type)F;
    static const value_type mag_mask   = (value_type)-1 << frac_bits;
    static const value_type frac_mask  = ~mag_mask; //(value_type)-1 >> frac_bits;

    // constructors
    fixed32 ();
    fixed32 (const fixed32<F>& other);
    fixed32 (value_type mag, value_type frac);

    // properties
    value_type  magnitudePart     () const;
    value_type  fractionalPart    () const;
    void        setMagnitudePart  (value_type mag);
    void        setFractionalPart (value_type frac);

    // simple methods
    void        assign      (const fixed32<F>& src);
    void        assign      (value_type mag, value_type frac);
    void        fetch       (value_type& mag, value_type& frac) const;
    fixed32<F>& operator=   (const fixed32<F>& src);

    // additive arithmetic operations
    fixed32<F>  operator+   (const fixed32<F>& other) const;
    fixed32<F>& operator+=  (const fixed32<F>& other);
    fixed32<F>  operator-   (const fixed32<F>& other) const;
    fixed32<F>& operator-=  (const fixed32<F>& other);

    // multiplicative
    fixed32<F>  operator*   (const fixed32<F>& other) const;
    fixed32<F>& operator*=  (const fixed32<F>& other);
    fixed32<F>  operator/   (const fixed32<F>& other) const;
    fixed32<F>& operator/=  (const fixed32<F>& other);

    // unary functions
    fixed32<F>  operator-   () const;

    // conversion to floating point
#ifndef FPMATH_NOFLOAT
    float       toFloat         () const;
                operator float  () const { return toFloat (); }
    fixed32<F>& operator=       (float src);
#endif  /* !FPMATH_NOFLOAT */

    // conversion to double-precision floating point
#ifndef FPMATH_NODOUBLE
    double      toDouble        () const;
                operator double () const { return toDouble (); }
    fixed32<F>& operator=       (double src);
#endif  /* !FPMATH_NODOUBLE */

    // raw accessors
    value_type  value           () const;
                operator value_type () const { return value (); }

private:
    explicit fixed32 (value_type value) : m_value (value) {}

    value_type      m_value;
};


/*
 * fixed32<F> implementation
 */

// constructors
template <int32_t F>
fixed32<F>::fixed32 ()
    : m_value (0)
{
}
template <int32_t F>
fixed32<F>::fixed32 (const fixed32<F>& other)
{
    assign (other);
}
template <int32_t F>
fixed32<F>::fixed32 (value_type mag, value_type frac)
{
    assign (mag, frac);
}

// properties
template <int32_t F>
typename fixed32<F>::value_type fixed32<F>::magnitudePart () const
{
    // sign extended left shift
    //value_type sign   = (value_type)(m_value >> (value_bits - 1));
    //value_type extend = sign << mag_bits;
    //value_type mag    = m_value >> frac_bits;
    //value_type result = mag | extend;
    //return result;
    value_type result = m_value >> frac_bits;
    if (result < 0)
        result += 1; // correct for 2's complement
    return result;
}
template <int32_t F>
typename fixed32<F>::value_type fixed32<F>::fractionalPart () const
{
    // sign extended mask
    //value_type sign   = (value_type)(m_value >> (value_bits - 1));
    //value_type extend = sign << frac_bits;
    //value_type frac   = m_value & frac_mask;
    //return (frac | extend);
    value_type sign   = (value_type)(m_value >> mag_bits);
    value_type extend = sign & mag_mask;
    value_type frac   = m_value & frac_mask;
    value_type result = extend | frac;
    return result;
}
template <int32_t F>
void fixed32<F>::setMagnitudePart (value_type mag)
{
    m_value &= frac_mask;       // remove current integer component
    m_value |= mag << frac_bits;  // add new one
}
template <int32_t F>
void fixed32<F>::setFractionalPart (value_type frac)
{
    m_value &= mag_mask;        // remove current fractional component
    m_value |= (frac & frac_mask); // add new one
}

// simple methods
template <int32_t F>
void fixed32<F>::assign (const fixed32<F>& src)
{
    // simple copy
    m_value = src.m_value;
}
template <int32_t F>
void fixed32<F>::assign (value_type mag, value_type frac)
{
    // shift new magnitude up beyond fractional part
    value_type newMag  = ((mag << frac_bits) & mag_mask);

    // calculate the proper value of the fractional part
    value_type newFrac = frac << frac_bits;
    while (newFrac > frac_mask)
        newFrac /= 10;

    // combine both parts together
    m_value = newMag | newFrac;
}
template <int32_t F>
void fixed32<F>::fetch (value_type& mag, value_type& frac) const
{
    // convenience
    mag  = magnitudePart ();
    frac = fractionalPart ();
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator= (const fixed32<F>& src)
{
    assign (src);
    return *this;
}

// additive arithmetic operations
template <int32_t F>
fixed32<F> fixed32<F>::operator+ (const fixed32<F>& other) const
{
    fixed32<F> result (*this);
    result += other;
    return result;
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator+= (const fixed32<F>& other)
{
    m_value += other.m_value;
    return *this;
}
template <int32_t F>
fixed32<F> fixed32<F>::operator- (const fixed32<F>& other) const
{
    fixed32<F> result (*this);
    result += -other;
    return result;
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator-= (const fixed32<F>& other)
{
    //m_value -= other.m_value;
    //return *this;
    return operator+= (-other);
}

// multiplicative
template <int32_t F>
fixed32<F> fixed32<F>::operator* (const fixed32<F>& other) const
{
     fixed32<F> result (*this);
     result *= other;
     return result;
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator*= (const fixed32<F>& other)
{
    // do multiplication in twice the space
    wide_type lhs    = (wide_type)m_value;
    wide_type rhs    = (wide_type)other.m_value;
    wide_type result = lhs * rhs;

    // result is now a Q2N.2M number, so right shift by M to get Q(2N-M).M
    m_value = (value_type)(result >> frac_bits);

    return *this;
}
template <int32_t F>
fixed32<F> fixed32<F>::operator/ (const fixed32<F>& other) const
{
    fixed32<F> result (*this);
    result /= other;
    return result;
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator/= (const fixed32<F>& other)
{
    // left-hand operand must be converted to Q2N.2M before division
    wide_type lhs    = (wide_type)m_value << frac_bits;
    wide_type rhs    = (wide_type)other.m_value;
    wide_type result = lhs / rhs;

    // result should now be a QN.M number due to division
    m_value = (value_type)result;

    return *this;
}
template <int32_t F>
fixed32<F> fixed32<F>::operator- () const
{
    //value_type negMag   = (~m_value + (1 << frac_bits)) & mag_mask;
    ////value_type negFrac  = (-m_value) & frac_mask;
    //value_type negFrac  = (~m_value + 1) & frac_mask;
    //value_type negFixed = negMag | negFrac;
    //return fixed32<F> (negFixed);
    return fixed32<F> (-m_value);
}

// conversion to floating point
#ifndef FPMATH_NOFLOAT
template <int32_t F>
float fixed32<F>::toFloat () const
{
    float intPart   = (float)magnitudePart ();
    float fracPart  = (float)fractionalPart ();
    float fracDiv   = (float)((1 << frac_bits) - 1);
    float fracScale = fracPart / fracDiv;
    float result    = intPart + fracScale;
    return result;
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator= (float src)
{
    float intPart, fracPart;

    // break the number into integer and fractional parts
    fracPart = modf (src, &intPart);

    // scale the fractional part to this fixed point 1
    fracPart *= (float)(1 << frac_bits);

    // convert both parts into integers
    value_type mag  = (value_type)intPart;
    value_type frac = (value_type)fracPart;

    // actually save the values
    assign (mag, frac);
    return *this;
}
#endif /* !FPMATH_NOFLOAT */

// conversion to double-precision floating point
#ifndef FPMATH_NODOUBLE
template <int32_t F>
double fixed32<F>::toDouble () const
{
    double intPart  = (double)magnitudePart ();
    double fracPart = (double)fractionalPart ();
    double result   = intPart + (fracPart / (double)(1 << frac_bits));
    return result;
}
template <int32_t F>
fixed32<F>& fixed32<F>::operator= (double src)
{
    double intPart, fracPart;

    // break the number into integer and fractional parts
    fracPart = modf (src, &intPart);

    // scale the fractional part to this fixed point 1
    fracPart *= (double)(1 << frac_bits);

    // convert both parts into integers
    value_type mag  = (value_type)intPart;
    value_type frac = (value_type)fracPart;

    // actually save the values
    assign (mag, frac);
    return *this;
}
#endif /* !FPMATH_NODOUBLE */

// raw accessors
template <int32_t F>
typename fixed32<F>::value_type fixed32<F>::value () const
{
    return m_value;
}


/*
 * End of fixed32.hpp
 */
FPMATH_NAMESPACE_CLOSE()
#endif  /* !__FPM_FIXED32_HPP*/

