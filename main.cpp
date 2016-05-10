#include "widget.h"
#include <QApplication>
#include <QSound>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowFlags((Qt::FramelessWindowHint));
/*    QSound bells("D:/左耳.wav");
    bells.play()*/;
    w.show();

    return a.exec();
}
