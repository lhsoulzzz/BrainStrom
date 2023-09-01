#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include"register.h"
#include"communicate.h"

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
    void on_logButton_clicked();

    void on_regButton_clicked();

    void login(QJsonObject json);

private:
    Ui::Widget *ui;
    Communicate *_com;//通信类指针，负责和服务器通信
};
#endif // WIDGET_H
