#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "picdialog.h"
#include <abstructedmap.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    std::string GetBorderFilePath()const;
    std::string GetObstacleFilePath()const;
    void ShowMenu();
    bool checkBorderPoint()const;
    bool checkObstaclePoint()const;
    void popMessage(QString title, QString content);
    void SetMapWidthAndLength();
    void LoadSmallMap();
    int GetSmallRangeWidth();
    int GetSmallRangeHeight();

private slots:
    void on_pbtSelectBorderFile_clicked();

    void on_pbtSelectObstacleFile_clicked();

    void on_pbtGenerateMap_clicked();

    void on_action_triggered();

    void TickLocation();

private:
    Ui::MainWindow *ui;
    PicDialog* picUI;

    QString mapFile_;
    QString smallMapFile_;
    double EMargin_;
    double NMargin_;
    double mapResolution_;
    abstructedMap map_;
    QPixmap* mapImage_; //save samll map
    QTimer* locTimer_;

    cv::Mat fullMapMat_;
    cv::Mat smallMapMat_;
    cv::Mat middleMapMat_;
};

#endif // MAINWINDOW_H
