#include "picdialog.h"
#include "ui_picdialog.h"
#include <drawv1.h>

PicDialog::PicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicDialog)
{
    ui->setupUi(this);

}

PicDialog::~PicDialog()
{
    delete mapImage_;
    delete ui;
}

void PicDialog::LabelFillBackground()
{
    this->ui->scrollArea->setWidgetResizable(true);
    this->ui->scrollArea->setWindowState(Qt::WindowMaximized);
    this->ui->label->setPixmap(*mapImage_);
    this->ui->label->show();

    //this->ui->label->setAutoFillBackground(true);
}

void PicDialog::LableFillPixmap()
{
    this->ui->scrollArea->setWidgetResizable(true);
    this->ui->scrollArea->setWindowState(Qt::WindowMaximized);
    this->ui->label->setPixmap(*mapImage_);
    this->ui->label->show();
}

void PicDialog::SetPgmFile(const QString &strFile)
{
    mapImage_ = new QPixmap(strFile);

}

void PicDialog::SetPixmap(QPixmap &map)
{
    mapImage_ = &map;
}

void PicDialog::UpdateRobotLocation(RobotPose::RobotPoseType goalPose)
{
    this->robotInfo_.currentPose_ = goalPose;

}

void PicDialog::UpdateRobotSpeed(RobotPose::RobotSpeedType speed)
{
    return;
}
