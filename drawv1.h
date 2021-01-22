#ifndef DRAWV1_H
#define DRAWV1_H
#include <tinycomponent.h>

#include <util.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <abstructedmap.h>

#include <tuple>
#include <QPixmap>

namespace drawV1{

cv::Mat DrawImgInRectangleRange(cv::Mat img, double widthScale, double heightScale, const abstructedMap& object);
void DrawContourWithUTM(cv::Mat img, const abstructedMap& object, cv::Scalar color = cv::Scalar(255,255,255));
void DrawFixedObstaclePointsWithUTM(cv::Mat img, abstructedMap object,  cv::Scalar color = cv::Scalar(0,0,0));

void DrawTrajectoryPointsWithUTM(cv::Mat img, abstructedMap object, std::tuple<double, double, double> pose, cv::Scalar color = cv::Scalar(0,0,0));

QPixmap MatToPixmap(cv::Mat src);
}
#endif // DRAWV1_H
