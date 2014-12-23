#include "itsobject.h"

itsObject::itsObject()
{

}

itsObject::~itsObject()
{

}

itsObject::itsObject(const Rect &rect, CATEGORIES category){
    setObject(rect);
    setCategory(category);
}

Rect itsObject::getObject() const{
    return mObject;
}

CATEGORIES itsObject::getCategory() const{
    return mCategory;
}

void itsObject::setObject(const Rect &rect){
    //memcpy(mObject, rect, sizeof(mObject));
    mObject = rect;
}

void itsObject::setCategory(const CATEGORIES &category){
    mCategory = category;
}

void itsObject::read(const QJsonObject &json){
    QJsonArray rectArray = json["rect"].toArray();
    //QJsonArray rectArray = json["rect"];
    assert(rectArray.size() == 4);
    mObject.x = rectArray[0].toInt();
    mObject.y = rectArray[1].toInt();
    mObject.width = rectArray[2].toInt();
    mObject.height = rectArray[3].toInt();
    mCategory = CATEGORIES(json["category"].toInt());
}

void itsObject::write(QJsonObject &json) const{
    QJsonArray rectArray;
    rectArray.append(mObject.x);
    rectArray.append(mObject.y);
    rectArray.append(mObject.width);
    rectArray.append(mObject.height);
    json["rect"] = rectArray;
    json["category"] = mCategory;
}
