#ifndef ANALYZEGOLDENWINDOW_H
#define ANALYZEGOLDENWINDOW_H

#include <QMainWindow>

namespace Ui {
class analyzeGoldenWindow;
}

class analyzeGoldenWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit analyzeGoldenWindow(QWidget *parent = 0);
    ~analyzeGoldenWindow();

private:
    Ui::analyzeGoldenWindow *ui;
};

#endif // ANALYZEGOLDENWINDOW_H
