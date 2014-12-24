#include "itsfileinfo.h"

itsFileInfo::itsFileInfo()
{

}

itsFileInfo::~itsFileInfo()
{

}

void itsFileInfo::read(const QJsonObject &json){
    mFileName = json["name"].toString();
    mType = json["type"].toString();
    mVersion = json["version"].toString();
    mCreateTime = json["createTime"].toString();
    mLastUpdate = json["lastUpdate"].toString();

    /*
    qDebug() << mFileName;
    qDebug() << mType;
    qDebug() << mVersion;
    qDebug() << mCreateTime;
    qDebug() << mLastUpdate;
    */
}

void itsFileInfo::write(QJsonObject &json) const{
    json["name"] = mFileName;
    json["type"] = mVersion;
    json["version"] = mFileName;
    json["createTime"] = mCreateTime;
    json["lastUpdate"] = mLastUpdate;
}

