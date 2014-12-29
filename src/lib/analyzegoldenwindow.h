#ifndef ANALYZEGOLDENWINDOW_H
#define ANALYZEGOLDENWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMenuBar>

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
    void loadItsObjects();
    void drawFrame(const int frameIdx);
    void setActionMenuBar();

private slots:
    void newFile();

private:
    Ui::analyzeGoldenWindow *ui;
    QStandardItemModel *mTestModel;
    CItsApp *mApp;

    QMenuBar *mMenuBar;
    QMenu *mFileMenu;
    QAction *mNewFileAction;
};

#endif // ANALYZEGOLDENWINDOW_H
