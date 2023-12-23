#ifndef _FILEOPERATE_H_
#define _FILEOPERATE_H_


#include "common.h"
#include "MFD.h"
using namespace std;

// 声明外部全局变量，用于存储用户信息、计数、输入用户信息、头部数量、文件状态信息、
// 输入文件信息、输入文件状态信息、输入文件簇信息、用户文件目录信息、文件簇信息
extern vector<MFD> UsrInfo;                // 存储主文件目录信息
extern int num;                            // 用于计数的整数
extern MFD UsrInput;                       // 用于输入用户信息的结构体对象
extern int Headnum;                         // 存储头部数量的整数
extern vector<vector<UOF>> FileState;      // 二维向量存储用户文件状态信息
extern UFD FileInput;                      // 用于输入文件信息的结构体对象
extern UOF StateInput;                      // 用于输入文件状态信息的结构体对象
extern Cluster ClusterInput;                // 用于输入文件簇信息的结构体对象
extern vector<vector<UFD>> FileInfo;       // 二维向量存储用户文件目录信息
extern vector<Cluster> FileCluster;        // 存储文件簇信息

// 功能: 从磁盘文件 "disk.txt" 中读取初始化信息，包括用户信息、文件信息和磁盘块信息。
void disk2mem()
{
    FILE *streamInit;

    // 打开磁盘文件，如果打开失败则输出错误信息并返回
    if ((streamInit = fopen("disk.txt", "r")) == NULL)
    {
        cout << "从磁盘中读入时失败！" << endl;
        return;
    }

    // 读入用户信息
    fscanf(streamInit, "%d", &Headnum);
    int alreadynum;
    int ret;
    alreadynum = 0;

    // 逐行读取用户信息，将信息存储到 UsrInfo 中
    while (alreadynum < Headnum && (ret = fscanf(streamInit, "%s %s %d", &UsrInput.usrname, &UsrInput.usrpass, &UsrInput.link)) != -1)
    {
        UsrInfo.push_back(UsrInput);
        alreadynum++;
    }

    // 初始化文件信息
    vector<UFD> t;
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        FileInfo.push_back(t);
    }

    // 初始化文件状态信息
    vector<UOF> temp;
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        FileState.push_back(temp);
    }

    // 逐用户读取文件信息
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        fscanf(streamInit, "%d", &Headnum);
        alreadynum = 0;

        // 逐行读取文件信息，将信息存储到 FileInfo 中
        while (alreadynum < Headnum && (ret = fscanf(streamInit, "%s %d %d %d", &FileInput.filename, &FileInput.mode, &FileInput.length, &FileInput.addr)) != -1)
        {
            FileInfo[i].push_back(FileInput);
            alreadynum++;
        }
    }

    // 逐用户读取文件状态信息
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        fscanf(streamInit, "%d", &Headnum);
        alreadynum = 0;

        // 逐行读取文件状态信息，将信息存储到 FileState 中
        while (alreadynum < Headnum && (ret = fscanf(streamInit, "%s %d %d %d", &StateInput.filename, &StateInput.mode, &StateInput.state, &StateInput.write_poit)) != -1)
        {
            FileState[i].push_back(StateInput);
            alreadynum++;
        }
    }

    alreadynum = 0;
    char Tempbuf[258];

    // 逐块读取磁盘块信息，将信息存储到 FileCluster 中
    int total = 0;
    char c;
    while (total < 68)
    {
        total++;
        memset(Tempbuf, 0, sizeof(Tempbuf));
        if ((ret = fscanf(streamInit, "%d %d", &ClusterInput.next_num, &ClusterInput.is_data)) != -1)
        {
            if (ClusterInput.is_data == 1)
            {
                fscanf(streamInit, "%c", &c);
//                cout<<"c = "<<c<<endl;
                fgets(Tempbuf, 257, streamInit);
            }
            strcpy(ClusterInput.data, Tempbuf);
//            cout<<ClusterInput.next_num<<"  "<<ClusterInput.is_data<<"  "<<ClusterInput.data<<endl;
            FileCluster.push_back(ClusterInput);
        }
        else
        {
            ClusterInput.next_num = alreadynum;
            ClusterInput.is_data = 0;
            strcpy(ClusterInput.data, Tempbuf);

            FileCluster.push_back(ClusterInput);
        }
        alreadynum++;
    }

    // 关闭文件流
    fclose(streamInit);
}


// 将系统信息输出到文件
void out_to_file()
{
    FILE* fd;
    // 打开文件以写入模式
    fd = fopen("disk.txt", "w");

    // 输出用户信息
    fprintf(fd, "%d%c", UsrInfo.size(), ' ');
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        fprintf(fd, "%s %s %d%c", UsrInfo[i].usrname, UsrInfo[i].usrpass, UsrInfo[i].link, ' ');
    }
    fprintf(fd, "\n");

    // 输出文件信息
    for (int i = 0; i < FileInfo.size(); i++)
    {
        fprintf(fd, "%d%c", FileInfo[i].size(), ' ');
        for (int j = 0; j < FileInfo[i].size(); j++)
        {
            fprintf(fd, "%s %d %d %d%c", FileInfo[i][j].filename, FileInfo[i][j].mode, FileInfo[i][j].length, FileInfo[i][j].addr, ' ');
        }
        fprintf(fd, "\n");
    }

    // 输出文件状态信息
    for (int i = 0; i < FileState.size(); i++)
    {
        fprintf(fd, "%d%c", FileState[i].size(), ' ');
        for (int j = 0; j < FileState[i].size(); j++)
        {
            fprintf(fd, "%s %d %d %d%c", FileState[i][j].filename, FileState[i][j].mode, FileState[i][j].state, FileState[i][j].write_poit, ' ');
        }
        fprintf(fd, "\n");
    }

    // 输出簇信息
    for (int i = 0; i < FileCluster.size(); i++)
    {
        fprintf(fd, "%d %d%c", FileCluster[i].next_num, FileCluster[i].is_data, ' ');
        fputs(FileCluster[i].data, fd);
        fprintf(fd, "\n");
    }

    // 关闭文件
    fclose(fd);
}


#endif
