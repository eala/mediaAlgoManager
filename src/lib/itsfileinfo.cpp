#include "itsfileinfo.h"

itsFileInfo::itsFileInfo()
    : mFileName(""), mType(""), mVersion(""), mCreateTime(""), mLastUpdate("")
{
}

itsFileInfo::~itsFileInfo()
{

}

void itsFileInfo::read(const jsonxx::Object &json){
    mFileName = json.get<jsonxx::String>("name");
    mType = json.get<jsonxx::String>("type");
    mVersion = json.get<jsonxx::String>("version");
    mCreateTime = json.get<jsonxx::String>("createTime");
    mLastUpdate = json.get<jsonxx::String>("lastUpdate");
}

void itsFileInfo::write(jsonxx::Object &json) {
    json << "name" << mFileName;
    json << "type" << mType;
    json << "version" << mVersion;
    json << "createTime" << mCreateTime;
    json << "lastUpdate" << mLastUpdate;
}
