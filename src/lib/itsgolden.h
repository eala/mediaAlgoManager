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
    ~itsGolden();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    // File related
    QString getMediaType(){return mFileInfo.getMediaType();}
    QString getFileName(){return mFileInfo.getFileName();}
    void setFileName(const QString &fileName){ mFileInfo.setFileName(fileName);}

    // support CRUD operations
    // Update
    void updateFrame(const itsObject &itsObj);

private:
    itsFileInfo mFileInfo;
    QList<itsFrame> mFrames;
};

#endif // ITSGOLDEN_H
