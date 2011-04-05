#ifndef _INCLUDE_UTILITY_H_
#define _INCLUDE_UTILITY_H_

#include <utility>

class Date;

struct Interpolation
{
    Interpolation();
    ~Interpolation();

    template<class XAXIS, class YAXIS> static inline
    YAXIS linearInterpolation(
            const std::pair<XAXIS, YAXIS>& p1,
            const std::pair<XAXIS, YAXIS>& p2,
            const XAXIS& xVal); 
};


template<class XAXIS, class YAXIS>
YAXIS Interpolation::linearInterpolation(
        const std::pair<XAXIS, YAXIS>& p1,
        const std::pair<XAXIS, YAXIS>& p2,
        const XAXIS& xVal)
{
    return YAXIS(
            ((p2.second - p1.second) / (YAXIS)(p2.first - p1.first)) * 
              (YAXIS)(xVal - p1.first));
}

#endif // _INCLUDE_UTILITY_H_
