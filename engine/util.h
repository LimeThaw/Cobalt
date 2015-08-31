#ifndef UTIL_H
#define UTIL_H

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

#endif
