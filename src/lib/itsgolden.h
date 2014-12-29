#ifndef ITSGOLDEN_H
#define ITSGOLDEN_H

#include <QJsonArray>
#include <QJsonObject>

#include "itsfileinfo.h"
#include "itsFrame.h"
#include "itsobject.h"

// all the APIs
class itsGolden
{
public:
    itsGolden();
    itsGolden( const QString &fileName);
    ~itsGolden();

    void read( QJsonObject &json);
    void write(QJsonObject &json) ;

    // getter & setter
    itsGolden& operator=(const itsGolden &golden);

    vector<int> &getFrameIndices() {return mFrameIndices;}
    vector<itsFrame> &getFrames() {return mFrames;}
    itsFrame &getFrame(int frameIdx);

    // File related
    string getMediaType(){return mFileInfo.getMediaType();}
    string getFileName() const{return mFileInfo.getFileName();}
    void setFileName( const string &fileName){ mFileInfo.setFileName(fileName);}

    // support CRUD operations
    // Update
    void updateFrame(int frameIdx,  Rect &rect,  CATEGORIES &categ);

    // Delete
    void deleteObjects( CATEGORIES &categ);

    // comparison
    double evaluate(itsGolden &otherItsGolden, const CATEGORIES &categ);

private:
    QJsonObject mJson;
    itsFileInfo mFileInfo;
    vector<int> mFrameIndices;
    vector<itsFrame> mFrames;
};

#endif // ITSGOLDEN_H
