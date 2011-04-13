#ifndef _INCLUDE_UTILITY_H_
#define _INCLUDE_UTILITY_H_

#include <utility>

#include "Date.h"

struct Interpolation
{
    Interpolation();
    ~Interpolation();

    static inline double
    linearInterpolation(
            const std::pair<Date, double>& p1,
            const std::pair<Date, double>& p2,
            const Date& xVal)
    {
        Duration xToP1 = xVal - p1.first;
        Duration p2ToP1 = p2.first - p1.first;

        return xToP1.getDuration(Duration::DAY) * 
            (p2.second - p1.second) / p2ToP1.getDuration(Duration::DAY);
    }

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
