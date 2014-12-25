#include "citsapp.h"

CItsApp::CItsApp():mTestIts(NULL), mGoldenIts(NULL)
{
    sprintf(mProcWinTitle, "demo video");
}

CItsApp::CItsApp(const string &fileName) :mTestIts(NULL), mGoldenIts(NULL)
{
    sprintf(mProcWinTitle, "demo video");
    mMediaFileName = fileName;
    mCapture.open(mMediaFileName);
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

void CItsApp::moveToFrame(int index){
    assert(index >= 0);
    // fixme later, add use capture.get property to check it is video
    if(mCapture.isOpened()){
        mCapture.set(CV_CAP_PROP_POS_FRAMES, index);
    }

    mCapture >> mProcFrame;
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
            throw "Erro when reading video\n";
            return -1;
        }

        namedWindow(mProcWinTitle);

        for(;;){
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
