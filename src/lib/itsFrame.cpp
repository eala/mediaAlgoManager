#include "itsFrame.h"

itsFrame::itsFrame(): mIndex(0)
{
    mCandidates.clear();
}

itsFrame::~itsFrame()
{

}

void itsFrame::read(const jsonxx::Object &json){
    mIndex = json.get<jsonxx::Number>("frameIdx");
    mCandidates.clear();
    jsonxx::Array jsonCandidates = json.get<jsonxx::Array>("objects");

    for(size_t i=0; i< jsonCandidates.size(); ++i){
        jsonxx::Object jsonCandidate = jsonCandidates.get<jsonxx::Object>(i);
        itsObject itsObj;
        itsObj.read(jsonCandidate); // fixme later, itsObject should be able to init from read
        mCandidates.push_back(itsObj);
    }
}

void itsFrame::write(jsonxx::Object &json) {
    json << "frameIdx" << mIndex;
    jsonxx::Array jsonCandidates = json.get<jsonxx::Array>("objects");

    for(size_t i=0; i<mCandidates.size(); ++i){
        jsonxx::Object jsonCandidate;
        itsObject itsObj = mCandidates[i];
        itsObj.write(jsonCandidate);
        jsonCandidates << jsonCandidate;
    }
}

double itsFrame::evaluate(itsFrame &otherFrame, const CATEGORIES &categ){
    vector<itsObject> otherCandidates = otherFrame.getCandidates();

    double score(0.0);
    for(size_t i=0; i< mCandidates.size(); ++i){
        if(categ == mCandidates[i].getCategory()){
            for(size_t j=0; j<otherCandidates.size(); ++j){
                if(categ == otherCandidates[j].getCategory()){
                    score += mCandidates[i].evaluate(otherCandidates[j]);
                }
            }
        }
    }
    //std::cout << "frame " << mIndex << " score: " << score;
    return score;
}

itsFrame& itsFrame::operator=(const itsFrame &frame){
    mIndex = frame.mIndex;
    mCandidates = frame.mCandidates;
    return *this;
}

#if 0
void itsFrame::deleteObjects( CATEGORIES &categ){
    // not working
    for(int i = mCandidates.size()-1; i>0 ; i--){
        if(categ == mCandidates[i].getCategory()){
            mCandidates.removeAt(i);
            qDebug() << "candidate " << i << ", category: " << mCandidates[i].getCategory() << endl;
        }
    }
}
#endif
