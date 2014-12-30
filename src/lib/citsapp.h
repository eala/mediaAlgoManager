#ifndef CITSAPP_H
#define CITSAPP_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "itsFrame.h"
#include "itsgolden.h"

using namespace cv;

class CItsApp
{
public:
    enum EVALUATE_STATE{
        NO_FILES=0,
        ONLY_TEST_FILE,
        ONLY_GOLDEN_FILE,
        READY,
        UNKNOWN
    };

    CItsApp(const string &fileName);
    CItsApp(itsGolden &testIts, itsGolden &goldenIts);
    ~CItsApp();

    int readImage( string &fileName);
    int readVideo();

    void moveToFrame(int index);
    void displayItsFrame(Mat &frame, itsFrame *itsData, cv::Scalar color);
    Mat &getProcFrame(){return mProcFrame;}
    int getCurrentFrameIndex(){return mProcFrameIdx;}

    void setTestIts(const itsGolden &testIts){ mTestIts = testIts;}
    void setGoldenIts(const itsGolden &goldenIts){ mGoldenIts = goldenIts;}

    itsGolden &getTestIts() {return mTestIts;}
    itsGolden &getGoldenIts() {return mGoldenIts;}
    EVALUATE_STATE getState(){return mState;}

    vector<itsFrame> &getTestFrames() {return mTestIts.getFrames();}
    vector<itsFrame> &getGoldenFrames() {return mGoldenIts.getFrames();}

    // APIs
    double evaluate(const CATEGORIES &categ);   // return total scores
    double getCurrentFrameScore();
    double getTotalScore();
    double getFrameScore(int index);
    void showProcResult();

private:
    CItsApp();  // do not allow implicit initilization
    string mMediaFileName;
    VideoCapture mCapture;

    Mat mProcFrame;
    int mProcFrameIdx;
    char mProcWinTitle[128];

    itsGolden mTestIts;
    itsGolden mGoldenIts;

    EVALUATE_STATE mState;

    map<int, double> mFrameScores;
    double mTotalScore;
};

#endif // CITSAPP_H
