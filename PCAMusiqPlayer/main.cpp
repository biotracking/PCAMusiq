#include <QtGui/QApplication>
#include "MainWindow.h"

//namespace po = boost::program_options;

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
