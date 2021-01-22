#ifndef PICDIALOG_H
#define PICDIALOG_H

#include <QDialog>
#include <abstructedmap.h>
#include <util.h>
namespace Ui {
class PicDialog;
}
//using MainWindow;
class PicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PicDialog(QWidget *parent = 0);
    ~PicDialog();

    void LabelFillBackground();
    void LableFillPixmap();
    void SetPgmFile(const QString& strFile);
    void SetPixmap(QPixmap& map);
    void UpdateRobotLocation(RobotPose::RobotPoseType goalPose);
    void UpdateRobotSpeed(RobotPose::RobotSpeedType speed);
private:
    Ui::PicDialog *ui;
    QPixmap* mapImage_;

    struct RobotInfo
    {
        RobotPose::RobotPoseType currentPose_;
        RobotPose::RobotSpeedType speed_;
    };
    RobotInfo robotInfo_;
    //MainWindow* mainWindow_;
};

#endif // PICDIALOG_H
