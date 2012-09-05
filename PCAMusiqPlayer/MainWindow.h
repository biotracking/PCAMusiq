#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>

#include "PCAPlayer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel image;

    PCAPlayer player;

public slots:
    void newVideoFrame(QImage frame);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
