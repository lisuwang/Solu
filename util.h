#ifndef UTIL_H
#define UTIL_H
#include <cassert>
#include <algorithm>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#define assertm(exp, msg) assert(((void)msg, exp))

namespace RobotPose
{
using LonType = double;
using LatType = double;
using HeadingType = double;
using VxSpeed = double;
using VySpeed = double;
using AccSpeed = double;

using RobotPoseType = std::tuple<LonType, LatType, HeadingType>;
using RobotSpeedType = std::tuple<VxSpeed, VySpeed, AccSpeed>;
};
#endif // UTIL_H
