#ifndef _MFD_H_
#define _MFD_H_

#include"common.h"
#include"command.h"
using namespace std;
/*
	MFD.h 用于定义数据结构、完成指令录入功能
*/


// 32个字节
// MFD 结构体定义，表示主文件目录项
typedef struct mfd
{
    char usrname[14];  // 用户名，14个字节 
    char usrpass[14];  // 用户密码，14个字节 
    int link;          // 该用户的UFD所在的盘号	4个字节 
} MFD;

// UFD 结构体定义，表示用户文件目录项
typedef struct ufd
{
    char filename[20];  // 文件名，20个字节 
    int mode;           // 文件权限，0 - readonly;  1 - writeonly;  2 - read / write  4个字节 
    int length;         // 文件长度（字节数）
    int addr;           // 文件起始物理块地址 4个字节 
} UFD;

// UOF 结构体定义，表示用户打开文件项
typedef struct uof
{
    char filename[20];  // 文件名，20个字节 
    int mode;           // 文件权限
    int state;          // 文件状态，0未打开, 1打开
    int write_poit;     // 写指针位置
} UOF;

// Cluster 结构体定义，表示磁盘上的一个物理块
typedef struct cluster
{
    int next_num;       // 下一个物理块的编号
    int is_data;        // 标识是否为数据块
    char data[257];     // 存储数据的数组，大小为256字节
} Cluster;

// func 类型定义，表示函数指针类型
typedef void(*func)(void);

// HAND_TO 结构体定义，表示命令处理函数的信息
typedef struct hand
{
    char *pname;        // 函数名
    func handler;       // 函数指针
} Hand_to;

// COMMAND 结构体定义，表示命令信息
typedef struct cmd
{
    string cmd_num[4];  // 命令的四个参数

} Command;


// 全局变量，表示当前命令的信息
Command cmd_in;

// 外部声明，表示命令处理函数列表 <---- main.cpp
extern Hand_to handlerlist[];

// 函数定义，用于解析输入的命令
void cmd_in_parse()
{
    string s;
    // 从标准输入获取一行命令
    getline(cin, s);
    
    for(int i=0; i < 4 ; i++)
    	cmd_in.cmd_num[i]="";
	
	// 如果没输入命令，不执行 
	if(s.length()==0){
		cout<<endl;
		return ;
	}
    // 使用字符串流解析命令
    stringstream stream(s);
    int i = 0;

    // 命令解析至 cmd_in 中
    while (stream >> cmd_in.cmd_num[i])
        i++;

    // 遍历命令列表
    int flag = 0;
    for (int i = 0; handlerlist[i].pname != NULL; i++)
    {
        // 判断命令是否在命令列表中
        if (strcmp(handlerlist[i].pname, cmd_in.cmd_num[0].c_str()) == 0)
        {
            flag = 1;
            // 调用相应的命令处理函数
            handlerlist[i].handler();
            break;
        }
    }

    // 输出未找到命令的提示
    if (!flag)
        cout << "命令无效!!!" << endl;
}



#endif _MFD_H_
