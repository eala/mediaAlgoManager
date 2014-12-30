#ifndef ANALYZEGOLDENWINDOW_H
#define ANALYZEGOLDENWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMenuBar>
#include <QToolBar>
#include <QKeyEvent>

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
    analyzeGoldenWindow(QWidget *parent=0);
    ~analyzeGoldenWindow();

private:
    void createAction();
    void createMenu();
    void createStatusBar();
    void createToolBar();

    void loadItsObjects();
    void drawFrame(const int frameIdx);
    void updateScoreLabels();
    void stepFrame(int step=1);

protected:
    // event handler
    void keyPressEvent(QKeyEvent* event);

private slots:
    void newFile();
    void loadTestFile();
    void loadGoldenFile();
    void evaluate();

private:
    Ui::analyzeGoldenWindow *ui;
    QStandardItemModel *mTestModel;
    QStandardItemModel *mGoldenModel;
    CItsApp *mApp;

    QAction *mNewFileAction;
    QAction *mLoadTestFileAction;
    QAction *mLoadGoldenFileAction;
    QAction *mEvaluateAction;

    QMenuBar *mMenuBar;
    QMenu *mFileMenu;

    QToolBar *mFileToolBar;
    QStatusBar *mStatusBar;
};

#endif // ANALYZEGOLDENWINDOW_H
