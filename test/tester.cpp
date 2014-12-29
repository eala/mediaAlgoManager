#include <QtTest/QtTest>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "itsgolden.h"
#include "citsapp.h"

// ---------------------------------------------------------------------------
// initTestCase() will be called before the first test function is executed
// cleanupTestCase() will be called after the last test function was executed
// init() will be called before "each" function is executed
// cleanup() will be called after every test function
// ---------------------------------------------------------------------------

class Tester: public QObject
{
    Q_OBJECT
private slots:
    // init & cleanup
    void initTestCase();
    void cleanupTestCase();

    // test functions
    void readFrame();
    void readVideo();
    void evaluateGolden();
    void evaluateGoldenByItsApp();

private:
    itsGolden *mTestIts;
    itsGolden *mGoldenIts;
    CItsApp *mItsApp;
};

void Tester::initTestCase()
{
    mTestIts = new itsGolden(QStringLiteral("config/V2014-07-15-15-21-20.json"));
    mGoldenIts = new itsGolden(QStringLiteral("config/V2014-07-15-15-21-20-2.json"));
    mItsApp = new CItsApp(*mTestIts, *mGoldenIts);
    QVERIFY(mTestIts != NULL);
    QVERIFY(mGoldenIts != NULL);
    QVERIFY(mItsApp != NULL);
}

void Tester::cleanupTestCase()
{
    if(mTestIts){
        delete mTestIts;
        mTestIts = NULL;
    }

    if(mGoldenIts){
        delete mGoldenIts;
        mGoldenIts = NULL;
    }

    if(mItsApp){
        delete mItsApp;
        mItsApp = NULL;
    }
}

void Tester::readFrame()
{
    const int index=1;
    mItsApp->moveToFrame(index);
    Mat img = mItsApp->getProcFrame();
    qDebug() << "img.rows: " << img.rows << ", img.cols: " << img.cols;
    QVERIFY(img.rows > 0 && img.cols > 0);
}

void Tester::readVideo()
{   // we only check: different frame indices, different frame contents
    mItsApp->moveToFrame(1);
    Mat firstImg = mItsApp->getProcFrame().clone();
    cvtColor(firstImg, firstImg, CV_BGR2GRAY);
    mItsApp->moveToFrame(1000);
    Mat secondImg = mItsApp->getProcFrame().clone();
    cvtColor(secondImg, secondImg, CV_BGR2GRAY);

#if 0
    imshow("first frame", firstImg);
    imshow("second img", secondImg);
    imshow("frames diff", (firstImg - secondImg));
    waitKey(0);
#endif

    int diffCount = countNonZero(firstImg - secondImg);
    qDebug() << "different pixel counts: " << diffCount;
    QVERIFY(diffCount > 0);
}

void Tester::evaluateGolden()
{
    double compareScore = mTestIts->evaluate(*mGoldenIts, CAR);
    QCOMPARE(compareScore, 2.0);
    //QVERIFY(compareScore != 3.0);
}

void Tester::evaluateGoldenByItsApp()
{
    double score = mItsApp->evaluate(CAR);
    qDebug() << "score: " << score;
    QCOMPARE(score, 2.0);
    //QVERIFY(score != 3.0);
}

QTEST_MAIN(Tester)
#include "tester.moc"
