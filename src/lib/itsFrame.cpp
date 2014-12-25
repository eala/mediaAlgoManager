#include "itsFrame.h"

itsFrame::itsFrame(): mIndex(0)
{
    mCandidates.clear();
}

itsFrame::~itsFrame()
{

}

void itsFrame::read(const QJsonObject &json){
    mIndex = json["frameIdx"].toInt();
    mCandidates.clear();
    QJsonArray jsonCandidates = json["objects"].toArray();
    for(int i=0; i< jsonCandidates.size(); ++i){
        QJsonObject jsonCandidate = jsonCandidates[i].toObject();
        itsObject itsObj;
        itsObj.read(jsonCandidate); // fixme later, itsObject should be able to init from read
        mCandidates.push_back(itsObj);
    }
}

void itsFrame::write(QJsonObject &json) {
    json["frameIdx"] = mIndex;
    QJsonArray jsonCandidates = json["objects"].toArray();
    for(size_t i=0; i<mCandidates.size(); ++i){
        QJsonObject jsonCandidate;
        itsObject itsObj = mCandidates[i];
        itsObj.write(jsonCandidate);
        jsonCandidates.append(jsonCandidate);
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
    qDebug() << "frame " << mIndex << " score: " << score << endl;
    return score;
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
