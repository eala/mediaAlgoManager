#include "analyzegoldenwindow.h"
#include "ui_analyzegoldenwindow.h"

analyzeGoldenWindow::analyzeGoldenWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::analyzeGoldenWindow)
{
    ui->setupUi(this);
}

analyzeGoldenWindow::~analyzeGoldenWindow()
{
    delete ui;
}
