#ifndef ABSTRUCTEDMAP_H
#define ABSTRUCTEDMAP_H

#include <vector>
#include <math.h>
#include <tinycomponent.h>


/*According to the resolution of the map, calculate the relation
between from pixel to UTM, please the see the below picture.
                              Y(North)
                              ^
                              |
                              |       .(a0=4410305.111, b0=715858.888)
                              |   .(a1,b1)   .(a2,b2)
                              |
                              |      .(a3,b3)
(0,0=>4410207.479, 715848.059)|----------------------------->X (East)

The map have four param:
1. UTM coordinate
2. Map Right-Up corner and Left-down corner
   According to the border coordinate, the (Emax,Nmax) is the right-up corder,
   and the (Emin, Nmin) is the left-down corder
3. Resolution , 1 pixel = 0.1M , 10cm
                1 pixel = 0.05M, 5cm
                1 pixel = 0.02M, 2cm
                1 pixel = 0.01M, 1cm
4. Map size, Length, Width
Initialize a abstructedMap object must have up four param
*/


class BaseMap
{

};
class abstructedMap : public BaseMap
{
public:
    static bool IsValidJsonMap(const char* );
    abstructedMap() ;
    void RealConstructor(double Emin, double Nmin,
                  double Emax, double Nmax,
                  double resolution);

    void constructFromTwoPoint(const UTMPoint left_down, const UTMPoint right_up,
                  double resolution);
    void constructFromBorderPoints(const std::vector<UTMPoint> borderPoints,
                  double resolution);
    abstructedMap(const char* borderJsonFile,
                  const char* obstacleJsonFile,
                  double resolution,
                  double mapMargin = 0);

    UTMPoint FindMaxBorderUTMPoint(const std::vector<UTMPoint> borderPoints)const;
    UTMPoint FindMinBorderUTMPoint(const std::vector<UTMPoint> obstaclePoints)const;
    void SetObstacleUTMPoint(const char* obstacleJsonFile);
    void SetObstacleUTMPoint(const std::vector<UTMPoint> obstaclePoints, double radius = 1.50);
    void SetObstalceOneUTMPoint(int index, double newRadius);
    void SetRobotStartPoint(UTMPoint point, double angle);
    void SetRealRegionStartUTMPoint(const UTMPoint left_down);
    void SetRealRegionEndUTMPoint(const UTMPoint right_up);
    void SetMapStartUTMPoint(double distanceToRealRegion);
    void SetMapEndUTMPoint(double distanceToRealRegion);
    MapPoint FromUTMPoint(double E, double N)const;
    UTMPoint FromMapPoint(int x, int y)const;
    std::vector<MapPoint> FromUTMPoints(std::vector<UTMPoint> const&)const;
    std::vector<UTMPoint> FromMapPoints(std::vector<MapPoint> const&)const;
    using Distance = double;
    CircleObstacleRegion FindTheNearestObstacle(MapPoint const& mapPoint)const;
    CircleObstacleRegion FindTheNearestObstacle(int x , int y)const;
    Distance MaxDistanceAnyPoint(const std::vector<UTMPoint> & UTMPoints,
                                 std::vector<UTMPoint>::iterator first ,
                                 std::vector<UTMPoint>::iterator last )const;
    abstructedMap::Distance GetMapEDistanceUTM()const;
    abstructedMap::Distance GetMapNDistanceUTM()const;
    abstructedMap::Distance GetRealRegionEDistanceUTM()const;
    abstructedMap::Distance GetRealRegionNDistanceUTM()const;
    void SetMapRealRegionToMapMargin(double );
    int GetMapEDistancePixel()const;
    int GetMapNDistancePixel()const;
    int GetRealRegionEDistancePixel()const;
    int GetRealRegionNDistancePixel()const;
    double MaxRealRegionUTMX_; //cache the survey UTMX max value
    double MinRealRegionUTMX_; //cache the survey UTMX min value
    double MaxRealRegionUTMY_; //cache the survey UTMY max value
    double MinRealRegionUTMY_; //cache the survey UTMY min value

    std::vector<UTMPoint> realRegionBorderPoints_; //the survey border points
    std::vector<CircleObstacleRegion> obstacleRegions_; //the standard obstacle objects

    UTMPoint mapStartPoint_;  //the left down corner of the map
    UTMPoint mapEndPoint_;    //the roght up corner of the map
    UTMPoint realRegionStartPoint_;  //the left down corner of the survey region
    UTMPoint realRegionEndPoint_;    //the right up corner of the survey region

    double onePixel_;          //one pixel equal UTM diatance

    Distance mapEDistance_;    //the whole map E distance, positive value
    Distance mapNDistance_;    //the whole map N distance, positive value
    Distance realRegionEDistance_; //the real region E distance, positive value
    Distance realRegionNDistance_; //the real region N distance, positive value

    UTMPoint robotStartPoint_; //robot start point in map
    double robotStartAngle_;   //robot thelta to "zhen" North

    double realRegionAreaSize_; //survey area
    double mapAreaSize_;        //the whole map area
    double mapRealRegionToMarginDistance_ = 2; //the real region to map margin distance

};

#endif // ABSTRUCTEDMAP_H
