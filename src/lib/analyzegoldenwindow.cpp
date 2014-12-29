#include "analyzegoldenwindow.h"
#include "ui_analyzegoldenwindow.h"

#include "MatToQImage.h"
#include "itsobject.h"

enum TableColumns{
    FRAME_INDEX=0,
    CANDIDATE_X,
    CANDIDATE_Y,
    CANDIDATE_WIDTH,
    CANDIDATE_HEIGHT,
    CANDIDATE_CATEGORY,
    TABLE_COLUMNS_COUNT
};

const char columnName[TABLE_COLUMNS_COUNT][128] = {
    "Frame Index",
    "x",
    "y",
    "width",
    "height",
    "category"
};

analyzeGoldenWindow::analyzeGoldenWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::analyzeGoldenWindow),
    mApp(NULL)
{
    // release memory when window close
    setAttribute(Qt::WA_DeleteOnClose);

    // fixme later, add some readSettings() to restore as last time
    if(NULL == mApp){
        itsGolden testFile(QStringLiteral("config/V2014-07-15-15-21-20.json"));
        itsGolden goldenFile(QStringLiteral("config/V2014-07-15-15-21-20-2.json"));
        mApp = new CItsApp(testFile, goldenFile);
    }

    setActionMenuBar();

    ui->setupUi(this);



    // notice!! you can't see ui->frameLabel before ui->setupUi(this);
    drawFrame(1);

    // init test files
    if(mApp && CItsApp::READY == mApp->getState()){
        loadItsObjects();
    }
}

void analyzeGoldenWindow::setActionMenuBar(){
    mMenuBar = new QMenuBar(this);
    mMenuBar->setNativeMenuBar(true);

    mFileMenu = mMenuBar->addMenu(tr("&File"));
    mNewFileAction = new QAction(tr("&New ..."), this);
    mFileMenu->addAction(mNewFileAction);
    connect(mNewFileAction, SIGNAL(triggered()), this, SLOT(newFile()));
}

analyzeGoldenWindow::~analyzeGoldenWindow()
{
    // fixme later, add like writeSettings() to store layout, e.g. use QSettings
    if(mTestModel){
        delete mTestModel;
        mTestModel = NULL;
    }
    delete ui;
}

void analyzeGoldenWindow::drawFrame(const int frameIdx)
{
    if(mApp){
        mApp->moveToFrame(frameIdx);
        Mat drawFrame = mApp->getProcFrame();

        // draw test & golden rectangles
        itsFrame testDatas;
        int testIdx = mApp->getTestIts().getFrame(frameIdx, testDatas);
        if(-1!=testIdx){
            mApp->displayItsFrame(drawFrame, &testDatas, Scalar(255, 0, 0));
        }
        itsFrame goldenDatas;
        int goldenIdx = mApp->getGoldenIts().getFrame(frameIdx, goldenDatas);
        if(-1!=goldenIdx){
            mApp->displayItsFrame(drawFrame, &goldenDatas, Scalar(0, 0, 255));
        }
        if(drawFrame.rows > 0) ui->frameLabel->setPixmap(QPixmap::fromImage(MatToQImage(drawFrame)));
    }
}

void analyzeGoldenWindow::loadItsObjects()
{
    const int rowCount = 10;
    mTestModel = new QStandardItemModel(rowCount, CANDIDATE_CATEGORY, this);

    // use const string to store TableColumns, ref: fcws
    mTestModel->setHorizontalHeaderItem(FRAME_INDEX, new QStandardItem(QString(columnName[FRAME_INDEX])));
    mTestModel->setHorizontalHeaderItem(CANDIDATE_X, new QStandardItem(QString(columnName[CANDIDATE_X])));
    mTestModel->setHorizontalHeaderItem(CANDIDATE_Y, new QStandardItem(QString(columnName[CANDIDATE_Y])));
    mTestModel->setHorizontalHeaderItem(CANDIDATE_WIDTH, new QStandardItem(QString(columnName[CANDIDATE_WIDTH])));
    mTestModel->setHorizontalHeaderItem(CANDIDATE_HEIGHT, new QStandardItem(QString(columnName[CANDIDATE_HEIGHT])));
    mTestModel->setHorizontalHeaderItem(CANDIDATE_CATEGORY, new QStandardItem(QString(columnName[CANDIDATE_CATEGORY])));

    vector<itsFrame> testFrames = mApp->getTestFrames();
    static int rowIdx = 0;

    for(size_t i=0; i< testFrames.size(); ++i){
        vector<itsObject> testFrameObjects = testFrames[i].getCandidates();
        int frameIdx = testFrames[i].getIndex();
        for(size_t j=0; j<testFrameObjects.size(); ++j){
            Rect objRect = testFrameObjects[j].getObject();
            QString categoryName = QString::fromStdString(testFrameObjects[j].getCategoryName());
            // fixme later, strange! when to release resources
            QStandardItem *tableItemItsFrameIdx = new QStandardItem(QString::number(frameIdx));
            mTestModel->setItem(rowIdx, FRAME_INDEX, tableItemItsFrameIdx);
            QStandardItem *tableItemItsObjX = new QStandardItem(QString::number(objRect.x));
            mTestModel->setItem(rowIdx, CANDIDATE_X,tableItemItsObjX);
            QStandardItem *tableItemItsObjY = new QStandardItem(QString::number(objRect.y));
            mTestModel->setItem(rowIdx, CANDIDATE_Y,tableItemItsObjY);
            QStandardItem *tableItemItsObjW = new QStandardItem(QString::number(objRect.width));
            mTestModel->setItem(rowIdx, CANDIDATE_WIDTH,tableItemItsObjW);
            QStandardItem *tableItemItsObjH = new QStandardItem(QString::number(objRect.height));
            mTestModel->setItem(rowIdx, CANDIDATE_HEIGHT,tableItemItsObjH);
            QStandardItem *tableItemItsCategory = new QStandardItem(categoryName);
            mTestModel->setItem(rowIdx, CANDIDATE_CATEGORY,tableItemItsCategory);
            rowIdx++;
        }
    }

    ui->candidateTableView->setModel(mTestModel);

}

void analyzeGoldenWindow::newFile()
{
    analyzeGoldenWindow *newAnalyzeGoldenWin = new analyzeGoldenWindow;
    newAnalyzeGoldenWin->show();
}



/*
void analyzeGoldenWindow::setFrame(const cv::Mat &img){
    mFrame = img;
}
*/


