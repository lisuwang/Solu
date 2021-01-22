#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "ui_picdialog.h"
#include <QMessageBox>


#include <util.h>
#include <drawv1.h>
#include <abstructedmap.h>

#include <bestposa.h>
#include <forpublish.h>
#include <QTimer>
#include <maper.h>
static ForPublish gpsSourceObject;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    locTimer_ = new QTimer(this);
    connect(locTimer_, SIGNAL(timeout()), this, SLOT(TickLocation()));
    gpsSourceObject.transaction();
    gpsSourceObject.MainThreadReadGPSInfo();
}

MainWindow::~MainWindow()
{
    delete mapImage_;
    delete ui;
    delete locTimer_;
}

std::string MainWindow::GetBorderFilePath() const
{
    return this->ui->leBorderFile->text().toStdString();
}

std::string MainWindow::GetObstacleFilePath() const
{
    return this->ui->leObstacleFile->text().toStdString();
}

void MainWindow::ShowMenu()
{
    this->ui->menu->show();
}

bool MainWindow::checkBorderPoint() const
{
    return false;
}

bool MainWindow::checkObstaclePoint() const
{
    return false;
}

void MainWindow::popMessage(QString title, QString content)
{
    QMessageBox::critical(this, title, content);
}

void MainWindow::SetMapWidthAndLength()
{
    this->ui->teEDistance->setText(QString("%1").arg(map_.GetMapEDistanceUTM()));
    this->ui->teNDistance->setText(QString("%1").arg(map_.GetMapNDistanceUTM()));
}

void MainWindow::LoadSmallMap()
{

}

int MainWindow::GetSmallRangeWidth()
{
    return this->ui->lbSmallMap->width();
}

int MainWindow::GetSmallRangeHeight()
{
    return this->ui->lbSmallMap->height();
}

void MainWindow::on_pbtSelectBorderFile_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "请选择文件路径...", "./");
    if(file_path.isEmpty())
    {
        return;
    }
    else
    {
        this->ui->leBorderFile->setText(file_path);
    }
}

void MainWindow::on_pbtSelectObstacleFile_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "请选择文件路径...", "./");
    if(file_path.isEmpty())
    {
        return;
    }
    else
    {
        this->ui->leObstacleFile->setText(file_path);
    }
}

void MainWindow::on_pbtGenerateMap_clicked()
{

    //char* backgroundFileName = (char*)"./background.pgm";
    const cv::String backgroundFileName = "./background.pgm";
    using namespace cv;
    using namespace std;

    if(!abstructedMap::IsValidJsonMap(GetBorderFilePath().c_str()))
    {
        popMessage("错误对话框","现在选择的文件不正确，请选择正确的测量边界数据文件");
        return;
    }
    if(!abstructedMap::IsValidJsonMap(GetObstacleFilePath().c_str()))
    {
        popMessage("错误对话框","现在选择的文件不正确，请选择正确的测量障碍物数据文件");
        return;
    }
    double mapRatio = this->ui->cbMapRatio->currentText().toDouble();
    double mapMargin = this->ui->cbMapMargin->currentText().toDouble();
    abstructedMap object{GetBorderFilePath().c_str(),GetObstacleFilePath().c_str(),mapRatio, mapMargin};
    //object.SetObstacleUTMPoint(fixedObstacleUTMPoints);
    ///首先根据现场的测量数据，生成底图，长宽动态变化 M*M的黑底图
    /// 创建一个图形
    int XDistance = static_cast<int>(floor(object.mapEDistance_ / object.onePixel_));
    int YDistance = static_cast<int>(floor(object.mapNDistance_ / object.onePixel_));
    img_write(img_creat(const_cast<char*>(backgroundFileName.c_str()),YDistance, XDistance), const_cast<char*>(backgroundFileName.c_str()));

    Mat img_copy_hongxi = imread(backgroundFileName,0);
    object.SetObstalceOneUTMPoint(0, 0.5);
    object.SetObstalceOneUTMPoint(10, 2.5);
    object.SetObstalceOneUTMPoint(22, 5.0);
    object.SetObstalceOneUTMPoint(26, 0.2);
    //标定出车可以去的范围
    drawV1::DrawContourWithUTM(img_copy_hongxi, object);
    //标定现场障碍物的位置
    drawV1::DrawFixedObstaclePointsWithUTM(img_copy_hongxi, object);
    //生成地图
    mapFile_ = "hongxi.pgm";
    smallMapFile_ = "hongxi_small.pgm";
    imwrite(mapFile_.toStdString(), img_copy_hongxi);
    this->fullMapMat_ = img_copy_hongxi;
    map_ = object;
    this->SetMapWidthAndLength();
    int width = this->GetSmallRangeWidth();
    int height = this->GetSmallRangeHeight();
    double mapWidth = object.GetMapEDistancePixel();
    double mapHeight = object.GetMapNDistancePixel();
    double widthScale = width / mapWidth;
    double heightScale = height / mapHeight;
    Mat img_small_hongxi = img_copy_hongxi;

    img_small_hongxi = drawV1::DrawImgInRectangleRange(img_small_hongxi, widthScale, heightScale, object);
    imwrite(smallMapFile_.toStdString(), img_small_hongxi);
    this->smallMapMat_ = img_small_hongxi;
    this->mapImage_ = new QPixmap(smallMapFile_);
    this->ui->lbSmallMap->setPixmap(*mapImage_);
    this->ui->lbSmallMap->show();
    this->locTimer_->start(1000);
}

void MainWindow::on_action_triggered()
{
    this->picUI = new PicDialog(this);
    this->picUI->SetPgmFile(mapFile_);
    //this->picUI->mainWindow_ = this;
    picUI->setWindowState(Qt::WindowMaximized);
    picUI->show();
    picUI->LabelFillBackground();
}

void MainWindow::TickLocation()
{
    gpsSourceObject.MainThreadReadGPSInfo();
    GPSInfo info = gpsSourceObject.GetGPSInfo();
    RobotPose::RobotPoseType pose;
    this->picUI->UpdateRobotLocation(pose);
    drawV1::DrawTrajectoryPointsWithUTM(this->fullMapMat_, this->map_,
                                        std::make_tuple(info.bestposa_.lon,
                                                        info.bestposa_.lat,
                                                        info.headinga_.heading),
                                        cv::Scalar(50,50,50));
    QPixmap pixmap = drawV1::MatToPixmap(this->fullMapMat_);
    this->picUI->SetPixmap(pixmap);
    this->picUI->LableFillPixmap();

}
