#ifndef ITSFILEINFO_H
#define ITSFILEINFO_H

#include "jsonxx.h"

using namespace std;

class itsFileInfo
{
public:
    itsFileInfo();
    ~itsFileInfo();

    void read(const jsonxx::Object &json);
    void write(jsonxx::Object &json);

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
