#define _CRT_SECURE_NO_WARNINGS

#include "menu.h"
#include "common.h"
#include "command.h"
#include "MFD.h"
#include "Fileoperate.h"
using namespace std;

// 定义全局变量，表示用户信息、文件信息、文件状态、文件簇等数据结构
vector<MFD> UsrInfo;              // 主文件目录项向量
vector<vector<UFD>> FileInfo;     // 用户文件目录项向量的向量 -----> 当前登录用户（第一维）下的 目录（第二维） 
vector<vector<UOF>> FileState;     // 用户打开文件项向量的向量 -----> 当前登录用户（第一维）下的 文件（第二维） 
vector<Cluster> FileCluster;       // 磁盘物理块向量

// 定义单个数据项的输入结构体
MFD UsrInput;                     // 用户输入的主文件目录项
UFD FileInput;                    // 用户输入的用户文件目录项
UOF StateInput;                   // 用户输入的用户打开文件项
Cluster ClusterInput;             // 用户输入的磁盘物理块

int num;                          // 选项选择变量
int Headnum;                      // 头节点编号
int curID;                        // 当前用户的编号


// 命令处理函数列表
Hand_to handlerlist[] = {
    {"chmod", chmod},{"rename", rename},{"cp", copy_check},
	{"cat", cat},{"passwd", chage_psw},{"login", login},
	{"logout", logout},{"create", create},{"rd", rd}, 
	{"write", write},{"help", help},{"ls", ls},{"save", save},
	{"register", Register},{"exit", exit},{"cls", clear},
    {NULL, NULL}
};


// 主程序入口
int main()
{
	system("color F0");
    loginWelcome();      // 登陆欢迎信息
    disk2mem();     // 初始化文件系统到内存

    // 主循环
    while (1)
    {
        // 默认以root用户登陆
        cout << "\\" << UsrInfo[curID].usrname << "> ";
        cmd_in_parse();    // 解析用户输入的命令
    }

    return 0;
}

