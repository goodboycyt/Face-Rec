#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "zhujiemian.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_guan_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    zhujiemian zhujiemian;
};

#endif // WIDGET_H
