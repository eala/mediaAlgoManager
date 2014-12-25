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
    void write(QJsonObject &json) ;

    string getMediaType(){ return mType;}
    string getFileName() const{return mFileName;}

    void setFileName(const string &fileName){ mFileName = fileName;}

private:
    string mFileName;
    string mType;
    string mVersion;
    string mCreateTime;
    string mLastUpdate;
};

#endif // ITSFILEINFO_H
