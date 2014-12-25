#include "itsobject.h"

CATEGORIES itsObject::parseCategory(const QString &str){
    CATEGORIES category;
    if(0==QString::compare("CAR", str, Qt::CaseInsensitive)){
        category = CAR;
    }else if(0==QString::compare("PEDESTRIAN", str, Qt::CaseInsensitive)){
        category = PEDESTRIAN;
    }else if(0==QString::compare("LANE", str, Qt::CaseInsensitive)){
        category = LANE;
    }else{
        category = UNKNOWN;
    }
    return category;
}

itsObject::itsObject()
{

}

itsObject::~itsObject()
{

}

itsObject::itsObject( Rect &rect, CATEGORIES category){
    setObject(rect);
    setCategory(category);
}

Rect &itsObject::getObject() {
    return mObject;
}

CATEGORIES itsObject::getCategory(){
    return mCategory;
}

void itsObject::setObject( Rect &rect){
    mObject = rect;
}

void itsObject::setCategory( CATEGORIES &category){
    mCategory = category;
}

void itsObject::read( QJsonObject &json){
    QJsonArray rectArray = json["rect"].toArray();
    assert(rectArray.size() == 4);
    mObject.x = rectArray[0].toInt();
    mObject.y = rectArray[1].toInt();
    mObject.width = rectArray[2].toInt();
    mObject.height = rectArray[3].toInt();
    mCategory = parseCategory(json["category"].toString());
}

void itsObject::write(QJsonObject &json) {
    QJsonArray rectArray;
    rectArray.append(mObject.x);
    rectArray.append(mObject.y);
    rectArray.append(mObject.width);
    rectArray.append(mObject.height);
    json["rect"] = rectArray;
    json["category"] = mCategory;
}

// calculate rectangle overlapping -- intersection/union
double itsObject::evaluate( itsObject &otherObject){
    double score(0.0);

     Rect otherObj = otherObject.getObject();
    if(mObject.width <= 0 || mObject.height <=0 ||
            otherObj.width <= 0 || otherObj.height <=0){
        return score;
    }

    int intersectWidth =
            std::min(mObject.x+mObject.width, otherObj.x+otherObj.width) -
            std::max(mObject.x, otherObj.x);
    int intersectHeight =
            std::min(mObject.y+mObject.height, otherObj.y+otherObj.height) -
            std::max(mObject.y, otherObj.y);

    if(intersectWidth > 0 && intersectHeight > 0){
        double intersectArea = intersectWidth * intersectHeight;
        double unionsectArea =
                (mObject.width * mObject.height) + (otherObj.width * otherObj.height)
                - intersectArea;
        score = intersectArea / unionsectArea;
    }
    return score;
}

void itsObject::update( Rect &rect,  CATEGORIES &category){
    setObject(rect);
    setCategory(category);
}
