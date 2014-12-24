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
    itsObject(const Rect &rect, CATEGORIES category);
    ~itsObject();

    CATEGORIES parseCategory(const QString &str);
    Rect &getObject();
    CATEGORIES getCategory();

    void setObject(const Rect &rect);
    void setCategory(const CATEGORIES &category);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    double evaluate(itsObject &otherObject);
    // double evaluate(const itsObject &otherObject, CATEGORIES categ);

private:
    Rect mObject;
    CATEGORIES mCategory;
};

#endif // ITSOBJECT_H
