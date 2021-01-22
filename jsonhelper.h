#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <util.h>
#include <tinycomponent.h>

namespace JsonHelper
{

bool IsUTMPointsEmpty(const std::vector<UTMPoint>& goals)
{
    return goals.empty();
}
std::vector<UTMPoint> GetUTMFromJSONFile(const char* filePath)
{
    std::vector<UTMPoint> UTMPoints;
    QFile load_file(filePath);
    if (!load_file.open(QFile::ReadOnly)) {
        qWarning("Couldn't open json file.");
        return UTMPoints;
    }

    QByteArray data = load_file.readAll();
    QJsonParseError err;
    QJsonDocument json_doc = QJsonDocument::fromJson(data, &err);
    if (json_doc.isNull()) {
        qDebug() << err.errorString();
        return UTMPoints;
    }
    QJsonObject echo_info = json_doc.object();
    double UTMX, UTMY, UTMZ;
    if (echo_info.contains("features") && echo_info["features"].isArray())
    {
        QJsonArray info_array = echo_info["features"].toArray();
        for (int i=0; i<info_array.size(); i++) {
            QJsonObject item_object = info_array[i].toObject();
            if (item_object.contains("geometry") && item_object["geometry"].isObject()) {
                QJsonObject tmp = item_object["geometry"].toObject();
                if(tmp.contains("coordinates") && tmp["coordinates"].isArray())
                {
                    QJsonArray arr = tmp["coordinates"].toArray();
                    UTMX = arr.at(1).toDouble(); //X
                    UTMY = arr.at(0).toDouble(); //Y
                    UTMZ = arr.at(2).toDouble(); //Z
                    UTMPoints.push_back(UTMPoint{UTMX, UTMY, UTMZ});
                }

            }
        }
    }
    else
    {
        assertm(UTMPoints.size() != 0, "Read a unknown format Json file");
    }
    return UTMPoints;
}

}
#endif // JSONHELPER_H
