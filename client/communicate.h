#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


class Communicate : public QObject
{
    Q_OBJECT
public:
    explicit Communicate(QObject *parent = nullptr);

    void writeData(const QJsonObject &json);

signals:
    void rstResult(int );
    void login(const QJsonObject &json);
    void receiveSingleQuestion(const QJsonObject &json);
    void Rank(QJsonObject json);
public slots:
    void readData();
private:
    QTcpSocket s;

};

#endif // COMMUNICATE_H
