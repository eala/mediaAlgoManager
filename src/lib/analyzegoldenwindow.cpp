#include "analyzegoldenwindow.h"
#include "ui_analyzegoldenwindow.h"

#include "MatToQImage.h"

analyzeGoldenWindow::analyzeGoldenWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::analyzeGoldenWindow),
    mApp(NULL)
{
    if(NULL == mApp){
        itsGolden testFile(QStringLiteral("/Users/hank/allProjects/install/its_env/V2014-07-15-15-21-20.json"));
        itsGolden goldenFile(QStringLiteral("/Users/hank/allProjects/install/its_env/V2014-07-15-15-21-20-2.json"));
        mApp = new CItsApp(testFile, goldenFile);
    }


    ui->setupUi(this);

    // notice!! you can't see ui->frameLabel before ui->setupUi(this);
    const int index = 2000;
    mApp->moveToFrame(index);
    Mat drawFrame = mApp->getProcFrame();
    //imshow("test", mApp->getProcFrame());

    if(drawFrame.rows > 0){
        ui->frameLabel->setPixmap(QPixmap::fromImage(MatToQImage(drawFrame)));
    }



}

analyzeGoldenWindow::~analyzeGoldenWindow()
{
    delete ui;
}

/*
void analyzeGoldenWindow::setFrame(const cv::Mat &img){
    mFrame = img;
}
*/


