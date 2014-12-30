#include "citsapp.h"

CItsApp::CItsApp()
    : mProcFrameIdx(0), mTestIts(NULL), mGoldenIts(NULL), mTotalScore(0.0),
      mState(NO_FILES)
{
    mFrameScores.clear();
    sprintf(mProcWinTitle, "demo video");
}

CItsApp::CItsApp(const string &fileName)
    : mProcFrameIdx(0), mTestIts(NULL), mGoldenIts(NULL), mTotalScore(0.0),
      mState(NO_FILES)
{
     mFrameScores.clear();
    sprintf(mProcWinTitle, "demo video");
    mMediaFileName = fileName;
    mCapture.open(mMediaFileName);
}

CItsApp::CItsApp(itsGolden &testIts, itsGolden &goldenIts)
    : mProcFrameIdx(0), mTestIts(NULL), mGoldenIts(NULL), mTotalScore(0.0),
      mState(NO_FILES)
{
     mFrameScores.clear();
    //qDebug() << "testIts media file: " << QString::fromStdString(testIts.getFileName());
    //qDebug() << "goldenIts media file: " << QString::fromStdString(goldenIts.getFileName());

    // check same video
    if(0 != testIts.getFileName().compare(goldenIts.getFileName())){
        qDebug() << "media files for test file & golden file are different\n";
    }else{
        *this = CItsApp(testIts.getFileName());
        mTestIts = testIts;
        mGoldenIts = goldenIts;
        mState = READY;
    }
}

CItsApp::~CItsApp()
{
    if(mCapture.isOpened()){
        mCapture.release();
    }
}

void CItsApp::displayItsFrame(Mat &frame, itsFrame *itsData, cv::Scalar color){
    if(NULL != itsData){
        vector<itsObject> candidates = itsData->getCandidates();
        for(size_t i=0; i<candidates.size(); ++i){
            rectangle(frame, candidates[i].getObject(), color);
        }
    }
}

void CItsApp::setTestIts(const itsGolden &testIts){
    mTestIts = testIts;
    if(NO_FILES == mState){
        mState = ONLY_TEST_FILE;
    }else if(ONLY_GOLDEN_FILE == mState){
        mState = READY;
    }
}

void CItsApp::setGoldenIts(const itsGolden &goldenIts){
    mGoldenIts = goldenIts;
    if(NO_FILES == mState){
        mState = ONLY_GOLDEN_FILE;
    }else if(ONLY_TEST_FILE == mState){
        mState = READY;
    }
}

void CItsApp::moveToFrame(int index){
    assert(index >= 0);
    mProcFrameIdx = index;

    // fixme later, add use capture.get property to check it is video
    string mediaFileName;
    if(!mCapture.isOpened()){
        if(ONLY_TEST_FILE == mState || READY == mState){
            mediaFileName = mTestIts.getFileName();
        }else if(ONLY_GOLDEN_FILE){
            mediaFileName = mGoldenIts.getFileName();
            mCapture.open(mGoldenIts.getFileName());
        }
        mCapture.open(mediaFileName);
    }

    if(mCapture.isOpened()){
        mCapture.set(CV_CAP_PROP_POS_FRAMES, index);
    }else{
        qDebug() << "Could not open " << QString::fromStdString(mediaFileName);
    }

    mCapture >> mProcFrame;
}

void CItsApp::stepFrame(int step)
{
    mProcFrameIdx += step;
    mProcFrameIdx = std::max(mProcFrameIdx, 0);
    moveToFrame(mProcFrameIdx);
}

void CItsApp::showProcResult(){
    if(mProcFrame.rows >0 ){
        imshow(mProcWinTitle, mProcFrame);
    }
}

int CItsApp::readImage( string &fileName){
    Mat image = imread(fileName);
    if(image.rows > 0){
        imshow("My Image", image);
        return 0;
    }else{
        return -1;
    }
}

int CItsApp::readVideo(){
    if(mMediaFileName.length() > 0){
        if(!mCapture.isOpened()){
            throw "Error when reading video\n";
            return -1;
        }

        namedWindow(mProcWinTitle);

        for(;;){
            mProcFrameIdx++;
            mCapture >> mProcFrame;
            if(mProcFrame.empty()) break;
            // it will make every frame show a window
            // sprintf(mProcWinTitle, "frame %d", (int)(mCapture.get(CV_CAP_PROP_POS_FRAMES)));
            imshow(mProcWinTitle, mProcFrame);
            char key=waitKey(20);
            if('Q' == key || 'q' == key) break;
        }
        destroyAllWindows();
    }
    return 0;
}

double CItsApp::evaluate(const CATEGORIES &categ){
    if(READY == mState){
        mTotalScore = mTestIts.evaluate(mGoldenIts, categ);
        mFrameScores = mTestIts.getFrameScores();
    }else{
        qDebug() << "CItsApp does not contain test file & golden file information" << endl;
        mTotalScore = -1;
    }
    return mTotalScore;
}

double CItsApp::getCurrentFrameScore(){
    return mFrameScores[mProcFrameIdx];
}

double CItsApp::getTotalScore(){
    return mTotalScore;
}

double CItsApp::getFrameScore(int index){
    return mFrameScores[index];
}
