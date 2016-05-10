#include "zhujiemian.h"
#include "ui_zhujiemian.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "QSound"
#include <stdio.h>
#include <string.h>
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "qdebug.h"
#include <QDesktopServices>
#include <QUrl>
zhujiemian::zhujiemian(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::zhujiemian)
{
    ui->setupUi(this);


}

zhujiemian::~zhujiemian()
{
    delete ui;
}

QSound bells("D:/左耳.wav");


void zhujiemian::on_kai_button_clicked()
{
    jiance();
QPixmap a=QPixmap("D:\\saveImage.jpg");

    ui->carema_label->setPixmap(a);
    double leastDistSq1;
    leastDistSq1=recognize();
    qDebug()<<leastDistSq1<<endl;
    int b= leastDistSq1;
    qDebug()<<b<<endl;
    if(b>10000000){

       bells.play();
    }

}

void zhujiemian::on_guan_button_clicked()
{
    this->close();
}

void zhujiemian::on_add_button_clicked()
{
    add_jiance();
    learn();
    qDebug()<<"over learn"<<endl;

}

void zhujiemian::on_see_button_clicked()
{
    QDesktopServices::openUrl(QUrl("D:/", QUrl::TolerantMode));
}
