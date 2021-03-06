#ifndef ITS_FRAME_H
#define ITS_FRAME_H

#include<opencv2/core/core.hpp>
#include "itsobject.h"

class itsFrame
{
public:
    itsFrame();
    ~itsFrame();

    void read(const jsonxx::Object &json);
    void write(jsonxx::Object &json);

    int getIndex(){return mIndex;}
    vector<itsObject> &getCandidates() {return mCandidates;}
    itsObject &getObject( Rect &rect);

    double evaluate(itsFrame &otherFrame, const CATEGORIES &categ);

    itsFrame& operator=(const itsFrame &frame);

    /*
    void addObject( itsObject &object);
    void updateObject(itsObject &object);
    void deleteObjects( CATEGORIES &categ);
    void clearObjects(){mCandidates.clear();}
    */

private:
    int mIndex;
    vector<itsObject> mCandidates;
};

#endif // FRAME_H
