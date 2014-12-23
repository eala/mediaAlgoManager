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
