#include "itsobject.h"

CATEGORIES itsObject::parseCategory(const string &str){
    CATEGORIES category;

    if(0==str.compare("CAR")){
        category = CAR;
    }else if(0==str.compare("PEDESTRIAN")){
        category = PEDESTRIAN;
    }else if(0==str.compare("LANE")){
        category = LANE;
    }else{
        category = UNKNOWN;
    }

    return category;
}

itsObject::itsObject(): mCategory(ALL)
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

std::string itsObject::getCategoryName(){
    return CATEGORY_NAMES[mCategory];
}

void itsObject::setObject( Rect &rect){
    mObject = rect;
}

void itsObject::setCategory( CATEGORIES &category){
    mCategory = category;
}

void itsObject::read(jsonxx::Object &json)
{
    jsonxx::Array rectArray = json.get<jsonxx::Array>("rect");
    assert(rectArray.size() == 4);

    mObject.x = rectArray.get<jsonxx::Number>(0);
    mObject.y = rectArray.get<jsonxx::Number>(1);
    mObject.width = rectArray.get<jsonxx::Number>(2);
    mObject.height = rectArray.get<jsonxx::Number>(3);
    mCategory = parseCategory(json.get<jsonxx::String>("category"));
}

void itsObject::write(jsonxx::Object &json){
    jsonxx::Array rectArray;
    rectArray << mObject.x;
    rectArray << mObject.y;
    rectArray << mObject.width;
    rectArray << mObject.height;
    json << "rect" << rectArray;
    json << "category" << mCategory;
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
