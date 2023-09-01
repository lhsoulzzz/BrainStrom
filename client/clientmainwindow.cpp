#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"
#include <QDebug>

ClientMainWindow::ClientMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientMainWindow)
{
    ui->setupUi(this);
    // MVD相关
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(3);
    QStringList labels;
    labels<<"文件名"<<"大小"<<"MD5";
    m_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setModel(m_model); // 关联视图和模型
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 网络相关
    m_client = new QTcpSocket(this);
    m_client->connectToHost("127.0.0.1",12345);
    if(m_client->waitForConnected()){
        qDebug()<<"conn ok";
        connect(m_client,SIGNAL(readyRead()),this,SLOT(myRead()));
    }else{
        qDebug()<<"conn fail"<<m_client->errorString();
    }
}

ClientMainWindow::~ClientMainWindow()
{
    delete ui;
}

void ClientMainWindow::on_pushButton_send_clicked()
{
    char buffer[] = "mayibase is very good.";
    qint64 len = m_client->write(buffer,sizeof(buffer));
    qDebug()<<len;
}

QByteArray g_buffer; // 全局保存封包内容，防止异步收包一次收不完
void ClientMainWindow::myRead()
{
    //m_client->bytesAvailable();// 获取等待被收包读取的字节数，大于0表示有内容可读
    QByteArray bufferCur = m_client->readAll();
    qDebug()<<"收到封包的大小:"<<bufferCur.size();

    g_buffer.append(bufferCur); // 把当前封包保存到全局buffer中
    int len = g_buffer.size(); // 取出全局buffer的总长
    qDebug()<<"全局总长:"<<len;
    while(len > 0){
        qDebug()<<"全局总长:"<<len;
        if(len < sizeof(Head)){ // 小于包头的长度，继续收
            break;
        }
        Head *head = (Head*)g_buffer.data();
        qDebug()<<"全局总长:"<<len<<"封包总长:"<<head->len;
        if(len < head->len){ // 小于封包总长度，继续收
            break;
        }
        // 说明完整收完一个功能包了，可以解包了
        // 拿出前面一个完整功能包的内容
        QByteArray buffer = g_buffer.left(head->len);
        // 进行解包即可


        int type = *(int*)buffer.data();
        if(type == 100){
            Data *d = (Data*)buffer.data();
            qDebug()<<"len="<<d->len<<"内容："<<d->buf;
        }else if(type == 201){
            SearchFileRes *res = (SearchFileRes*)buffer.data();
            //m_model->setRowCount(0); // 先清空模型的数据
            m_model->setRowCount(res->file_cnt);
            for(int i=0;i<res->file_cnt;++i){
                m_model->setItem(i,0,new QStandardItem(res->file_info[i].file_name));
                m_model->setItem(i,1,new QStandardItem(QString::number(res->file_info[i].file_size)));
            }
            ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        else if(type == 203){
            qDebug()<<"收到下载响应";
            FileDataInfo * fdi = (FileDataInfo*)buffer.data();
            QFile file(fdi->file_name); // 这里只能是本地路径的文件名或者完整路径
            if(file.open(QIODevice::WriteOnly)){
                file.write(fdi->file_content,fdi->content_size);
                file.flush();
                file.close();
                qDebug()<<"下载完成"<<fdi->file_name<<"总长:"<<fdi->head.len;
            }else{
                qDebug()<<"下载保存失败:"<<fdi->file_name;
            }

        }

        // 刷新全局buffer和总长len
        g_buffer = g_buffer.mid(head->len); // mid() 表示从指定位置开始往后到末尾拿数据
        len = g_buffer.size();
    }

}

void ClientMainWindow::on_pushButton_send_2_clicked()
{
    char buffer[] = "码蚁真好";
    QString buf = "码蚁真好";
    //qint64 len = m_client->write(buffer,sizeof(buffer));
    qint64 len = m_client->write(buf.toLocal8Bit().data(),buf.toLocal8Bit().length());
    qDebug()<<len;
}

void ClientMainWindow::on_pushButton_send_3_clicked()
{
    Data d;
    d.type = 100;
    d.len = sizeof(d);
    strcpy(d.buf,"mayi");
    int len = m_client->write((char*)&d,d.len);
    qDebug()<<len;
}

void ClientMainWindow::on_pushButton_send_4_clicked()
{
    // 发送文件
    QString filePath = QFileDialog::getOpenFileName();
    qDebug()<<"filePath:"<<filePath;

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"open file fail";
        return;
    }
    // 成功打开了
    long long allSize = sizeof(FileDataInfo)+file.size();
    FileDataInfo *fileInfo = (FileDataInfo*)malloc(allSize);
    fileInfo->head.type = 101; // 类型
    fileInfo->head.len = allSize;
    QString name = file.fileName().split("/").back(); // 按斜杠/切割，取最后一段
    strcpy(fileInfo->file_name,name.toLocal8Bit().data());
    fileInfo->file_size = file.size();
    fileInfo->content_size = file.size();
    // readAll()只能调用一次，因为读完文件指针会跑到文件末尾，想再次读得移动指针到头部
    QByteArray fileData = file.readAll();
    memcpy(fileInfo->file_content,fileData.data(),fileData.size());
    // MD5值后期再给
    // 发包
    qint64 len = m_client->write((char*)fileInfo,fileInfo->head.len);

//    QByteArray fileData = file.readAll();
//    int len = m_client->write(fileData);


    qDebug()<<"发送len:"<<len;
    file.close(); // 用完了，关闭文件
    // 用完了，记得释放内存，否则会导致内存泄露
    free(fileInfo);
    fileInfo = nullptr; // 防止悬空指针(野指针)
}

void ClientMainWindow::on_pushButton_send_5_clicked()
{
    SearchFileReq req;
    qDebug()<<"查询文件请求"<<m_client->write((char*)&req,req.head.len);

}

void ClientMainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    // 双击下载
    QString file_name = m_model->item(index.row(),0)->text();
    DowloadFileReq req;
    strcpy(req.file_name,file_name.toLocal8Bit().data());
    qDebug()<<"请求下载"<<m_client->write((char*)&req,req.head.len);

}
