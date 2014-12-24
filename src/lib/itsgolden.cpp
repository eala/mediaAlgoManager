#include "itsgolden.h"

itsGolden::itsGolden()
{

}

itsGolden::~itsGolden()
{

}

// store only existing objects
void itsGolden::read(const QJsonObject &json){
    mFileInfo.read(json["file"].toObject());
    QJsonArray framesArray = json["frames"].toArray();

    for(int i=0; i< framesArray.size(); ++i){
        itsFrame itsframe;
        itsframe.read(framesArray[i].toObject());
        mFrames.push_back(itsframe);
    }
}

void itsGolden::write(QJsonObject &json) const{
    QJsonObject jsonFileObj = json["file"].toObject();
    mFileInfo.write(jsonFileObj);

    QJsonArray framesArray = json["frames"].toArray();
    for(int i=0; i< framesArray.size(); ++i){
        QJsonObject jsonFrameObject = framesArray[i].toObject();
        mFrames[i].write(jsonFrameObject);
    }
}
