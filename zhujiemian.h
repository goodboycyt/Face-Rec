#ifndef ZHUJIEMIAN_H
#define ZHUJIEMIAN_H

#include <QDialog>
#include "QSound"
#include "jiance.h"
#include "shibie.h"
namespace Ui {
class zhujiemian;
}

class zhujiemian : public QDialog
{
    Q_OBJECT

public:
    explicit zhujiemian(QWidget *parent = 0);
    ~zhujiemian();


private slots:
    void on_kai_button_clicked();

    void on_guan_button_clicked();

    void on_add_button_clicked();

    void on_see_button_clicked();

private:
    Ui::zhujiemian *ui;
};

#endif // ZHUJIEMIAN_H
