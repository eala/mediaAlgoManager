#include "itsgolden.h"

itsGolden::itsGolden()
{
    mCategories.clear();
    mFrameIndices.clear();
    mFrames.clear();
    mFrameScores.clear();
}

itsGolden::itsGolden(const string &fileName)
{
    mCategories.clear();
    mFrameIndices.clear();
    mFrames.clear();
    mFrameScores.clear();

    std::ifstream loadFile;
    loadFile.open(fileName, std::fstream::in);
    bool parseResult = mJson.parse(loadFile);
    if(parseResult){
       cout << "parse json success" << endl;
    }else{
        cout << "parse json failed" << endl;
    }

    read(mJson);
    loadFile.close();
}

itsGolden::~itsGolden()
{

}

int itsGolden::getFrame(int frameIdx, itsFrame &frame){
    int index=-1;
    vector<int>::iterator it;
    it = find (mFrameIndices.begin(), mFrameIndices.end(), frameIdx);
    if(it != mFrameIndices.end()){
        index = it-mFrameIndices.begin();
        frame = mFrames[index];
    }
    return index;
}

// store only existing objects
void itsGolden::read(jsonxx::Object &json){
    mFileInfo.read(json.get<jsonxx::Object>("file"));
    jsonxx::Array framesArray = json.get<jsonxx::Array>("frames");

    for(size_t i=0; i< framesArray.size(); ++i){
        itsFrame itsframe;
        itsframe.read(framesArray.get<jsonxx::Object>(i));
        mFrameIndices.push_back(itsframe.getIndex());
        mFrames.push_back(itsframe);
    }

    jsonxx::Array categoryArray = json.get<jsonxx::Array>("categories");

#if 1
    mCategories.push_back("ALL");
    mCategories.push_back("CAR");
    mCategories.push_back("LANE");
    mCategories.push_back("PEDESTRIAN");
#else
    // fixme, not working
    for(int i=0; i< categoryArray.size(); ++i){
        //string refStr(categoryArray[i].toString().toUtf8().constData());
        string str(categoryArray.get<string>(i));
        refStr += "" + refStr;
        mCategories.push_back(refStr.c_str());
    }
#endif
}

void itsGolden::write(jsonxx::Object &json){
    jsonxx::Object jsonFileObj = json.get<jsonxx::Object>("file");
    mFileInfo.write(jsonFileObj);

    jsonxx::Array framesArray = json.get<jsonxx::Array>("frames");
    for(size_t i=0; i< framesArray.size(); ++i){
        jsonxx::Object jsonFrameObject = framesArray.get<jsonxx::Object>(i);
        mFrames[i].write(jsonFrameObject);
    }
}

itsGolden& itsGolden::operator=(const itsGolden &golden){
    mJson = golden.mJson;
    mFileInfo = golden.mFileInfo;
    mFrameIndices = golden.mFrameIndices;
    mFrames = golden.mFrames;
    return *this;
}

double itsGolden::evaluate(itsGolden &otherItsGolden, const CATEGORIES &categ){
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
            double frameScore = thisFrame.evaluate(otherFrame, categ);
            mFrameScores[mFrameIndices[i]] = frameScore;
            score += frameScore;
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
