#include "mainwindow.h"
#include <QApplication>


//using namespace cv;
//using namespace std;
//char* backgroundFileName = (char*)"./background.pgm";
//const char* mapObstacleUTMPointsDataPath = "./obstacle/FixUTMObstacle.json";
//const char* mapBorderUTMPointsDataPath = "./border/UTMPoints.json";
//const UTM2Pixel::Distance UTMOrginMap_N = 4405288.29747509;
//const UTM2Pixel::Distance UTMOrginMap_E = 458931.7770937295-150;
int main(int argc, char *argv[])
{
    //UTM2Pixel object = makeUTM2PixelObject(UTMOrginMap_N,UTMOrginMap_E,20.517, 3, 2, 250,250);
    //std::vector<UTMPoint> borderUTMPoints = drawV1::GetUTMFromJSONFile(mapBorderUTMPointsDataPath);
    //std::vector<UTMPoint> fixedObstacleUTMPoints = JsonHelper::GetUTMFromJSONFile(mapObstacleUTMPointsDataPath);

    QApplication a(argc, argv);

    MainWindow w;
    w.ShowMenu();
    w.show();

    return a.exec();
}
