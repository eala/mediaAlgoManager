#ifndef ITS_FRAME_H
#define ITS_FRAME_H

#include<opencv2/core/core.hpp>
#include<QtCore>
#include "itsobject.h"

class itsFrame
{
public:
    itsFrame();
    ~itsFrame();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    int getIndex(){return mIndex;}
    QList<itsObject> &getCandidates() {return mCandidates;}

    double evaluate(itsFrame &otherFrame, const CATEGORIES &categ);

private:
    int mIndex;
    QList<itsObject> mCandidates;
};

#endif // FRAME_H
