#include "itsgolden.h"

itsGolden::itsGolden(const QString &fileName)
{
    mFrames.clear();
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        //return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    mJson = loadDoc.object();
    read(mJson);
    loadFile.close();
}

itsGolden::~itsGolden()
{

}

itsFrame &itsGolden::getFrame(int frameIdx){
    vector<int>::iterator it;
    it = find (mFrameIndices.begin(), mFrameIndices.end(), frameIdx);
    return mFrames[it-mFrameIndices.begin()];
}

// store only existing objects
void itsGolden::read( QJsonObject &json){
    mFileInfo.read(json["file"].toObject());
    QJsonArray framesArray = json["frames"].toArray();

    for(int i=0; i< framesArray.size(); ++i){
        itsFrame itsframe;
        itsframe.read(framesArray[i].toObject());
        mFrameIndices.push_back(itsframe.getIndex());
        mFrames.push_back(itsframe);
    }
}

void itsGolden::write(QJsonObject &json) {
    QJsonObject jsonFileObj = json["file"].toObject();
    mFileInfo.write(jsonFileObj);

    QJsonArray framesArray = json["frames"].toArray();
    for(int i=0; i< framesArray.size(); ++i){
        QJsonObject jsonFrameObject = framesArray[i].toObject();
        mFrames[i].write(jsonFrameObject);
    }
}

double itsGolden::evaluate(itsGolden &otherItsGolden,  CATEGORIES &categ){
    // fixme later, fill two golden files with union frame counts
    assert(mFrames.size() == mFrameIndices.size());
    vector<itsFrame> otherFrames = otherItsGolden.getFrames();
    vector<int> otherFrameIndices = otherItsGolden.getFrameIndices();

    double score(0.0);
    for(size_t i=0; i< mFrames.size(); ++i){
        std::vector<int>::iterator it;
        it = find (otherFrameIndices.begin(), otherFrameIndices.end(), mFrameIndices[i]);
        if(it != otherFrameIndices.end()){
            itsFrame thisFrame = mFrames[i];
            itsFrame otherFrame = otherFrames[it - otherFrameIndices.begin()];
            score += thisFrame.evaluate(otherFrame, categ);
        }
    }
    return score;
}

/*
void itsGolden::updateFrame(int frameIdx,  Rect &rect,  CATEGORIES &categ){
    // pass to lower
}

void itsGolden::deleteObjects( CATEGORIES &categ){
#if 0
    for(int i=mFrames.size()-1; i>0; i--){
        mFrames[i].deleteObjects(categ);
    }
    write(mJson);
#else
    QJsonValueRef fileInfoRef = mJson.find("file").value();
    QJsonObject fileInfoObj = fileInfoRef.toObject();
    QJsonValueRef versionRef = fileInfoObj.find("version").value();
    versionRef = 2;

    qDebug()<< "version: " << (mJson["file"].toObject())["version"].toInt() << endl;

    QJsonDocument myJsonDoc;
    myJsonDoc.setObject(mJson);

    QFile updatedJsonFile("/Users/hank/testJson.json");
    //QFile updatedJsonFile(mFileInfo.getFileName());
    updatedJsonFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    updatedJsonFile.write(myJsonDoc.toJson());
    updatedJsonFile.close();
#endif
}
*/
