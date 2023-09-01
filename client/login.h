#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "communicate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void login(const QJsonObject &json);

    void on_log_to_rig_Button_clicked();

    void on_loginButton_clicked();

private:
    Ui::Widget *ui;

    Communicate *_com; //通信类指针 负责和服务器通信
};
#endif // WIDGET_H
