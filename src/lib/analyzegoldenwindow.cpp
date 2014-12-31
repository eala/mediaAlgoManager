#include "analyzegoldenwindow.h"
#include "ui_analyzegoldenwindow.h"

#include "MatToQImage.h"
#include "itsobject.h"

#include <QFileDialog>

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
    "#",
    "x",
    "y",
    "w",
    "h",
    "category"
};

void analyzeGoldenWindow::updateCategoryComboBox(const vector<string> &categories, FileCategory fileType){
    if(TEST_FILE == fileType){
        ui->testCategoryComboBox->clear();
        qDebug() << "categories: " << categories.size();
        for(size_t i=0; i<categories.size(); ++i){
            ui->testCategoryComboBox->addItem(QString::fromStdString(categories[i]));
        }
    }else if(GOLDEN_FILE == fileType){
        ui->goldenCategoryComboBox->clear();
        for(size_t i=0; i<categories.size(); ++i){
            ui->goldenCategoryComboBox->addItem(QString::fromStdString(categories[i]));
        }
    }
}

analyzeGoldenWindow::analyzeGoldenWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::analyzeGoldenWindow),
    mTestModel(NULL), mGoldenModel(NULL), mApp(NULL)
{
    // release memory when window close
    setAttribute(Qt::WA_DeleteOnClose);

    // fixme later, add some readSettings() to restore as last time
    if(NULL == mApp) mApp = new CItsApp();

    ui->setupUi(this);

    createAction();
    createMenu();
    createToolBar();
    createStatusBar();

}

analyzeGoldenWindow::~analyzeGoldenWindow()
{
    // fixme later, add like writeSettings() to store layout, e.g. use QSettings
    if(mTestModel){
        delete mTestModel;
        mTestModel = NULL;
    }
    if(mGoldenModel){
        delete mGoldenModel;
        mGoldenModel = NULL;
    }
    delete ui;
}

void analyzeGoldenWindow::createAction()
{
    mNewFileAction = new QAction(tr("&New evaluate window"), this);
    mNewFileAction->setStatusTip(tr("create new comparison window"));
    connect(mNewFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    mLoadTestFileAction = new QAction(QIcon(":/src/bin/images/open.png"), tr("&Open Test file"), this);
    //openTestAct->setShortcuts(QKeySequence::New);
    mLoadTestFileAction->setStatusTip(tr("Open test file"));
    connect(mLoadTestFileAction, SIGNAL(triggered()), this, SLOT(loadTestFile()));

    mLoadGoldenFileAction = new QAction(QIcon(":/src/bin/images/open.png"), tr("&Open Golden file"), this);
    mLoadGoldenFileAction->setStatusTip(tr("Open golden file"));
    connect(mLoadGoldenFileAction, SIGNAL(triggered()), this, SLOT(loadGoldenFile()));

    mEvaluateAction = new QAction(QIcon(":/src/bin/images/paste.png"), tr("&Evaluate"), this);
    mEvaluateAction->setStatusTip(tr("evaluate test & golden files"));
    connect(mEvaluateAction, SIGNAL(triggered()), this, SLOT(evaluate()));
}

void analyzeGoldenWindow::createMenu(){
    mMenuBar = new QMenuBar(this);
    mMenuBar->setNativeMenuBar(true);
    mFileMenu = mMenuBar->addMenu(tr("&File"));
    mFileMenu->addAction(mNewFileAction);
    mFileMenu->addAction(mLoadTestFileAction);
    mFileMenu->addAction(mLoadGoldenFileAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mEvaluateAction);
}

void analyzeGoldenWindow::createToolBar()
{
    mFileToolBar = addToolBar(tr("File"));
    mFileToolBar->addAction(mLoadTestFileAction);
    mFileToolBar->addAction(mLoadGoldenFileAction);
    mFileToolBar->addAction(mEvaluateAction);
}

void analyzeGoldenWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void analyzeGoldenWindow::updateScoreLabels(){
    //int currentFrameIdx = mApp->getCurrentFrameIndex();
    double currentFrameScore = mApp->getCurrentFrameScore();
    double totalScore = mApp->getTotalScore();
    ui->frameScoreLabel->setText(QString::number(currentFrameScore));
    ui->totalScoreLabel->setText(QString::number(totalScore));
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
        ui->frameIdxLabel->setText("#" + QString::number(mApp->getCurrentFrameIndex()));
    }
}

void analyzeGoldenWindow::drawItsObjects()
{
    int rowCount(0);
    int rowIdx(0);
    if(CItsApp::READY == mApp->getState() || CItsApp::ONLY_TEST_FILE == mApp->getState()){
        vector<itsFrame> testFrames = mApp->getTestFrames();
        rowCount = testFrames.size();

        if(!mTestModel){
            mTestModel = new QStandardItemModel(rowCount, CANDIDATE_CATEGORY, this);    // lazy init
        }else{
            mTestModel->clear();
        }
        // use const string to store TableColumns, ref: fcws
        mTestModel->setHorizontalHeaderItem(FRAME_INDEX, new QStandardItem(QString(columnName[FRAME_INDEX])));
        mTestModel->setHorizontalHeaderItem(CANDIDATE_X, new QStandardItem(QString(columnName[CANDIDATE_X])));
        mTestModel->setHorizontalHeaderItem(CANDIDATE_Y, new QStandardItem(QString(columnName[CANDIDATE_Y])));
        mTestModel->setHorizontalHeaderItem(CANDIDATE_WIDTH, new QStandardItem(QString(columnName[CANDIDATE_WIDTH])));
        mTestModel->setHorizontalHeaderItem(CANDIDATE_HEIGHT, new QStandardItem(QString(columnName[CANDIDATE_HEIGHT])));
        mTestModel->setHorizontalHeaderItem(CANDIDATE_CATEGORY, new QStandardItem(QString(columnName[CANDIDATE_CATEGORY])));

        rowIdx=0;
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
        ui->candidateTableView->resizeColumnsToContents();
        ui->candidateTableView->resizeRowsToContents();
        ui->candidateTableView->verticalHeader()->setVisible(false);
    }

    if(CItsApp::READY == mApp->getState() || CItsApp::ONLY_GOLDEN_FILE == mApp->getState()){
        vector<itsFrame> goldenFrames = mApp->getGoldenFrames();
        rowCount = goldenFrames.size();

        // update golden view, fixme later, refactor the same components
        if(!mGoldenModel){
            mGoldenModel = new QStandardItemModel(rowCount, CANDIDATE_CATEGORY, this);    // lazy init
        }else{
            mGoldenModel->clear();
        }
        // use const string to store TableColumns, ref: fcws
        mGoldenModel->setHorizontalHeaderItem(FRAME_INDEX, new QStandardItem(QString(columnName[FRAME_INDEX])));
        mGoldenModel->setHorizontalHeaderItem(CANDIDATE_X, new QStandardItem(QString(columnName[CANDIDATE_X])));
        mGoldenModel->setHorizontalHeaderItem(CANDIDATE_Y, new QStandardItem(QString(columnName[CANDIDATE_Y])));
        mGoldenModel->setHorizontalHeaderItem(CANDIDATE_WIDTH, new QStandardItem(QString(columnName[CANDIDATE_WIDTH])));
        mGoldenModel->setHorizontalHeaderItem(CANDIDATE_HEIGHT, new QStandardItem(QString(columnName[CANDIDATE_HEIGHT])));
        mGoldenModel->setHorizontalHeaderItem(CANDIDATE_CATEGORY, new QStandardItem(QString(columnName[CANDIDATE_CATEGORY])));

        rowIdx=0;
        for(size_t i=0; i< goldenFrames.size(); ++i){
            vector<itsObject> goldenFrameObjects = goldenFrames[i].getCandidates();
            int frameIdx = goldenFrames[i].getIndex();
            for(size_t j=0; j<goldenFrameObjects.size(); ++j){
                Rect objRect = goldenFrameObjects[j].getObject();
                QString categoryName = QString::fromStdString(goldenFrameObjects[j].getCategoryName());
                // fixme later, strange! when to release resources
                QStandardItem *tableItemItsFrameIdx = new QStandardItem(QString::number(frameIdx));
                mGoldenModel->setItem(rowIdx, FRAME_INDEX, tableItemItsFrameIdx);
                QStandardItem *tableItemItsObjX = new QStandardItem(QString::number(objRect.x));
                mGoldenModel->setItem(rowIdx, CANDIDATE_X,tableItemItsObjX);
                QStandardItem *tableItemItsObjY = new QStandardItem(QString::number(objRect.y));
                mGoldenModel->setItem(rowIdx, CANDIDATE_Y,tableItemItsObjY);
                QStandardItem *tableItemItsObjW = new QStandardItem(QString::number(objRect.width));
                mGoldenModel->setItem(rowIdx, CANDIDATE_WIDTH,tableItemItsObjW);
                QStandardItem *tableItemItsObjH = new QStandardItem(QString::number(objRect.height));
                mGoldenModel->setItem(rowIdx, CANDIDATE_HEIGHT,tableItemItsObjH);
                QStandardItem *tableItemItsCategory = new QStandardItem(categoryName);
                mGoldenModel->setItem(rowIdx, CANDIDATE_CATEGORY,tableItemItsCategory);
                rowIdx++;
            }
        }

        ui->goldenTableView->setModel(mGoldenModel);
        ui->goldenTableView->resizeColumnsToContents();
        ui->goldenTableView->resizeRowsToContents();
        ui->goldenTableView->verticalHeader()->setVisible(false);
    }
}

void analyzeGoldenWindow::newFile()
{
    analyzeGoldenWindow *newAnalyzeGoldenWin = new analyzeGoldenWindow;
    newAnalyzeGoldenWin->show();
}

void analyzeGoldenWindow::stepFrame(int step){
    if(mApp){
        mApp->stepFrame(step);
    }
    drawFrame(mApp->getCurrentFrameIndex());
}

void analyzeGoldenWindow::keyPressEvent(QKeyEvent* event){
    if(Qt::Key_Space == event->key() || Qt::Key_Right == event->key()){
        stepFrame();
    }else if(Qt::Key_Left == event->key()){
        stepFrame(-1);
    }
}

void analyzeGoldenWindow::loadItsFile(FileCategory fileCategory){
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        if(mApp){
            itsGolden golden(fileName);
            if(TEST_FILE == fileCategory){
                mApp->setTestIts(golden);
            }else if(GOLDEN_FILE == fileCategory){
                mApp->setGoldenIts(golden);
            }
            drawFrame();
        }else{
            qDebug("mApp isn't initialized correctly");
        }
    }else{
        qDebug() << "load file failed!";
    }
    drawItsObjects();
}

void analyzeGoldenWindow::loadTestFile(){
    // lazy to learn pass parameter in SLOT
    loadItsFile(TEST_FILE);
    updateCategoryComboBox(mApp->getTestIts().getCategories(), TEST_FILE);
}

void analyzeGoldenWindow::loadGoldenFile(){
    loadItsFile(GOLDEN_FILE);
    updateCategoryComboBox(mApp->getGoldenIts().getCategories(), GOLDEN_FILE);
}

void analyzeGoldenWindow::evaluate(){
    mApp->evaluate(CAR);
    updateScoreLabels();
}
