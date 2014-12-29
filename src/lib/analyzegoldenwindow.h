#ifndef ANALYZEGOLDENWINDOW_H
#define ANALYZEGOLDENWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "citsapp.h"

namespace Ui {
class analyzeGoldenWindow;
}

class analyzeGoldenWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit analyzeGoldenWindow(QWidget *parent = 0);
    ~analyzeGoldenWindow();

private:
    Ui::analyzeGoldenWindow *ui;
    QStandardItemModel *mTestModel;
    CItsApp *mApp;
};

#endif // ANALYZEGOLDENWINDOW_H
