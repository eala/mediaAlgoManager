#ifndef ITSOBJECT_H
#define ITSOBJECT_H

#include <opencv2/core/core.hpp>
#include <QJsonObject>
#include <QJsonArray>

using namespace cv;

// allowed categories

enum CATEGORIES{
    CAR=0,
    PEDESTRIAN,
    LANE,
    UNKNOWN,
    TOTAL_CATEGORIES
};

const char CATEGORY_NAMES[TOTAL_CATEGORIES][128] = {
    "car",
    "pedestrian",
    "lane",
    "unknown"
};

class itsObject
{
public:
    itsObject();
    itsObject(Rect &rect, CATEGORIES category);
    ~itsObject();

    CATEGORIES parseCategory(const QString &str);
    Rect &getObject();
    CATEGORIES getCategory();
    std::string getCategoryName();

    void setObject(Rect &rect);
    void setCategory(CATEGORIES &category);

    void read(QJsonObject &json);
    void write(QJsonObject &json);

    void update(Rect &rect, CATEGORIES &category);

    double evaluate(itsObject &otherObject);
    // double evaluate( itsObject &otherObject, CATEGORIES categ);

private:
    Rect mObject;
    CATEGORIES mCategory;
};

#endif // ITSOBJECT_H
