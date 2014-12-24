#include "itsFrame.h"

itsFrame::itsFrame()
{

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

void itsFrame::write(QJsonObject &json) const{
    json["frameIdx"] = mIndex;
    QJsonArray jsonCandidates = json["objects"].toArray();
    for(int i=0; i<mCandidates.size(); ++i){
        QJsonObject jsonCandidate;
        itsObject itsObj = mCandidates[i];
        itsObj.write(jsonCandidate);
        jsonCandidates.append(jsonCandidate);
    }
}

double itsFrame::evaluate(itsFrame &otherFrame, const CATEGORIES &categ){
    QList<itsObject> otherCandidates = otherFrame.getCandidates();

    double score(0.0);
    for(int i=0; i< mCandidates.size(); ++i){
        if(mCandidates[i].getCategory() == categ){
            for(int j=0; j<otherCandidates.size(); ++j){
                if(otherCandidates[j].getCategory() == categ){
                    score += mCandidates[i].evaluate(otherCandidates[j]);
                }
            }
        }
    }
    qDebug() << "frame score: " << score << endl;
    return score;
}
