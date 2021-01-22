#ifndef TINYCOMPONENT_H
#define TINYCOMPONENT_H
struct MapPoint{
    int x, y;
};
struct UTMPoint{
    double E; //X
    double N; //Y
    double Z;
};
struct CircleObstacleRegion
{
    UTMPoint circleCenter;
    double radius;
};
#endif // TINYCOMPONENT_H
