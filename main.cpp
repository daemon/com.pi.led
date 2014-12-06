#include <iostream>
#include <iomanip>
#include <type_traits>

/* Naive implementation of approximating PI. It's somewhat compiler specific,
 * system specific, and prone to rounding error. I made it to demonstrate
 * calculating PI at compile-time. 
 *
 * I use BBP to approximate PI. */

// Define float_t as the type of floating-point type to use.
/* GNU C specific */
#ifdef __GNUC__
  #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
  #if GCC_VERSION >= 40600
    #include <quadmath.h>
    typedef __float128 float_t;
    #define __USING_FLOAT128__
  #else
    #define long double float_t;
  #endif
#else
  typedef long double float_t;
#endif

template <unsigned int N>
struct const_float
{
  static constexpr float_t value = N;
};

typedef const_float<0> cf0;
typedef const_float<1> cf1;
typedef const_float<2> cf2;
typedef const_float<4> cf4;

template <unsigned long long K>
struct exponent_16
{
  static constexpr float_t value = 16 * exponent_16<K - 1>::value;
};

template <>
struct exponent_16<0>
{
  static constexpr float_t value = 1.0;
};

template <unsigned long long Precision>
struct pi
{
  static constexpr float_t value = (cf1::value / exponent_16<Precision>::value) * 
                                      (cf4::value / (8 * Precision + 1) -
                                       cf2::value / (8 * Precision + 4) -
                                       cf1::value / (8 * Precision + 5) -
                                       cf1::value / (8 * Precision + 6)) +
                                    std::conditional<Precision != 0, pi<Precision - 1>, cf0>::type::value;
};

int main()
{
  // 3.1415926 53589793 23846264 33832795 02|884197, where | is the maximum I could get
  // Precise to around 19 or 33 digits, depending on your compiler and system.
  float_t val = pi<25>::value;
#ifdef __USING_FLOAT128__
  char output[36];
  quadmath_snprintf(output, 36, "%-#*.33Qf", 33, val);
  std::cout << output << std::endl;
#else
  std::cout << std::setprecision(19) << val << std::endl;
#endif

  return 0;
}