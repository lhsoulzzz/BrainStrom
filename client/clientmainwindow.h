#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "struct_data.h"
#include <QFileDialog> // 文件对话框
#include <QFile>
#include <QStandardItemModel>

namespace Ui {
class ClientMainWindow;
}

class ClientMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientMainWindow(QWidget *parent = 0);
    ~ClientMainWindow();

private slots:
    void on_pushButton_send_clicked();
    void myRead(); // 自定义收包槽

    void on_pushButton_send_2_clicked();

    void on_pushButton_send_3_clicked();

    void on_pushButton_send_4_clicked();

    void on_pushButton_send_5_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::ClientMainWindow *ui;
    QTcpSocket *m_client; // 客户端套接字
    QStandardItemModel *m_model;
};

#endif // CLIENTMAINWINDOW_H
