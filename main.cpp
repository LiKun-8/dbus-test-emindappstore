#include "softwarecenterwindow.h"
#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include "datamanager.h"
#include <QtQml>
#include <QLockFile>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    if(QFile("/tmp/lockFile").exists())
//        return 0;
//    else{
//        SoftwareCenterWindow::lockFile.lock();
//    }

    qmlRegisterType<DataManager>("com.emind.DataManager",1,0,"DataManager");

    QTranslator *qTranslator = new QTranslator();
    qTranslator->load(":/app_zh.qm");
    a.installTranslator(qTranslator);
    SoftwareCenterWindow w;
    SoftwareCenterWindow::setWinStyle(":/style.qss");
    w.show();

    return a.exec();
}
