#ifndef ITSGOLDEN_H
#define ITSGOLDEN_H

#include "itsfileinfo.h"
#include "itsFrame.h"
#include "itsobject.h"
#include "jsonxx.h"

// all the APIs
class itsGolden
{
public:
    itsGolden();
    itsGolden(const string &fileName);
    ~itsGolden();

    void read(jsonxx::Object &json);
    void write(jsonxx::Object &json);

    // getter & setter
    itsGolden& operator=(const itsGolden &golden);

    map<int, itsFrame> &getFrames() {return mFrames;}
    vector<itsFrame> &getAllFrames() {return mAllFrames;}

    int getFrame(int frameIdx, itsFrame &frame);
    map<int, double> &getFrameScores() { return mFrameScores;}
    vector<string> &getCategories() {return mCategories;}

    // File related
    string getMediaType(){return mFileInfo.getMediaType();}
    string getFileName() const{return mFileInfo.getFileName();}
    void setFileName( const string &fileName){ mFileInfo.setFileName(fileName);}

    // support CRUD operations
    // Update
    void addObject(const int frameIdx, const Rect &objRect, CATEGORIES &categ);
    void updateFrame(int frameIdx,  Rect &rect,  CATEGORIES &categ);

    // Delete
    void deleteObjects( CATEGORIES &categ);

    // comparison
    double evaluate(itsGolden &otherItsGolden, const CATEGORIES &categ);

private:
    jsonxx::Object mJson;
    itsFileInfo mFileInfo;
    vector<string> mCategories;
    vector<itsFrame> mAllFrames;
    map<int, itsFrame> mFrames;
    map<int, double> mFrameScores;  // frameIdx, score
};

#endif // ITSGOLDEN_H
