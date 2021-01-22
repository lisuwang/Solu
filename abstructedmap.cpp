#include "abstructedmap.h"
#include <math.h>
#include <algorithm>
#include <limits>
#include <jsonhelper.h>

bool abstructedMap::IsValidJsonMap(const char* JsonFile)
{
    return !JsonHelper::GetUTMFromJSONFile(JsonFile).empty();
}

abstructedMap::abstructedMap()
{

}

void abstructedMap::RealConstructor(double Emin, double Nmin, double Emax, double Nmax, double resolution)
{
    if(Nmin < 0 || Nmax < 0 || Emin <0 || Emax < 0 )
        throw std::string("utm coordiniate is negative!!!");

    if(resolution > 0.2 || resolution < 0.01)
        throw "the map only support 4 resolution , one pixel = 10cm , 5cm , 2cm , 1cm ";
    switch(int(100*resolution))
    {
    case 10:
        onePixel_ = 0.1;
        break;
    case 5:
        onePixel_ = 0.05;
        break;
    case 2:
        onePixel_ = 0.02;
        break;
    case 1:
        onePixel_ = 0.01;
        break;
    default:
        throw "the map only support 4 resolution , 10cm = 1, 5cm = 2, 2cm = 3, 1cm = 4";
        break;
    }
    double maxEDistance = Emax - Emin;
    double maxNDistance = Nmax - Nmin;
    assertm(maxEDistance > 50, "The map EDistance is too small , must > 50M");
    assertm(maxNDistance > 50, "The map NDistance is too small , must > 50M");

    this->realRegionAreaSize_ = maxEDistance * maxNDistance;
    this->MinRealRegionUTMX_ = Emin;
    this->MaxRealRegionUTMX_ = Emax;
    this->MinRealRegionUTMY_ = Nmin;
    this->MaxRealRegionUTMY_ = Nmax;
    this->realRegionStartPoint_ = {this->MinRealRegionUTMX_, this->MinRealRegionUTMY_,0};
    this->realRegionEndPoint_ = {this->MaxRealRegionUTMX_, this->MaxRealRegionUTMY_,0};;
    this->realRegionEDistance_ = maxEDistance;
    this->realRegionNDistance_ = maxNDistance;

    //the map start move 2 meters respective
    this->mapStartPoint_.E = Emin - this->mapRealRegionToMarginDistance_; //there is possible a subtle bug when the E is in from zero point
    this->mapStartPoint_.N = Nmin - this->mapRealRegionToMarginDistance_; //there is possible a subtle bug when the E is in from zero point
    this->mapEndPoint_.E = Emax + this->mapRealRegionToMarginDistance_;
    this->mapEndPoint_.N = Nmax + this->mapRealRegionToMarginDistance_;
    this->mapEDistance_ = this->realRegionEDistance_ + 2*this->mapRealRegionToMarginDistance_;
    this->mapNDistance_ = this->realRegionNDistance_ + 2*this->mapRealRegionToMarginDistance_;
    this->mapAreaSize_ = (this->mapEDistance_) * (this->mapNDistance_);

}

void abstructedMap::constructFromTwoPoint(const UTMPoint left_down, const UTMPoint right_up, double resolution)
{
    RealConstructor(left_down.E, left_down.N, right_up.E, right_up.N, resolution);
}

void abstructedMap::constructFromBorderPoints(const std::vector<UTMPoint> borderPoints, double resolution)
{
    this->realRegionBorderPoints_ = borderPoints;
    constructFromTwoPoint(FindMinBorderUTMPoint(borderPoints),
                          FindMaxBorderUTMPoint(borderPoints),
                          resolution);
}

abstructedMap::abstructedMap(const char *borderJsonFile, const char* obstacleJsonFile, double resolution, double mapMargin)
{
    if(mapMargin - 0 > 0.0000000001)
    {
        this->mapRealRegionToMarginDistance_ = mapMargin;
    }
    constructFromBorderPoints(JsonHelper::GetUTMFromJSONFile(borderJsonFile), resolution);
    SetObstacleUTMPoint(obstacleJsonFile);
}

UTMPoint abstructedMap::FindMaxBorderUTMPoint(const std::vector<UTMPoint> borderPoints) const
{
    assertm(borderPoints.size() >= 3, "The border point at least has 3 points");
    double tempEmax = -0.1;
    double tempNmax = -0.1;
    for (auto x : borderPoints) {
        if(x.E > tempEmax)
        {
            tempEmax = x.E;
        }
        if(x.N > tempNmax)
        {
            tempNmax = x.N;
        }
    }
    return UTMPoint{tempEmax, tempNmax, 0};
}

UTMPoint abstructedMap::FindMinBorderUTMPoint(const std::vector<UTMPoint> borderPoints) const
{
    assertm(borderPoints.size() >= 3, "The border point at least has 3 points");
    double tempEmin = std::numeric_limits<double>::max();
    double tempNmin = std::numeric_limits<double>::max();
    for (auto x : borderPoints) {
        if(x.E < tempEmin)
        {
            tempEmin = x.E;
        }
        if(x.N < tempNmin)
        {
            tempNmin = x.N;
        }
    }
    return UTMPoint{tempEmin, tempNmin, 0};
}

void abstructedMap::SetObstacleUTMPoint(const char *obstacleJsonFile)
{
    std::vector<UTMPoint> obstaclePoints = JsonHelper::GetUTMFromJSONFile(obstacleJsonFile);
    SetObstacleUTMPoint(obstaclePoints);
}

void abstructedMap::SetObstacleUTMPoint(const std::vector<UTMPoint> obstaclePoints, double radius)
{
    for (auto x : obstaclePoints)
    {
        CircleObstacleRegion tmp{x, radius};
        this->obstacleRegions_.push_back(tmp);
    }
}

void abstructedMap::SetObstalceOneUTMPoint(int index, double newRadius)
{
    assertm(index >= 0 || index < (int)this->obstacleRegions_.size(), "the updated obstacle point is out of map range");
    this->obstacleRegions_[index].radius = newRadius;
}

void abstructedMap::SetRobotStartPoint(UTMPoint point, double angle)
{
    this->robotStartPoint_ = point;
    this->robotStartAngle_ = angle;
}

MapPoint abstructedMap::FromUTMPoint(double E, double N)const
{
    assertm(E <= this->realRegionEndPoint_.E, "current E is NOT less than the RIGHT-UP point E");
    assertm(N <= this->realRegionEndPoint_.N, "current N is NOT less than the RIGHT-UP point N");
    assertm(E >= this->realRegionStartPoint_.E, "current E is NOT greater than the LEFT_DOWN point E");
    assertm(N >= this->realRegionStartPoint_.N, "current N is NOT greater than the LEFT_DOWN point N");
    double minusX = E - this->mapStartPoint_.E;
    double minusY = N - this->mapStartPoint_.N;
    double priX = minusX / (double)onePixel_;
    double priY = minusY / (double)onePixel_;
    return MapPoint{int(floor(priX+0.5)), int(floor(priY+0.5))};
}

UTMPoint abstructedMap::FromMapPoint(int x, int y)const
{
    double utmXDistance = this->mapEDistance_;
    double utmYDistance = this->mapNDistance_;
    assertm(x <= utmXDistance/onePixel_ &&
            y <= utmYDistance/onePixel_ &&
            x >=0 && y >= 0, "the point is larger the map range or is negetive value");
    return UTMPoint{mapStartPoint_.E+x*onePixel_,
                mapStartPoint_.N+y*onePixel_, 0};
}

std::vector<MapPoint> abstructedMap::FromUTMPoints(const std::vector<UTMPoint> & UTMPoints)
const{
    std::vector<MapPoint> ret ;
    auto add = [&ret, this](const UTMPoint& it){
        ret.push_back(FromUTMPoint(it.E, it.N));
    };
    std::for_each(UTMPoints.begin(), UTMPoints.end(),add);
    return ret;
}

std::vector<UTMPoint> abstructedMap::FromMapPoints(const std::vector<MapPoint> & mapPoints)
const{
    std::vector<UTMPoint> ret;
    auto add = [&ret, this](const MapPoint& it){
        ret.push_back(FromMapPoint(it.x, it.y));
    };
    std::for_each(mapPoints.begin(), mapPoints.end(), add);
    return ret;
}
CircleObstacleRegion abstructedMap::FindTheNearestObstacle(const MapPoint &mapPoint) const
{
    return FindTheNearestObstacle(mapPoint.x, mapPoint.y);
}
CircleObstacleRegion abstructedMap::FindTheNearestObstacle(int x, int y) const
{
    UTMPoint utmPoint = FromMapPoint(x, y);
    CircleObstacleRegion ret;
    auto distance = [&ret, &utmPoint, this](const CircleObstacleRegion& it) {
        if(pow((utmPoint.E - it.circleCenter.E),2)+pow((utmPoint.N - it.circleCenter.N),2) < it.radius * it.radius)
        {
            ret = it;
        }
    };
    std::for_each(this->obstacleRegions_.begin(),
                  this->obstacleRegions_.end(),
                  distance);
    return ret;
}

abstructedMap::Distance abstructedMap::MaxDistanceAnyPoint(const std::vector<UTMPoint> &UTMPoints, std::vector<UTMPoint>::iterator first, std::vector<UTMPoint>::iterator last) const
{
    static abstructedMap::Distance maxDis = 0;
    if(std::distance(first, last)==2)
    {
        abstructedMap::Distance lastDis = std::sqrt(std::pow((first->E - last->E),2)+std::pow((first->N - last->N),2));
        maxDis < lastDis ? maxDis = lastDis : lastDis+=0.1 ;
        return maxDis;
    }
    ++first;
    for(auto it = first; it != last; ++it)
    {
        if(it+1<=last)
        {
            abstructedMap::Distance tmpDis= std::sqrt(std::pow((first->E - it->E),2)+std::pow((first->N - it->N),2));
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

abstructedMap::Distance abstructedMap::GetMapEDistanceUTM() const
{
    return this->mapEDistance_;
}

abstructedMap::Distance abstructedMap::GetMapNDistanceUTM() const
{
    return this->mapNDistance_;
}

abstructedMap::Distance abstructedMap::GetRealRegionEDistanceUTM() const
{
    return this->realRegionEDistance_;
}

abstructedMap::Distance abstructedMap::GetRealRegionNDistanceUTM() const
{
    return this->realRegionNDistance_;
}

void abstructedMap::SetMapRealRegionToMapMargin(double)
{

}

int abstructedMap::GetMapEDistancePixel() const
{
    assertm(onePixel_ > 0 , "only support 0.1, 0.05, 0.02, 0.01 one pixel");
    return int(floor(mapEDistance_/onePixel_+0.5));
}

int abstructedMap::GetMapNDistancePixel() const
{
    assertm(onePixel_ > 0 , "only support 0.1, 0.05, 0.02, 0.01 one pixel");
    return int(floor(mapNDistance_/onePixel_+0.5));
}

int abstructedMap::GetRealRegionEDistancePixel() const
{
    assertm(onePixel_ > 0 , "only support 0.1, 0.05, 0.02, 0.01 one pixel");
    return int(floor(realRegionEDistance_/onePixel_+0.5));
}

int abstructedMap::GetRealRegionNDistancePixel() const
{
    return int(floor(realRegionEDistance_/onePixel_+0.5));
}


