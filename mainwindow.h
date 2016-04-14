#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scales.h"
#include "scanmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Scales sc;
    ScanManager* scanMan;
    unsigned int calibr1, calibr2;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonPortListUpdate_clicked();

    void on_pushButtonScan_clicked();

    void on_pushButtonCalibr1_clicked();

    void on_pushButtonCalibr2_clicked();

    void on_pushButtonReadSettings_clicked();

    void on_pushButtonWriteSettings_clicked();

    void confUpdate();
    void weightUpdate(unsigned int value);
    void adcUpdate(unsigned int value);
    void correctAnswer();
    void errorMessage(QString message);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
