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

// fixme later, the following all should write as a Test
#define MODIFY_JSON_VALUE 0
#define EVALUATE_GOLDEN_DEMO 0
#define READ_VIDEO_DEMO 1
#define DRAW_OBJECTS_IN_VIDEO_DEMO 0
#define EVALUATE_TEST_AND_GOLDEN_DEMO 0
#define USE_CUSTOM_VIEW 1

#if USE_CUSTOM_VIEW
#include "analyzegoldenwindow.h"
#else
#include "mainwindow.h"
#endif

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <vector>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

#include "itsFrame.h"
#include "itsobject.h"
#include "itsgolden.h"

#include "citsapp.h"

using namespace cv;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(mediaalgomanager);

    QApplication app(argc, argv);
    app.setOrganizationName("HwangTaiChi");
    app.setApplicationName("OpenCV meets Qt");

    #if MODIFY_JSON_VALUE
    itsGolden golden(QStringLiteral("config/V2014-07-15-15-21-20.json"));
    golden.deleteObjects(CAR);
    #endif

    #if EVALUATE_GOLDEN_DEMO
    itsGolden golden(QStringLiteral("config/V2014-07-15-15-21-20.json"));
    itsGolden compareGolden(QStringLiteral("config/V2014-07-15-15-21-20-2.json"));
    double compareScore = golden.evaluate(compareGolden, CAR);
    qDebug() << "score" << compareScore << endl;
    #endif

    #if READ_VIDEO_DEMO
    itsGolden golden(QStringLiteral("config/V2014-07-15-15-21-20.json"));
    CItsApp itsApp(golden.getFileName());
    itsApp.readVideo();
    #endif

    #if DRAW_OBJECTS_IN_VIDEO_DEMO
    itsGolden golden(QStringLiteral("config/V2014-07-15-15-21-20.json"));
    CItsApp itsApp(golden.getFileName());
    int index=1;
    itsApp.moveToFrame(index);
    itsFrame itsframe = golden.getFrame(index);
    itsApp.displayItsFrame(itsApp.getProcFrame(), &itsframe, Scalar(255, 0, 0));

    itsApp.showProcResult();
    #endif

    #if EVALUATE_TEST_AND_GOLDEN_DEMO
    itsGolden testFile(QStringLiteral("config/V2014-07-15-15-21-20.json"));
    itsGolden goldenFile(QStringLiteral("config/V2014-07-15-15-21-20-2.json"));
    CItsApp itsApp(testFile, goldenFile);
    qDebug() << "score" << itsApp.evaluate(CAR) << endl;
    #endif

#if USE_CUSTOM_VIEW
    analyzeGoldenWindow goldenWindow;
    goldenWindow.show();
#else
    MainWindow mainWin;
    mainWin.show();
#endif

    return app.exec();
}

