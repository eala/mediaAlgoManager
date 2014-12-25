#include "itsfileinfo.h"

itsFileInfo::itsFileInfo()
{

}

itsFileInfo::~itsFileInfo()
{

}

void itsFileInfo::read(const QJsonObject &json){
    mFileName = json["name"].toString().toStdString();
    mType = json["type"].toString().toStdString();
    mVersion = json["version"].toString().toStdString();
    mCreateTime = json["createTime"].toString().toStdString();
    mLastUpdate = json["lastUpdate"].toString().toStdString();
}

void itsFileInfo::write(QJsonObject &json) {
    json["name"] =QString::fromStdString(mFileName);
    json["type"] =QString::fromStdString(mType);
    json["version"] =QString::fromStdString(mVersion);
    json["createTime"] =QString::fromStdString(mCreateTime);
    json["lastUpdate"] =QString::fromStdString(mLastUpdate);
}

