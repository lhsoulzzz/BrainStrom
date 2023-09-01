#include "communicate.h"
#include<QHostAddress>
#include "common.h"

Communicate::Communicate(QObject *parent) : QObject(parent)
{
    //连接服务器
    s.connectToHost(QHostAddress("192.168.90.119"), 8000);

    connect(&s,SIGNAL(readyRead()),this,SLOT(readData()));
}

void Communicate::readData()
{
    QByteArray data;
    while(s.bytesAvailable())
    {
        data +=s.readAll();//读完
    }

    //数据解析
    QJsonDocument dt=QJsonDocument::fromJson(data);//通过QJsonDocument转换成json字符串
    if(dt.isNull())//没有读到数据
    {
        return;
    }

    QJsonObject root=dt.object();//拿到json这样的一个对象

    //具体的逻辑处理
    int cmd=root["cmd"].toInt();
    switch (cmd)
    {
    case REGISTER:
        emit rstResult(root["result"].toInt());//发给注册那一端
        break;
    case LOGIN:
        emit login(root);
        break;
    case SINGLE_GET_QUESTION:
        emit receiveSingleQuestion(root["question"].toObject());
        break;
    case RANK:
        emit Rank(root);
        break;
    case ANSWER:
        emit Rank(root);
        break;
    case RANKRESULT:
        emit Rank(root);
        break;
    default:
        break;
    }
}

void Communicate::writeData(const QJsonObject &json)
{
    QJsonDocument d(json);
    QByteArray sendData=d.toJson();
    int len=sendData.size();

    s.write((char *)&len,sizeof(int));//发送数据长度
    s.write(sendData);//发送数据
}

