#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <qthread.h>

class I : public QThread
{
public:
    static void sleep(unsigned long secs) {
        QThread::sleep(secs);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap Elipicture("SplashScreen.png");
    QSplashScreen SplashScreenEli(Elipicture);
    SplashScreenEli.show();
    MainWindow w;
    I::sleep(6);
    w.showMaximized();
    SplashScreenEli.finish(&w);
    return a.exec();
}
