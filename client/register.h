#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QJsonObject>
#include <communicate.h>
#include "common.h"
#include <QMessageBox>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(Communicate *com,QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_reg_reg_Button_clicked();

    void on_reg_back_Button_clicked();

    void rstResult(int );

private:
    Ui::Register *ui;
    Communicate *_com;
};

#endif // REGISTER_H
