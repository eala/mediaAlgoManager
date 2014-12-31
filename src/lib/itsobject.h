#ifndef ITSOBJECT_H
#define ITSOBJECT_H

#include <opencv2/core/core.hpp>
#include "jsonxx.h"
#include <fstream>

using namespace cv;

// allowed categories

enum CATEGORIES{
    CAR=0,
    PEDESTRIAN,
    LANE,
    UNKNOWN,
    ALL,
    TOTAL_CATEGORIES
};

const char CATEGORY_NAMES[TOTAL_CATEGORIES][128] = {
    "car",
    "pedestrian",
    "lane",
    "unknown",
    "all"
};

class itsObject
{
public:
    itsObject();
    itsObject(Rect &rect, CATEGORIES category);
    ~itsObject();

    CATEGORIES parseCategory(const string &str);
    Rect &getObject();
    CATEGORIES getCategory();
    std::string getCategoryName();

    void setObject(Rect &rect);
    void setCategory(CATEGORIES &category);

    void read(jsonxx::Object &json);
    void write(jsonxx::Object &json);

    void update(Rect &rect, CATEGORIES &category);

    double evaluate(itsObject &otherObject);
    // double evaluate( itsObject &otherObject, CATEGORIES categ);

private:
    Rect mObject;
    CATEGORIES mCategory;
};

#endif // ITSOBJECT_H
