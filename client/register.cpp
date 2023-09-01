#include "register.h"
#include "ui_register.h"

Register::Register(Communicate *com,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    _com = com;

    connect(_com, SIGNAL(rstResult(int)), this, SLOT(rstResult(int)));
}

void Register::rstResult(int ret)
{
    switch(ret)
    {
    case OK:
        QMessageBox::information(this,"注册","注册成功");
        break;
    case ERROR:
        QMessageBox::critical(this,"注册","注册失败 未知错误");
        break;
    case EXISTENCE:
        QMessageBox::critical(this,"注册","用户已存在 请重新注册");
        break;
    default:
        break;
    }
}

Register::~Register()
{
delete ui;
}

void Register::on_reg_reg_Button_clicked()
{
    QString userName = ui->lineEdit->text();
    QString passwd = ui->lineEdit_2->text();

    //发送注册信息
    QJsonObject json;
    json["cmd"] = REGISTER;
    json["userName"] = userName;
    json["passwd"] = passwd;
    //给服务器发送数据
    _com->writeData(json);
}

void Register::on_reg_back_Button_clicked()
{
    close();
}
