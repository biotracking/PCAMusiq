#ifndef MATHUTIL_H
#define MATHUTIL_H


#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define CLAMP(value, low, high) MAX(low, MIN(high, value))

#endif // MATHUTIL_H
