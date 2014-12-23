/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include "mainwindow.h"

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

#include "itsFrame.h"
#include "itsobject.h"

using namespace cv;

class CApp{
public:
    static int readImage(const string &fileName);
    static int readVideo(const string &fileName);
};

int CApp::readImage(const string &fileName){
    Mat image = imread(fileName);
    if(image.rows > 0){
        imshow("My Image", image);
        return 0;
    }else{
        return -1;
    }
}

// failed, fixme later
// error message: GStreamer: Error opening bin: Unrecoverable syntax error while parsing pipeline
int CApp::readVideo(const string &fileName){
    VideoCapture capture(fileName);
    if(!capture.isOpened()){
        throw "Erro when reading video\n";
        return -1;
    }

    Mat frame;
    const string windowName("demo video");
    namedWindow(windowName);
    for(;;){
        capture >> frame;
        if(frame.empty()) break;
        imshow(windowName, frame);
        char key=waitKey(20);
        if('Q' == key || 'q' == key) break;
    }
    destroyAllWindows();
    return 0;
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(mediaalgomanager);

    QApplication app(argc, argv);
    app.setOrganizationName("HwangTaiChi");
    app.setApplicationName("OpenCV meets Qt");

    // load json file
    QFile loadFile(QStringLiteral("/Users/hank/allProjects/install/its_env/V2014-07-15-15-21-20.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    // debug
    // qDebug() << saveData;

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject jsonObject = loadDoc.object();

    QJsonArray framesArray = jsonObject["frames"].toArray();

    QList<itsFrame> itsFrameArray;
    for(int i=0; i< framesArray.size(); ++i){
        itsFrame itsframe;
        itsframe.read(framesArray[i].toObject());
        itsFrameArray.push_back(itsframe);
    }

    MainWindow mainWin;
    mainWin.show();

    CApp::readVideo("/Users/hank/allProjects/install/its_env/0815-7.avi");



    return app.exec();
}

