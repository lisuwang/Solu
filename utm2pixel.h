#ifndef UTM2PIXEL_H
#define UTM2PIXEL_H
#include <vector>
#include <math.h>
/*According to the resolution of the map, calculate the relation
between from pixel to UTM, please the see the below picture.
                              Y(North)
                              ^
                              |
                              |       .(4410305.111, 715858.888=>x,y)
                              |
                              |
                              |
(0,0=>4410207.479, 715848.059)|----------------------------->X (East)

The map have four param:
1. UTM cordinate
2. Direction
    Right-Down , in short RD
    Right-Up,    in short RU
    Left-Down,   in short LD
    Left-Up,     in short LU
3. Resolution , 1 pixel = 0.05M, 5cm
                1 pixel = 0.02M, 2cm
                1 pixel = 0.01M, 1cm
4. Map size, Length, Width
Initialize a UTM2Pixel object must have up four param
*/
class UTM2Pixel
{
public:
    UTM2Pixel();
    struct MapPoint{
        int x, y;
    };
    struct UTMPoint{
        double E; //X
        double N; //Y
        double Z;
    };
    enum Direction{
        RD = 1,
        RU = 2,
        LD = 3,
        LU = 4
    };
    enum Resolution{
        CM10 = 1,
        CM5  = 2,
        CM2  = 3,
        CM1  = 4
    };
    struct Size{
        double Length;
        double Width;
    };
    MapPoint FromUTMPoint(double E, double N);
    UTMPoint FromMapPoint(int x, int y);
    std::vector<MapPoint> FromUTMPoints(std::vector<UTMPoint> const&);
    std::vector<UTMPoint> FromMapPoints(std::vector<MapPoint> const&);
    using Distance = double;
    Distance MaxDistanceAnyPoint(const std::vector<UTMPoint> & UTMPoints,
                                 std::vector<UTMPoint>::iterator first ,
                                 std::vector<UTMPoint>::iterator last )const;

    UTMPoint mapStartPoint_;  //the left down corner of the map
    UTMPoint mapEndPoint_;    //the roght up corner of the map
    double onePixel_;          //one pixel equal UTM diatance
    Direction mapSurveyDirection_;  //map person to survey the map , the orgin point
    Size mapSize_;

};
UTM2Pixel makeUTM2PixelObject(double N,
                              double E,
                              double Z,
                              int direction,
                              int resolution,
                              double length,
                              double width);
#endif // UTM2PIXEL_H
