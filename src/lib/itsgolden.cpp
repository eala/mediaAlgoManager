#include "itsgolden.h"

itsGolden::itsGolden(const QString &fileName)
{
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        //return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject goldenJson = loadDoc.object();
    read(goldenJson);
    loadFile.close();
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
        mFrameIndices.push_back(itsframe.getIndex());
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

double itsGolden::evaluate(itsGolden &otherItsGolden, const CATEGORIES &categ){
    // fixme later, fill two golden files with union frame counts
    assert(mFrames.size() == mFrameIndices.size());
    QList<itsFrame> otherFrames = otherItsGolden.getFrames();
    QList<int> otherFrameIndices = otherItsGolden.getFrameIndices();

    double score(0.0);
    for(int i=0; i< mFrames.size(); ++i){
        if(otherFrameIndices.contains(mFrameIndices[i])){
            itsFrame thisFrame = mFrames[i];
            itsFrame otherFrame = otherFrames[otherFrameIndices.indexOf(mFrameIndices[i])];
            score += thisFrame.evaluate(otherFrame, categ);
        }else{
            // fixme later
        }
    }
    return score;
}
