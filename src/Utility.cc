#include "Utility.h"


Interpolation::Interpolation()
{
}

Interpolation::~Interpolation()
{
}

template<class XAXIS, class YAXIS>
std::pair<XAXIS, YAXIS> Interpolation::interpolationOnYieldCurve(
        std::pair<XAXIS, YAXIS>& p1,
        std::pair<XAXIS, YAXIS>& p2)
{
    return std::pair<XAXIS, YAXIS>(XAXIS(NULL), YAXIS(NULL));
}
