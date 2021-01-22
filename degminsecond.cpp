#include "degminsecond.h"
#include <math.h>

DegMinSecond::DegMinSecond(double deg)
{
    oneSegFormat_.deg_ = deg;
    twoSegFormat_.deg_ = deg;
    twoSegFormat_.min_ = 60*(deg - int(deg));
    threeSegFormat_.deg_ = deg;
    threeSegFormat_.min_ = int(twoSegFormat_.min_);
    threeSegFormat_.second_ = 60*(twoSegFormat_.min_ - threeSegFormat_.min_);

}

DegMinSecond::DegMinSecond(int deg, double min)
{

    twoSegFormat_.deg_ = deg;
    twoSegFormat_.min_ = min;
    oneSegFormat_.deg_ = deg + (min/60)*100;
    threeSegFormat_.deg_ = deg;
    threeSegFormat_.min_ = int((oneSegFormat_.deg_ - deg)*60);
    threeSegFormat_.second_ = 60 * (twoSegFormat_.min_ - threeSegFormat_.min_ );


}

DegMinSecond::DegMinSecond(int deg, int min, double second)
{
    oneSegFormat_.deg_ = deg + (double)min / 60 + second / 3600;
    twoSegFormat_.deg_ = deg;
    twoSegFormat_.min_ = min + second / 60;
    threeSegFormat_.deg_ = deg;
    threeSegFormat_.min_ = min;
    threeSegFormat_.second_ = second;
}


std::string DegMinSecond::OutPutDegFormat() const
{
    std::string retStr;
    retStr += std::to_string(oneSegFormat_.deg_);
    return retStr;
}

std::string DegMinSecond::OutPutDegMinFormat() const
{
    std::string retStr;
    retStr += std::to_string(twoSegFormat_.deg_);
    retStr += ":";
    retStr += std::to_string(twoSegFormat_.min_);
    return retStr;
}

std::string DegMinSecond::OutPutDegMinSecondFormat() const
{
    std::string retStr;
    retStr += std::to_string(threeSegFormat_.deg_);
    retStr += ":";
    retStr += std::to_string(threeSegFormat_.min_);
    retStr += ":";
    retStr += std::to_string(threeSegFormat_.second_);
    return retStr;
}


