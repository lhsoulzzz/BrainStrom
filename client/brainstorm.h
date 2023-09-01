#ifndef BRAINSTORM_H
#define BRAINSTORM_H

#include <QTimer>
#include <QDialog>
#include <QMovie>
#include "communicate.h"


namespace Ui {
class BrainStorm;
}

class BrainStorm : public QDialog
{
    Q_OBJECT

public:
    explicit BrainStorm(Communicate *com,QJsonObject &json,QWidget *parent = nullptr);
    ~BrainStorm();
private:
    //单人训练获取题目
    void singleGetQuestion();
    //单人训练设置问题
    void singleSetQuestion();
    //单人训练回答问题
    void singleAnswerQuestion(int select);

    //设置得分
    void setEnemyScore();
    void setSelfScore();
private slots:
    //单人训练从服务器接收问题
    void receiveSingleQuestion(const QJsonObject &json);
    //个人赛倒计时
    void singleTimerOut();

    void on_singleButton_clicked();

    void on_sonButton_clicked();

    void on_sbackButto_clicked();

    void on_se_back_Button_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
//----------------------------------------------------------------------------------
    //接受排位
    void Rank(QJsonObject json);

    void on_rankButton_clicked();

    void on_pushButton_5_clicked();

    //rank设置问题
    void rankSetQuestion();

    //排位计时器
    void RankTimerOut();

    //rank结果
    void rankSetResult(QJsonObject &json);

    //rank答题
    void RankAnswerQuestion(int select);

    void on_rankSelectButton1_clicked();

    void on_rankSelectButton2_clicked();

    void on_rankSelectButton3_clicked();

    void on_rankSelectButton4_clicked();

private:
    Ui::BrainStorm *ui;
    Communicate *_com; //通信类指针 负责和服务器通信
    QJsonObject _singleQuestion;
    int _currentSingleQuestion;//当前问题下标
    QTimer _singleTimer;//个人训练定时器
    int _singleSec; //个人训练答题时间
    int _singleScore;
//------------------------------------------------------------
    QJsonObject _rankQuestion;   //rank的题
    QString     _enemyName;//对手的名字
    QString     _enemyRank;//对手的段位
    int         _enemyScore;   //对手的得分

    int _rankSec;     //rank 答题计时
    int _myScore;     //自己的得分

    QString _userName;//自己的名字
    QString _userRank;//自己的段位

    int _currentRankQuestion;//当前回答哪一个问题的下标

    int _enemyRankQuestion;//对手当前回答问题的下标

    QTimer _rankTimer;     //排位定时器

};

#endif // BRAINSTORM_H
