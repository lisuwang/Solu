#include "utm2pixel.h"
#include <vector>
#include <string>
#include <exception>
#include <cassert>
#include <util.h>

UTM2Pixel::UTM2Pixel()
{

}

UTM2Pixel::MapPoint UTM2Pixel::FromUTMPoint(double E, double N)
{
    assertm(E < mapEndPoint_.E, "current E less than the start E");
    assertm(N < mapEndPoint_.N, "current N less than the start N");
    assertm(E > mapStartPoint_.E, "current E greater than the end E");
    assertm(N > mapStartPoint_.N, "current N greater than the end N");
    double minusX = E - mapStartPoint_.E;
    double minusY = N - mapStartPoint_.N;
    double priX = minusX / (double)onePixel_;
    double priY = minusY / (double)onePixel_;
    return UTM2Pixel::MapPoint{int(floor(priX+0.5)), int(floor(priY+0.5))};
}

UTM2Pixel::UTMPoint UTM2Pixel::FromMapPoint(int x, int y)
{
    assert(x <= mapSize_.Length/onePixel_ && y <= mapSize_.Width/onePixel_
             &&
             x >=0 && y >= 0);
    return UTM2Pixel::UTMPoint{mapStartPoint_.E+x*onePixel_,
                mapStartPoint_.N+y*onePixel_, 0};
}

std::vector<UTM2Pixel::MapPoint> UTM2Pixel::FromUTMPoints(const std::vector<UTM2Pixel::UTMPoint> & UTMPoints)
{
   std::vector<UTM2Pixel::MapPoint> ret ;
   auto add = [&ret, this](const UTM2Pixel::UTMPoint& it){
       ret.push_back(FromUTMPoint(it.E, it.N));
   };
    std::for_each(UTMPoints.begin(), UTMPoints.end(),add);
    return ret;
}

std::vector<UTM2Pixel::UTMPoint> UTM2Pixel::FromMapPoints(const std::vector<UTM2Pixel::MapPoint> & mapPoints)
{
    std::vector<UTM2Pixel::UTMPoint> ret;
    auto add = [&ret, this](const UTM2Pixel::MapPoint& it){
      ret.push_back(FromMapPoint(it.x, it.y));
    };
    std::for_each(mapPoints.begin(), mapPoints.end(), add);
    return ret;
}

UTM2Pixel::Distance UTM2Pixel::MaxDistanceAnyPoint(const std::vector<UTM2Pixel::UTMPoint> &UTMPoints,
                                                   std::vector<UTM2Pixel::UTMPoint>::iterator first,
                                                   std::vector<UTM2Pixel::UTMPoint>::iterator last) const
{
    static UTM2Pixel::Distance maxDis = 0;
    if(std::distance(first, last)==2)
    {
        UTM2Pixel::Distance lastDis = std::sqrt(std::pow((first->E - last->E),2)+std::pow((first->N - last->N),2));
        maxDis < lastDis ? maxDis = lastDis : lastDis+=0.1 ;
        return maxDis;
    }
    ++first;
    for(auto it = first; it != last; ++it)
    {
        if(it+1<=last)
        {
            UTM2Pixel::Distance tmpDis= std::sqrt(std::pow((first->E - it->E),2)+std::pow((first->N - it->N),2));
            if(maxDis < tmpDis)
            {
                maxDis = tmpDis;
            }

        }
        else
        {
            return maxDis;
        }
    }
    return MaxDistanceAnyPoint(UTMPoints, ++first, last);

}


UTM2Pixel makeUTM2PixelObject(double E, double N, double Z, int direction, int resolution, double length, double width)
{
    if(N < 0 || E <0 || Z < 0 )
        throw std::string("utm coordiniate is negative!!!");
    if(length < 0 || width < 0)
        throw "space length and width is negative!!!";
    if(length < 50 || width < 50)
        throw "space length and width is too small";

    if(direction > 4 || direction < 1)
        throw "the map start point only support 4 direction , LU, LD, RU, RD";

    if(resolution > 4 || resolution < 1)
        throw "the map only support 4 resolution , 10cm = 1, 5cm = 2, 2cm = 3, 1cm = 4";
    int controllerWayX = 1;
    int controllerWayY = 1;
    UTM2Pixel object;
    switch(resolution)
    {
        case 1:
            object.onePixel_ = 0.1;
            break;
        case 2:
            object.onePixel_ = 0.05;
            break;
        case 3:
            object.onePixel_ = 0.02;
            break;
        case 4:
            object.onePixel_ = 0.01;
            break;
        default:
            break;
    }
    switch(direction)
    {
        case 1:
            object.mapSurveyDirection_ = UTM2Pixel::RD;
            controllerWayX = -1;
            controllerWayY = 1;
            object.mapStartPoint_ = {E + controllerWayX*(length), N, Z};
            object.mapEndPoint_ = {E, N + controllerWayY*(width), Z};
            break;
        case 2:
            object.mapSurveyDirection_ = UTM2Pixel::RU;
            controllerWayX = -1;
            controllerWayY = -1;
            object.mapStartPoint_ = {E + controllerWayX*(length),
                                     N + controllerWayY*(width), Z};
            object.mapEndPoint_ = {E, N, Z};
            break;
        case 3:
            object.mapSurveyDirection_ = UTM2Pixel::LD;
            controllerWayX = 1;
            controllerWayY = 1;
            object.mapStartPoint_ = {E, N, Z};
            object.mapEndPoint_ = {E + controllerWayX*(length),
                                   N + controllerWayY*(width), Z};
            break;
        case 4:
            object.mapSurveyDirection_ = UTM2Pixel::LU;
            controllerWayX = 1;
            controllerWayY = -1;
            object.mapStartPoint_ = {E, N + controllerWayY*(width), Z};
            object.mapEndPoint_ = {E + controllerWayX*(length), N, Z};
            break;
        default:
            break;
    }
    object.mapSize_ = {length, width};
    return object;
}
