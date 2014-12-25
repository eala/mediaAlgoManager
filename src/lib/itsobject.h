#ifndef ITSOBJECT_H
#define ITSOBJECT_H

#include <opencv2/core/core.hpp>
#include <QJsonObject>
#include <QJsonArray>

using namespace cv;

// allowed categories

enum CATEGORIES{
    CAR=1,
    PEDESTRIAN,
    LANE,
    UNKNOWN
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
