#ifndef ITSFILEINFO_H
#define ITSFILEINFO_H

#include <QString>
#include <QJsonObject>

using namespace std;

class itsFileInfo
{
public:
    itsFileInfo();
    ~itsFileInfo();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString getMediaType(){return mType;}
    QString getFileName(){return mFileName;}

    void setFileName(const QString &fileName){ mFileName = fileName;}


private:
    QString mFileName;
    QString mType;
    QString mVersion;
    QString mCreateTime;
    QString mLastUpdate;
};

#endif // ITSFILEINFO_H
