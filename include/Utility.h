#ifndef _INCLUDE_UTILITY_H_
#define _INCLUDE_UTILITY_H_

#include <utility>

class Date;

class Interpolation
{
    Interpolation();
    ~Interpolation();

    template<class XAXIS, class YAXIS> static inline
    std::pair<XAXIS, YAXIS> linearInterpolation(
            std::pair<XAXIS, YAXIS>& p1,
            std::pair<XAXIS, YAXIS>& p2,
            XAXIS& xVal); 
};


template<class XAXIS, class YAXIS>
std::pair<XAXIS, YAXIS> Interpolation::linearInterpolation(
        std::pair<XAXIS, YAXIS>& p1,
        std::pair<XAXIS, YAXIS>& p2,
        XAXIS& xVal)
{
    return std::pair<XAXIS, YAXIS>(XAXIS(xVal),
            YAXIS(
                ((p2.second - p1.second) / (p2.first - p1.first)) * 
                (xVal - p1.first)
                )
            );
}

#endif // _INCLUDE_UTILITY_H_
