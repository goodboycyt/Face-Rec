#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_guan_clicked()
{
     this->close();

}

void Widget::on_pushButton_clicked()
{
    if(this->ui->user_text->text()=="cyt" && this->ui->pw_text->text()=="123456"){
        this->close();
        zhujiemian.show();
    }
}
