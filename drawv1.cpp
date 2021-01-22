#ifndef DRAWV1_H
#define DRAWV1_H
#include <drawv1.h>
#include <tinycomponent.h>
#include <vector>
#include <util.h>
#include <maper.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <abstructedmap.h>
#include <Gps2UTM.h>
#include <tuple>
#include <QPixmap>

namespace drawV1{

cv::Mat DrawImgInRectangleRange(cv::Mat img, double widthScale, double heightScale, const abstructedMap& object)
{
    using namespace std;
    using namespace cv;
    cv::Mat retMat;
    cv::resize(img, retMat, cv::Size(), widthScale, heightScale, cv::INTER_LINEAR);
    return retMat;
}
void DrawContourWithUTM(cv::Mat img, const abstructedMap& object, cv::Scalar color = cv::Scalar(255,255,255))
{
    using namespace std;
    using namespace cv;
    const int times = static_cast<int>(object.realRegionBorderPoints_.size());
    Point testP[1][times-1];
    vector<MapPoint> mapPoint = object.FromUTMPoints(object.realRegionBorderPoints_);
    for(int i = 0 ; i < times; ++i)
    {
        testP[0][i] = Point(mapPoint[static_cast<std::vector<MapPoint>::size_type>(i)].x,
                mapPoint[static_cast<std::vector<MapPoint>::size_type>(i)].y);
    }
    const Point* pp[1] = {testP[0]};
    int np[] = {times-1};

    fillPoly(img, pp, np, 1, color,8);
}
void DrawFixedObstaclePointsWithUTM(cv::Mat img, abstructedMap object,  cv::Scalar color )
{
    using namespace std;
    using namespace cv;
    std::vector<UTMPoint> obstaclePoints;
    std::vector<double> obstacleRadius;
    foreach (auto x, object.obstacleRegions_) {
        obstaclePoints.push_back(x.circleCenter);
        obstacleRadius.push_back(x.radius);
    }
    vector<MapPoint> mapPoint = object.FromUTMPoints(obstaclePoints);
    abstructedMap::Distance ratio = object.onePixel_;
    assertm(ratio > 0 , "ratio must larger than 0");
    assertm(ratio < 0.11 , "ratio must less than 0.11");
    //Caution:
    //There is possible a bug in below code, the circe border is out of the border
    std::vector<MapPoint>::size_type index = 0;
    for(auto x : mapPoint){
        circle(img, Point(x.x, x.y), int(floor(obstacleRadius[index++]/ratio)),color, -1);
    }
}

void DrawTrajectoryPointsWithUTM(cv::Mat img, abstructedMap object, std::tuple<double, double, double> pose, cv::Scalar color )
{
    using namespace std;
    using namespace cv;
    double lon = std::get<0>(pose);
    double lat = std::get<1>(pose);
    double heading = std::get<2>(pose);
    int zone = (int)floor((lon + 180.0) / 6) + 1;
    double xy[2];
    xy[0] = 0.0;
    xy[1] = 0.0;
    RobotHelper::LatLonToUTMXY(RobotHelper::DegToRad(lat), RobotHelper::DegToRad(lon), zone, xy, 2);

    UTMPoint utmPoint;
    utmPoint.E = xy[0];
    utmPoint.N = xy[1];

    MapPoint mapPoint = object.FromUTMPoint(utmPoint.E, utmPoint.N);
    abstructedMap::Distance ratio = object.onePixel_;
    assertm(ratio > 0 , "ratio must larger than 0");
    assertm(ratio < 0.11 , "ratio must less than 0.11");
    //Caution:
    //
    circle(img, Point(mapPoint.x, mapPoint.y), int(floor(1.5/ratio)),color, -1);
}

QPixmap MatToPixmap(cv::Mat src)
{
    QImage::Format format=QImage::Format_Grayscale8;
    int bpp=src.channels();
    if(bpp==3)format=QImage::Format_RGB888;
    QImage img(src.cols,src.rows,format);
    uchar *sptr,*dptr;
    int linesize=src.cols*bpp;
    for(int y=0;y<src.rows;y++){
        sptr=src.ptr(y);
        dptr=img.scanLine(y);
        memcpy(dptr,sptr,(std::size_t)linesize);
    }
    if(bpp==3)return QPixmap::fromImage(img.rgbSwapped());
    return QPixmap::fromImage(img);
}
}
#endif // DRAWV1_H
