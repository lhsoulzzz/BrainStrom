#ifndef STRUCT_DATA_H
#define STRUCT_DATA_H

struct Head
{
    int type;
    long long len;
};

struct Data
{
    int type;
    int len;
    char buf[32];
};

struct FileDataInfo
{
    Head head; // 用来表示数据头
    // 以下是数据体部分
    char file_name[256];  // 文件名字
    long long file_size; // 文件可能超大，得用长整型
    char md5[20]; //  md5值用于校验文件完整性的
    long long content_size; // 本次发送的内容长度
    char file_content[0]; // 文件内容，柔性数组，可以存放任意长度的内容
    FileDataInfo() {}
};

// 查询服务端都有哪些文件可以下载
struct SearchFileReq
{
    Head head;
    SearchFileReq() {
        head.type = 200;
        head.len = sizeof(SearchFileReq);
    }
};

struct FileInfo // 文件信息结构体
{
    char file_name[256]; // 包含文件完整路径的名字
    long long file_size;
    char md5[20]; // 文件md5,用于校验完整性的

};

struct SearchFileRes
{
    Head head;
    int file_cnt; // 文件数量
    FileInfo file_info[0]; // 柔性数组，可以有任意个文件数量的信息
    SearchFileRes() {
        head.type = 201;
        head.len = sizeof(SearchFileRes); // 这2个数据最后还得重新赋值
        // 真正的长度还应该包含 柔性数组里面元素的大小 * 个数
    }
};

// 下载
struct DowloadFileReq
{
    Head head;
    char file_name[256];
    DowloadFileReq() {
        head.type = 202;
        head.len = sizeof(DowloadFileReq);
    }
};

// 下载响应，只需要S端给C端发送 FileDataInfo，type为203
#endif // STRUCT_DATA_H
