#include "mainwindow.hpp"

#include <QApplication>
#include <QDebug>
#include <QLoggingCategory>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
