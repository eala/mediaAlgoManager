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
    //itsGolden();
    itsGolden(const QString &fileName);
    ~itsGolden();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    // getter & setter
    QList<int> &getFrameIndices() {return mFrameIndices;}
    QList<itsFrame> &getFrames() {return mFrames;}

    // File related
    QString getMediaType(){return mFileInfo.getMediaType();}
    QString getFileName(){return mFileInfo.getFileName();}
    void setFileName(const QString &fileName){ mFileInfo.setFileName(fileName);}

    // support CRUD operations
    // Update
    void updateFrame(const itsObject &itsObj);

    // comparison
    double evaluate(itsGolden &otherItsGolden, const CATEGORIES &categ);

private:
    itsFileInfo mFileInfo;
    QList<int> mFrameIndices;
    QList<itsFrame> mFrames;

};

#endif // ITSGOLDEN_H
