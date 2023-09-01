#include "brainstorm.h"
#include "ui_brainstorm.h"
#include "common.h"

BrainStorm::BrainStorm(Communicate *com,QJsonObject &json,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrainStorm)
{
    ui->setupUi(this);
    _com = com;

    _userName=json["userName"].toString();
    _userRank=json["userrank"].toString();

    connect(_com, SIGNAL(receiveSingleQuestion(const QJsonObject)), this, SLOT(receiveSingleQuestion(const QJsonObject)));
    connect(&_singleTimer,SIGNAL(timeout()), this, SLOT(singleTimerOut()));
    connect(_com,SIGNAL(Rank(QJsonObject)),this,SLOT(Rank(QJsonObject)));
    connect(&_rankTimer,SIGNAL(timeout()),this,SLOT(RankTimerOut()));
}

BrainStorm::~BrainStorm()
{
    delete ui;
}
//单人训练相关
void BrainStorm::singleTimerOut()
{
    --_singleSec;
    if(_singleSec == 0)
    {
        ui->singleResult->setText("回答错误");
        QString str = QString("本次得分：%1").arg(_singleScore);
        ui->singleScore->setText(str);
        _singleTimer.stop();
        ui->stackedWidget->setCurrentWidget(ui->single_end);
    }
    ui->lcdNumber->display(_singleSec);
}
void BrainStorm::singleGetQuestion()
{
    QJsonObject json;
    json["cmd"] = SINGLE_GET_QUESTION;

    _com->writeData(json);
}
//单人训练回答问题
void BrainStorm::singleAnswerQuestion(int select)
{
    if(select == _singleQuestion["answer"].toArray().at(_currentSingleQuestion).toString().toInt())
    {

        _singleScore +=_singleSec;
        _currentSingleQuestion++;
        singleSetQuestion();//设置下一趟
        _singleSec = 15;
        _singleTimer.stop();
        ui->lcdNumber->display(_singleSec);
        _singleTimer.start(1000);
    }
    else
    {
        ui->singleResult->setText("回答错误");
        QString str = QString("本次得分：%1").arg(_singleScore);
        ui->singleScore->setText(str);
        ui->stackedWidget->setCurrentWidget(ui->single_end);
        _singleTimer.stop();
    }

}

void BrainStorm::singleSetQuestion()
{

    if(QUESTION_NUM == _currentSingleQuestion)
    {
        ui->singleResult->setText("全部回答正确");
        QString str = QString("本次得分：%1").arg(_singleScore);
        ui->singleScore->setText(str);
        ui->stackedWidget->setCurrentWidget(ui->single_end);
        _singleTimer.stop();
        return ;
    }
    ui->singleQuestion->setText(_singleQuestion["question"].toArray().at(_currentSingleQuestion).toString());
    ui->pushButton->setText(_singleQuestion["selection1"].toArray().at(_currentSingleQuestion).toString());
    ui->pushButton_2->setText(_singleQuestion["selection2"].toArray().at(_currentSingleQuestion).toString());
    ui->pushButton_3->setText(_singleQuestion["selection3"].toArray().at(_currentSingleQuestion).toString());
    ui->pushButton_4->setText(_singleQuestion["selection4"].toArray().at(_currentSingleQuestion).toString());

}

void BrainStorm::receiveSingleQuestion(const QJsonObject &json)
{
    _singleQuestion = json;

    _singleSec = 15;
    _currentSingleQuestion = 0;
    _singleScore = 0;
    ui->lcdNumber->display(_singleSec);
    singleSetQuestion();

    //开启定时器
    _singleTimer.start(1000);

    ui->stackedWidget->setCurrentWidget(ui->single_running);
}


//选择个人训练赛
void BrainStorm::on_singleButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->SingleMenu);
}
//个人训练赛开始答题
void BrainStorm::on_sonButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->single_running);
    singleGetQuestion();
}

void BrainStorm::on_sbackButto_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);
}

void BrainStorm::on_se_back_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);
}

void BrainStorm::on_pushButton_clicked()
{
    singleAnswerQuestion(1);
}

void BrainStorm::on_pushButton_2_clicked()
{
    singleAnswerQuestion(2);
}

void BrainStorm::on_pushButton_3_clicked()
{
    singleAnswerQuestion(3);
}

void BrainStorm::on_pushButton_4_clicked()
{
    singleAnswerQuestion(4);
}

//-----------------------------------------------------------------------------------------------

//选择排位赛
void BrainStorm::on_rankButton_clicked()
{
    QJsonObject json;
    json["cmd"]=RANK;

    _com->writeData(json);

    //跳到等待页面
    ui->stackedWidget->setCurrentWidget(ui->rank_wait);
}
void BrainStorm::RankTimerOut()
{
    _rankSec--;
    if(_rankSec==0)
    {
        if(ui->rankSelectButton1->isEnabled())//判断随便一个按钮状态
        {
            _currentRankQuestion++;
        }
        rankSetQuestion();
        _rankSec=10;
    }
    ui->lcdNumber_2->display(_rankSec);
}

void BrainStorm::rankSetQuestion()
{
    ui->rankQuestion->setText(_rankQuestion["question"].toArray().at(_currentRankQuestion).toString());
    ui->rankSelectButton1->setText(_rankQuestion["selection1"].toArray().at(_currentRankQuestion).toString());
    ui->rankSelectButton2->setText(_rankQuestion["selection2"].toArray().at(_currentRankQuestion).toString());
    ui->rankSelectButton3->setText(_rankQuestion["selection3"].toArray().at(_currentRankQuestion).toString());
    ui->rankSelectButton4->setText(_rankQuestion["selection4"].toArray().at(_currentRankQuestion).toString());

    ui->rankSelectButton1->setEnabled(true);
    ui->rankSelectButton2->setEnabled(true);
    ui->rankSelectButton3->setEnabled(true);
    ui->rankSelectButton4->setEnabled(true);

    ui->rankSelectButton1->setStyleSheet("");
    ui->rankSelectButton2->setStyleSheet("");
    ui->rankSelectButton3->setStyleSheet("");
    ui->rankSelectButton4->setStyleSheet("");

    if(_currentRankQuestion==QUESTION_NUM)
    {
        _rankTimer.stop();
        //将结果发给服务器
        QJsonObject json;
        json["cmd"]       =RANKRESULT;
        json["score"]     =_myScore;
        json["enemyName"] =_enemyName;
        json["enemyScore"]=_enemyScore;

        _com->writeData(json);
    }
}

void BrainStorm::setEnemyScore()
{
    QString str=QString("%1(%2):%3").arg(_enemyName,-5).arg(_enemyRank).arg(_enemyScore);
    ui->enemyStatus->setText(str);
}

void BrainStorm::setSelfScore()
{
    QString str=QString("%1(%2):%3").arg(_userName,-5).arg(_userRank).arg(_myScore);
    ui->selfStatus->setText(str);
}


void BrainStorm::Rank(QJsonObject json)
{
    int cmd=json["cmd"].toInt();
    if(cmd==RANK)
    {
        _rankQuestion=json["question"].toObject();
        _enemyName   =json["enemyName"].toString();
        _enemyRank   =json["enemyRank"].toString();
        _enemyScore  =json["enemyScore"].toInt();

        _rankSec=10;
        _myScore=0;
        ui->lcdNumber_2->display(_rankSec);

        _currentRankQuestion=0;
        _enemyRankQuestion  =0;

        setEnemyScore();
        setSelfScore();
        rankSetQuestion();

        _rankTimer.start(1000);

        ui->stackedWidget->setCurrentWidget(ui->rank_running);
    }
    else if(cmd==ANSWER)//同步答题
    {
        _enemyScore       =json["enemyscore"].toInt();
        _enemyRankQuestion=json["enemyQuestionId"].toInt();
        setEnemyScore();

        if(_currentRankQuestion==_enemyRankQuestion)
        {
            _rankSec=10;
            _rankTimer.stop();
            ui->lcdNumber_2->display(_rankSec);
            _rankTimer.start(1000);
            rankSetQuestion();
        }
    }
    else if(cmd==RANKRESULT)
    {
        rankSetResult(json);
    }
}

void BrainStorm::rankSetResult(QJsonObject &json)
{
    QString newRank=json["newRank"].toString();
    if(_myScore==_enemyScore)
    {
        ui->rankresult->setText("平局");
    }
    else if(_myScore<_enemyScore)
    {
        ui->rankresult->setText("失败");
    }
    else if(_myScore>_enemyScore)
    {
        ui->rankresult->setText("成功");
    }

    QMovie * movie = new QMovie("C:/Users/soul/Desktop/Client/this_is_power.gif");
    ui->adv->setMovie(movie);
    movie->start();

    QString str=QString("%1 ---> %2").arg(_userRank).arg(newRank);
    ui->newrank->setText(str);
    _userRank=newRank;

    ui->stackedWidget->setCurrentWidget(ui->rank_result);
}


void BrainStorm::RankAnswerQuestion(int select)
{
    //计算得分
    if(select==_rankQuestion["answer"].toArray().at(_currentRankQuestion).toString().toInt())
        _myScore +=_rankSec;
    setSelfScore();
    _currentRankQuestion++;
    //判断是否跳到下一题
    if(_currentRankQuestion==_enemyRankQuestion)
    {
        _rankSec=10;
        _rankTimer.stop();
        ui->lcdNumber_2->display(_rankSec);
        _rankTimer.start(1000);
        rankSetQuestion();
    }

    QJsonObject json;
    json["cmd"]      =ANSWER;
    json["enemyName"]=_enemyName;
    json["score"]    =_myScore;
    json["questionId"] =_currentRankQuestion;

    _com->writeData(json);//发送给服务器
}

void BrainStorm::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);
}

void BrainStorm::on_rankSelectButton1_clicked()
{
    ui->rankSelectButton1->setStyleSheet("background-color: rgb(0, 170, 255)");
    ui->rankSelectButton1->setEnabled(false);
    ui->rankSelectButton2->setEnabled(false);
    ui->rankSelectButton3->setEnabled(false);
    ui->rankSelectButton4->setEnabled(false);
    RankAnswerQuestion(1);
}

void BrainStorm::on_rankSelectButton2_clicked()
{
    ui->rankSelectButton2->setStyleSheet("background-color: rgb(0, 170, 255)");
    ui->rankSelectButton1->setEnabled(false);
    ui->rankSelectButton2->setEnabled(false);
    ui->rankSelectButton3->setEnabled(false);
    ui->rankSelectButton4->setEnabled(false);
    RankAnswerQuestion(2);
}

void BrainStorm::on_rankSelectButton3_clicked()
{
    ui->rankSelectButton3->setStyleSheet("background-color: rgb(0, 170, 255)");
    ui->rankSelectButton1->setEnabled(false);
    ui->rankSelectButton2->setEnabled(false);
    ui->rankSelectButton3->setEnabled(false);
    ui->rankSelectButton4->setEnabled(false);
    RankAnswerQuestion(3);
}

void BrainStorm::on_rankSelectButton4_clicked()
{
    ui->rankSelectButton4->setStyleSheet("background-color: rgb(0, 170, 255)");
    ui->rankSelectButton1->setEnabled(false);
    ui->rankSelectButton2->setEnabled(false);
    ui->rankSelectButton3->setEnabled(false);
    ui->rankSelectButton4->setEnabled(false);
    RankAnswerQuestion(4);
}
