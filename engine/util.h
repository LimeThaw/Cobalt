#ifndef UTIL_H
#define UTIL_H

#include <cassert>
#include <cmath>

#define DEG_TO_RAD 0.017453294

template<typename T>
T clamp(const T &val, const T &min, const T &max) {
    if(val > max) {
        return max;
    } else if(val < min) {
        return min;
    } else {
        return val;
    }
}

inline unsigned char normalized_float_to_byte(float x) {
    assert(0 <= x && x <= 1);
    return (unsigned char) (x * 255);
}

template<typename T>
T log2(T x) {
    return std::log(x) / std::log(2);
}

#endif
