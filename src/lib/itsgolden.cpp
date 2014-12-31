#include "itsgolden.h"

itsGolden::itsGolden()
{
    mCategories.clear();
    mFrames.clear();
    mFrameScores.clear();
    mAllFrames.clear();
}

itsGolden::itsGolden(const string &fileName)
{
    mCategories.clear();
    mFrames.clear();
    mFrameScores.clear();
    mAllFrames.clear();

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
    map<int, itsFrame >::iterator it;
    it = mFrames.find(frameIdx);
    if(it != mFrames.end()){
        frame = mFrames[frameIdx];
        index = 0;  // use index 0 to indicate it finds
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
        mFrames[itsframe.getIndex()] = itsframe;
        mAllFrames.push_back(itsframe);
    }

    jsonxx::Array categoryArray = json.get<jsonxx::Array>("categories");

    for(int i=0; i< categoryArray.size(); ++i){
        string str(categoryArray.get<jsonxx::String>(i));
        mCategories.push_back(str.c_str());
    }
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
    mCategories = golden.mCategories;
    mAllFrames = golden.mAllFrames;
    mFrames = golden.mFrames;
    mFrameScores = golden.mFrameScores;

    return *this;
}

double itsGolden::evaluate(itsGolden &otherItsGolden, const CATEGORIES &categ){
    // fixme later, fill two golden files with union frame counts
    map<int, itsFrame> otherFrames = otherItsGolden.getFrames();

    double score(0.0);

    for(map<int, itsFrame>::iterator it=mFrames.begin(); it != mFrames.end(); ++it){
        int frameIndex = it->first;
        if(otherFrames.end() != mFrames.find(frameIndex)){
            itsFrame testFrame = mFrames[frameIndex];
            itsFrame goldenFrame = otherFrames[frameIndex];
            double frameScore = testFrame.evaluate(goldenFrame,categ);
            mFrameScores[frameIndex] = frameScore;
            score += frameScore;
        }
    }

    return score;
}

void itsGolden::addObject(const int frameIdx, const Rect &objRect, CATEGORIES &categ){

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
