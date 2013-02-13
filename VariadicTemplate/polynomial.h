#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#ifdef __SSE2__
#include <xmmintrin.h>
#endif

//! \brief old C based implementation
#define POLY0(x, c0)                        (c0)
#define POLY1(x, c0, c1)                    (c0 + (x * POLY0(x, c1)))
#define POLY2(x, c0, c1, c2)                (c0 + (x * POLY1(x, c1, c2)))
#define POLY3(x, c0, c1, c2, c3)            (c0 + (x * POLY2(x, c1, c2, c3)))
#define POLY4(x, c0, c1, c2, c3, c4)        (c0 + (x * POLY3(x, c1, c2, c3, c4)))
#define POLY5(x, c0, c1, c2, c3, c4, c5)    (c0 + (x * POLY4(x, c1, c2, c3, c4, c5)))

float poly_c_macro(float x)
{
    return POLY5(x, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
}

//! \brief Computing c0 + c1*x^1 + c2*x^2 + ... + cN*x^N
//! The function is coded like a variadic template using the same structure of
//! the C macros.
//! C macros is not expandable without changing the code, while the number of
//! arguments in the variadic template can be changed without further
//! modification (just recompilation)
//! \author Davide Anastasia <davideanastasia@users.sourceforge.net>
template <typename T, typename C>
T polynomial(T /*t*/, C c0)
{
    return c0;
}

template <typename T, typename C, typename ...Args>
T polynomial(T x, C c0, Args... cN)
{
    return (polynomial(x, cN...)*x) + c0;
}

#ifdef __SSE2__
//! \brief template specialization for Intel SSE intrinsics
template <typename C>
__m128 polynomial(__m128 /* t */, C c0)
{
    return _mm_set1_ps(static_cast<float>(c0));
}

template <typename C, typename ...Args>
__m128 polynomial(__m128 x, C c0, Args... cN)
{
    return (polynomial(x, cN...)*x) + _mm_set1_ps(static_cast<float>(c0));
}
#endif

template<typename T>
T poly_variadic(T x)
{
    return polynomial(x, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
}


#endif // POLYNOMIAL_H
