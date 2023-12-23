#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "common.h"
#include "MFD.h"
#include "Fileoperate.h"
#include<string.h>

// 声明外部全局变量，用于存储文件信息、用户信息和文件状态
extern vector<vector<UFD>> FileInfo;    // 二维向量存储用户文件目录信息
extern UFD FileInput;                   // 用于输入文件信息的结构体对象
extern vector<Cluster> FileCluster;     // 存储文件簇信息
extern vector<MFD> UsrInfo;             // 存储主文件目录信息
extern vector<vector<UOF>> FileState;   // 二维向量存储用户文件状态信息
extern UOF StateInput;                   // 用于输入文件状态信息的结构体对象
extern Cluster ClusterInput;            // 用于输入文件簇信息的结构体对象


// 修改文件权限
void chmod()
{
    int temp,index=-1;

    // 遍历文件信息表，找到目标文件并修改权限
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
        	index=i;
            stringstream ss;
            ss << cmd_in.cmd_num[2];
            ss >> temp;
            FileInfo[curID][i].mode = temp;
            break;
        }
    }

	if(index==-1){
		cout<<"文件名不存在！！！"<<endl;
		return ;
	}

    // 遍历打开文件状态表，找到目标文件并修改权限
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            FileState[curID][i].mode = temp;
            break;
        }
    }

    cout << "修改权限成功!" << endl;
}

// 重命名文件
void rename()
{
	if(cmd_in.cmd_num[2]==cmd_in.cmd_num[1]){
		cout<<"旧文件名应于新文件名不同！！！"<<endl;
		return ;
	}
	
    for(int i=0;i<FileInfo[curID].size();i++){
    	if(strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str())==0){
    		cout<<"新文件名与现有文件名重复，请重新设置！！！"<<endl;
    		return ;
		}	
	}

	int index = -1;	
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
        	index = i;
            break;
        }
    }
	
	if(index==-1){
		cout<<"旧文件名不存在！！！"<<endl;
		return ;
	}
	else{
		
		// 修改文件名
            strcpy(FileInfo[curID][index].filename, cmd_in.cmd_num[2].c_str());
            
		// 修改打开文件状态表中的文件名
	    for (int i = 0; i < FileState[curID].size(); i++)
	    {
	        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
	        {
	            strcpy(FileState[curID][i].filename, cmd_in.cmd_num[2].c_str());
	            break;
	        }
	    }
	
	    cout << "重命名文件成功" << endl;
	}
    
}


// 临时写操作函数
void copy_write();

// 复制文件，如果目标文件已存在则创建新的副本
void copy()
{
    string Tempbuff;
    int addre;

    // 查找源文件的地址
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            addre = FileInfo[curID][i].addr;
            break;
        }
    }

    int index;
    // 查找源文件在打开文件表中的索引
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }

    // 循环读取源文件的内容
    while (1)
    {
        if (FileCluster[addre].next_num == addre)
        {
            for (int i = 0; i < FileState[curID][index].write_poit; i++)
            {
                Tempbuff += FileCluster[addre].data[i];
            }
            break;
        }
        else
        {
            for (int i = 0; i < 256; i++)
            {
                Tempbuff += FileCluster[addre].data[i];
            }
            if (FileCluster[addre].next_num != addre)
            {
                addre = FileCluster[addre].next_num;
            }
            else
                break;
        }
    }

    // 设置新的写操作命令
    cmd_in.cmd_num[0] = "write";
    cmd_in.cmd_num[1] = cmd_in.cmd_num[2];
    cmd_in.cmd_num[2] = Tempbuff;
    
    // 获取新内容的长度
    stringstream ss;
    ss << Tempbuff.length();
    string curtp;
    ss >> curtp;
    cmd_in.cmd_num[3] = curtp;

    // 执行写操作
    copy_write();
}

// 创建文件
void create();

// 复制文件命令
void copy_check()
{
    int address=-1;

    // 查找源文件的地址
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }
	if(address==-1){
		cout<<"源文件不存在！！！"<<endl;
		return ;
	}

    int flag = 0;
    // 检查目标文件是否已存在
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str()) == 0)
        {
            flag = 1;
            break;
        }
    }

    string s1, s2, s3;
    s1 = cmd_in.cmd_num[0];
    s2 = cmd_in.cmd_num[1];
    s3 = cmd_in.cmd_num[2];

    // 新的文件名存在
    if (flag)
    {
        copy();
    }
    else // 新的文件名不存在的话要新建一个
    {
        // Create name mode
        cmd_in.cmd_num[0] = "create";
        cmd_in.cmd_num[1] = s3;
        cmd_in.cmd_num[2] = "2";
        create();
        cmd_in.cmd_num[0] = s1;
        cmd_in.cmd_num[1] = s2;
        cmd_in.cmd_num[2] = s3;
        copy();
    }
}

// 列出当前用户的文件列表
void ls()
{
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        cout << FileInfo[curID][i].filename << "  ";
    }
    cout << endl;
}

// 查看文件内容
void cat()
{
    int address = -1;
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }

    if (address == -1)
    {
        cout << "文件名不存在！！！" << endl;
        return;
    }
	
	// 判断用户权限是否满足 
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        // 判断文件名是否匹配
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            if (FileInfo[curID][i].mode == 0 || FileInfo[curID][i].mode == 2)
            {
                break;
            }
            else
            {
                cout << "没有读的权限!" << endl;
                return;
            }
        }
    }
	
    int index = -1;
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "请先打开文件！！！" << endl;
        return;
    }
//
//	int temp=address;
//	while(temp!=FileCluster[temp].next_num){
//		cout<<temp<<" "<<FileCluster[temp].data<<endl;
//		temp = FileCluster[temp].next_num;
//	}
//	cout<<temp<<" "<<FileCluster[temp].data<<endl;
	
    while (1)
    {
        if (FileCluster[address].next_num == address)
        {
            for (int i = 0; i < FileState[curID][index].write_poit; i++)
                cout << FileCluster[address].data[i];
            break;
        }
        else
        {
            for (int i = 0; i < 256; i++)
            {
                cout << FileCluster[address].data[i];
            }
            if (FileCluster[address].next_num != address)
            {
                address = FileCluster[address].next_num;
            }
            else
                break;
        }
    }
    cout << endl;
}

// 修改密码
void chage_psw()
{
    // Passwd oldPwd newPwd
    // 检查旧密码是否正确
    if (strcmp(UsrInfo[curID].usrpass, cmd_in.cmd_num[1].c_str()) == 0)
    {
        // 更新密码为新密码
        strcpy(UsrInfo[curID].usrpass, cmd_in.cmd_num[2].c_str());
        cout << "密码修改成功" << endl;
    }
    else
    {
        cout << "密码有误" << endl;
    }
}

// 保存文件信息到磁盘
void save()
{
    out_to_file();
    cout << "写入到磁盘成功!" << endl;
}

// 用户登录
void login()
{
    // Login userName pwd
    int flag = 0;
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0 && strcmp(cmd_in.cmd_num[2].c_str(), UsrInfo[i].usrpass) == 0)
        {
            curID = i; // 全局记录登陆者的ID
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        cout << "您已经登陆成功！" << endl;
        return;
    }
    else
    {
        cout << "您的用户名或密码不正确,请重新登陆!" << endl;
    }
}

// 退出系统，保存文件信息到磁盘
void exit()
{
    save();
    cout << "成功退出!" << endl;
    exit(0);
}

// 注销当前用户，保存文件信息到磁盘，并将当前用户ID重置为0
void logout()
{
    save();
    curID = 0;
    cout << "登出成功!" << endl;
}


// 描述：用户注册函数，用于创建新用户。
void Register()
{
	if(cmd_in.cmd_num[1].length()==0 || cmd_in.cmd_num[2].length()==0){
		cout<<"用户名或密码不能为空，请重新注册！！！"<<endl;
		return ;
	}
	//Register usrName pwd
	if (UsrInfo.size() < 16)
	{
		int flag = 1;
		for (int i = 0; i < UsrInfo.size(); i++)
		{
			if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			strcpy(UsrInput.usrname, cmd_in.cmd_num[1].c_str());
			strcpy(UsrInput.usrpass, cmd_in.cmd_num[2].c_str());
			UsrInput.link = UsrInfo.size();
			UsrInfo.push_back(UsrInput);

			//为新的用户开辟空间
			vector<UFD> t;
			FileInfo.push_back(t);

			vector<UOF> temp;
		    FileState.push_back(temp);

			cout << "您的用户名创建成功!" << endl;

		}
		else
		{
			cout << "用户名已经存在，请重新注册" << endl;
		}
	}
	else
		cout << "总人数已满，无法注册" << endl;
}

void create()
{
    // 创建文件 create name mode
    if (FileInfo[curID].size() < 16)
    {
        int flag = 1;
        for (int i = 0; i < FileInfo[curID].size(); i++)
        {
            if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                flag = 0;
                break;
            }
        }
        
        if (!flag)
        {
            cout << "不能建立同名文件！" << endl;
            return;
        }

        // 初始化文件信息
        strcpy(FileInput.filename, cmd_in.cmd_num[1].c_str());
        stringstream ss;
        ss << cmd_in.cmd_num[2];
        int temp;
        ss >> temp;
        FileInput.mode = temp;
        FileInput.length = 0;

        // 存储文件内容的物理块处理
        for (int i = 0; i < FileCluster.size(); i++)
        {
            if (FileCluster[i].is_data == 0) // 盘中没有数据
            {
                FileInput.addr = i;
                FileCluster[i].is_data = 1;
                break;
            }
        }

        // 将文件信息添加到用户文件目录
        FileInfo[curID].push_back(FileInput);
        cout << "文件创建成功" << endl;

        // 文件状态处理
        strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
        StateInput.state = 0;
        StateInput.mode = FileInput.mode;
        StateInput.write_poit = 0;

        // 将文件状态信息添加到用户文件状态目录
        FileState[curID].push_back(StateInput);
    }
    else
    {
        cout << "超过文件上限！" << endl;
    }
}

void create_temp()
{
    // 创建文件 create name mode
    if (FileInfo[curID].size() < 16)
    {
        int flag = 1;
        for (int i = 0; i < FileInfo[curID].size(); i++)
        {
            if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                flag = 0;
                break;
            }
        }
        
        if (!flag)
        {
            cout << "不能建立同名文件！" << endl;
            return;
        }

        // 初始化文件信息
        strcpy(FileInput.filename, cmd_in.cmd_num[1].c_str());
        stringstream ss;
        ss << cmd_in.cmd_num[2];
        int temp;
        ss >> temp;
        FileInput.mode = temp;
        FileInput.length = 0;

        // 存储文件内容的物理块处理
        for (int i = 0; i < FileCluster.size(); i++)
        {
            if (FileCluster[i].is_data == 0) // 盘中没有数据
            {
                FileInput.addr = i;
                FileCluster[i].is_data = 1;
                break;
            }
        }

        // 将文件信息添加到用户文件目录
        FileInfo[curID].push_back(FileInput);

        // 文件状态处理
        strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
        StateInput.state = 0;
        StateInput.mode = FileInput.mode;
        StateInput.write_poit = 0;

        // 将文件状态信息添加到用户文件状态目录
        FileState[curID].push_back(StateInput);
    }
    else
    {
        cout << "超过文件上限！" << endl;
    }
}

// 删除文件
void rd_temp()
{
    int flag = 0;
    vector<UFD>::iterator it;

    // 查找文件是否存在
    for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
    {
        if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            flag = 1;
            break;
        }
    }

    if (flag)
    {
        int address;
        for (int i = 0; i < FileInfo[curID].size(); i++)
        {
            if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                address = FileInfo[curID][i].addr;
                break;
            }
        }
        int index;
        for (int i = 0; i < FileState[curID].size(); i++)
        {
            if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                index = i;
                break;
            }
        }

        vector<int> reset_addr;
        reset_addr.clear();

        // 清除文件内容
        while (1)
        {
            if (FileCluster[address].next_num == address)
            {
                for (int i = 0; i < FileState[curID][index].write_poit; i++)
                    FileCluster[address].data[i] = 0;
                reset_addr.push_back(address);
                break;
            }
            else
            {
                reset_addr.push_back(address);
                for (int i = 0; i < 256; i++)
                {
                    FileCluster[address].data[i] = 0;
                }
                if (FileCluster[address].next_num != address)
                {
                    address = FileCluster[address].next_num;
                }
                else
                    break;
            }
        }

        // 重置簇状态
        for (int w = 0; w < reset_addr.size(); w++)
        {
            FileCluster[reset_addr[w]].is_data = 0;
            FileCluster[reset_addr[w]].next_num = reset_addr[w];
        }

        // 清除文件信息
        for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
        {
            if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileInfo[curID].erase(it);
                break;
            }
        }

        // 清除状态栏信息
        vector<UOF>::iterator it1;
        for (it1 = FileState[curID].begin(); it1 != FileState[curID].end(); it1++)
        {
            if (strcmp((*it1).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileState[curID].erase(it1);
                break;
            }
        }
    }
    else
        cout << "文件不存在!" << endl;
}

// 删除文件
void rd()
{
    int flag = 0;
    vector<UFD>::iterator it;

    // 查找文件是否存在
    for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
    {
        if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            flag = 1;
            break;
        }
    }

    if (flag)
    {
        int address;
        for (int i = 0; i < FileInfo[curID].size(); i++)
        {
            if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                address = FileInfo[curID][i].addr;
                break;
            }
        }
        int index;
        for (int i = 0; i < FileState[curID].size(); i++)
        {
            if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                index = i;
                break;
            }
        }

        vector<int> reset_addr;
        reset_addr.clear();

        // 清除文件内容
        while (1)
        {
            if (FileCluster[address].next_num == address)
            {
                for (int i = 0; i < FileState[curID][index].write_poit; i++)
                    FileCluster[address].data[i] = 0;
                reset_addr.push_back(address);
                break;
            }
            else
            {
                reset_addr.push_back(address);
                for (int i = 0; i < 256; i++)
                {
                    FileCluster[address].data[i] = 0;
                }
                if (FileCluster[address].next_num != address)
                {
                    address = FileCluster[address].next_num;
                }
                else
                    break;
            }
        }

        // 重置簇状态
        for (int w = 0; w < reset_addr.size(); w++)
        {
            FileCluster[reset_addr[w]].is_data = 0;
            FileCluster[reset_addr[w]].next_num = reset_addr[w];
        }

        // 清除文件信息
        for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
        {
            if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileInfo[curID].erase(it);
                break;
            }
        }

        // 清除状态栏信息
        vector<UOF>::iterator it1;
        for (it1 = FileState[curID].begin(); it1 != FileState[curID].end(); it1++)
        {
            if (strcmp((*it1).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileState[curID].erase(it1);
                break;
            }
        }

        cout << "删除成功!" << endl;
    }
    else
        cout << "文件不存在!" << endl;
}

// 写文件-副本，为覆盖写而生 物理块号：34#-100#，共计67块，每块大小256B 
void write_temp()
{
    // Write filename -m nbytes 
	
    // 定义缓冲区和用于读取字节数的变量
    const char* buf;
    stringstream ss;
    ss << cmd_in.cmd_num[3];
    int temp;
    ss >> temp;

    int index=-1,state;
    // 遍历当前用户的文件状态信息，查找文件在状态表中的索引id 
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }
    
   	if(index==-1){
		cout<<"文件名不存在，请重新输入命令！！！"<<endl;
		return ;
	} 
	

	
    // 获取文件的起始物理块地址
    int address;
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }
    
    string s;
	cout << "请输入待写入的内容：" ;
    getline(cin,s);  // 获取用户输入的待写入内容
	
	if(s.length()>1024){
		cout<<"输入内容超出1024个字节，无法输入！！！"<<endl;
		return ;
	}
		
	buf = s.c_str();
	cout<<buf<<endl;
	
    // 获取文件状态表中的写指针位置
    int wbegin;
    wbegin = FileState[curID][index].write_poit;
	
    // 找到写指针所在的最后一个磁盘块
    while (FileCluster[address].next_num != address)
        address = FileCluster[address].next_num;

    vector<int> newspace_num;  // 计算将要占用的物理块的数量
    newspace_num.clear();
	
	temp = temp>strlen(buf) ? strlen(buf) : temp;
	
    // 计算需要占用的新物理块的数量
    int num;
    if (temp <= 256 - wbegin)
        num = 0;
    else
    {
        num = ceil((temp - (256 - wbegin)) * 1.0 / 256);
    }

    newspace_num.push_back(address);

    // 遍历文件簇，找到并占用新的物理块
    for (int i = 0; i < FileCluster.size(); i++)
    {
        if (newspace_num.size() == num + 1)
            break;
        if (FileCluster[i].is_data == 0)
        {
            newspace_num.push_back(i);
            FileCluster[i].is_data = 1;
        }
    }

    // 将新的物理块链接起来
    for (int k = 0; k < newspace_num.size() - 1; k++)
    {
        FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
    }

    // 将数据写入物理块
    for (int i = 0; i < temp; i++)
    {
        if (wbegin == 256)
        {
            wbegin = 0;
            address = FileCluster[address].next_num;
        }
        FileCluster[address].data[wbegin] = buf[i];
        if(buf[i]=='\0'){
        	
		}
        wbegin++;
    }

    // 更新写指针
    FileState[curID][index].write_poit = wbegin;
    cout << "缓冲区写入成功!" << endl;
    return;
}

// 写文件 物理块号：34#-100#，共计67块，每块大小256B 
void write()
{
    // Write filename -m nbytes 
	
    // 定义缓冲区和用于读取字节数的变量
    const char* buf;

    stringstream ss;
    ss << cmd_in.cmd_num[3];
    int temp,mode;
    ss >> temp;

    // 判断用户权限是否满足 
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        // 判断文件名是否匹配
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)
            {
            	mode = FileInfo[curID][i].mode;
                break;
            }
            else
            {
                cout << "没有写的权限!" << endl;
                return;
            }
        }
    }
    

    int index=-1,state;
    // 遍历当前用户的文件状态信息，查找文件在状态表中的索引id 
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }
    
   	if(index==-1){
		cout<<"文件名不存在，请重新输入命令！！！"<<endl;
		return ;
	} 
	
    cout<<"请选择 覆盖写<1> 或 追加写<2>：";
    int ch;cin>>ch;cin.get();
    string s1,s2,s3,s4;
	if(ch==1){
    	s1 = cmd_in.cmd_num[0];
    	s2 = cmd_in.cmd_num[1];
    	s3 = cmd_in.cmd_num[2];
    	s4 = cmd_in.cmd_num[3];
    	
    	for(int i=0;i<4;i++)
    		cmd_in.cmd_num[i]="";

    	cmd_in.cmd_num[0] = "rd";
    	cmd_in.cmd_num[1] = s2;
    	rd_temp();
    	
    	cmd_in.cmd_num[0] = "create";
    	cmd_in.cmd_num[1] = s2;
    	cmd_in.cmd_num[2] = to_string(mode);
    	create_temp();
    	
    	cmd_in.cmd_num[0] = s1;
    	cmd_in.cmd_num[1] = s2;
    	cmd_in.cmd_num[2] = s3;
    	cmd_in.cmd_num[3] = s4;
    	write_temp();
    	return ;
	}
	
    // 获取文件的起始物理块地址
    int address;
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }

    string s;
	cout << "请输入待写入的内容：" ;
    getline(cin,s);  // 获取用户输入的待写入内容
	
	if(s.length()>1024){
		cout<<"输入内容超出1024个字节，无法输入！！！"<<endl;
		return ;
	}
		
	buf = s.c_str();
	cout<<buf<<endl;
	
    // 获取文件状态表中的写指针位置
    int wbegin;
    wbegin = FileState[curID][index].write_poit;

    // 找到写指针所在的最后一个磁盘块
    while (FileCluster[address].next_num != address)
        address = FileCluster[address].next_num;

    vector<int> newspace_num;  // 计算将要占用的物理块的数量
    newspace_num.clear();
	
	temp = temp>strlen(buf) ? strlen(buf) : temp;
	
    // 计算需要占用的新物理块的数量
    int num;
    if (temp <= 256 - wbegin)
        num = 0;
    else
    {
        num = ceil((temp - (256 - wbegin)) * 1.0 / 256);
    }

    newspace_num.push_back(address);

    // 遍历文件簇，找到并占用新的物理块
    for (int i = 0; i < FileCluster.size(); i++)
    {
        if (newspace_num.size() == num + 1)
            break;
        if (FileCluster[i].is_data == 0)
        {
            newspace_num.push_back(i);
            FileCluster[i].is_data = 1;
        }
    }

    // 将新的物理块链接起来
    for (int k = 0; k < newspace_num.size() - 1; k++)
    {
        FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
    }

    // 将数据写入物理块
    for (int i = 0; i < temp; i++)
    {
        if (wbegin == 256)
        {
            wbegin = 0;
            address = FileCluster[address].next_num;
        }
        FileCluster[address].data[wbegin] = buf[i];

        wbegin++;
    }

    // 更新写指针
    FileState[curID][index].write_poit = wbegin;
    cout << "缓冲区写入成功!" << endl;
    return;
}


void help()
{
	cout << "\t\t\t***********************************************************************" << endl;
	cout << "\t\t\t*     exit                                     退出程序               *" << endl;
	cout << "\t\t\t*     save                                     同步到磁盘             *" << endl;
	cout << "\t\t\t*     cls                                      清屏                   *" << endl;
	cout << "\t\t\t*     logout	                               登出                   *" << endl;
	cout << "\t\t\t*     ls                                       列出所有文件           *" << endl;
	cout << "\t\t\t*     rd  	filename                       删除文件               *" << endl;
	cout << "\t\t\t*     cat       filename                       显示文件内容           *" << endl;
	cout << "\t\t\t*     login	userName   pwd	               登录                   *" << endl;
	cout << "\t\t\t*     register  usrName    pwd                 注册                   *" << endl;
	cout << "\t\t\t*     passwd	oldPwd     newPwd              修改密码               *" << endl;
	cout << "\t\t\t*     create	filename   mode	               创建文件               *" << endl;
	cout << "\t\t\t*     chmod	filename   mode  	       改变文件属性           *" << endl;
	cout << "\t\t\t*     rename	srcFile    desFile             改变文件名             *" << endl;
	cout << "\t\t\t*     cp        srcFile    desFile             文件拷贝               *" << endl;
	cout << "\t\t\t*     write	filename   -m   nbytes         写文件                 *" << endl;
	cout << "\t\t\t***********************************************************************" << endl;
}

// 复制写操作
void copy_write()
{
    char buf[1024*1024];
    stringstream ss;
    ss << cmd_in.cmd_num[3];
    int temp;
    ss >> temp;

    // 检索文件
    int index;
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }

	cout<<"请选择 覆盖Copy<1> 或 追加Copy<2>：";
    int ch;cin>>ch;
    string s1,s2,s3,s4;
	if(ch==1){
    	s1 = cmd_in.cmd_num[0];
    	s2 = cmd_in.cmd_num[1];
    	s3 = cmd_in.cmd_num[2];
    	s4 = cmd_in.cmd_num[3];
    	
    	for(int i=0;i<4;i++)
    		cmd_in.cmd_num[i]="";

    	cmd_in.cmd_num[0] = "rd";
    	cmd_in.cmd_num[1] = s2;
    	rd_temp();
    	
    	cmd_in.cmd_num[0] = "create";
    	cmd_in.cmd_num[1] = s2;
    	cmd_in.cmd_num[2] = "2";
    	create_temp();
    	
    	cmd_in.cmd_num[0] = s1;
    	cmd_in.cmd_num[1] = s2;
    	cmd_in.cmd_num[2] = s3;
    	cmd_in.cmd_num[3] = s4;
	}
	
    // 起始物理块
    int address;
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }

    // 复制字符串
    strcpy(buf, cmd_in.cmd_num[2].c_str());

    int wbegin;
    wbegin = FileState[curID][index].write_poit;

    // 找到写指针所在的最后一个磁盘
    while (FileCluster[address].next_num != address)
        address = FileCluster[address].next_num;

    vector<int> newspace_num; // 计算将要占用的物理块的数量
    newspace_num.clear();

    if (temp <= 256 - wbegin)
        num = 0;
    else
    {
        num = ceil((temp - (256 - wbegin)) * 1.0 / 256);
    }

    newspace_num.push_back(address);

    for (int i = 0; i < FileCluster.size(); i++)
    {
        if (newspace_num.size() == num + 1)
            break;
        if (FileCluster[i].is_data == 0)
        {
            newspace_num.push_back(i);
            FileCluster[i].is_data = 1;
        }
    }

    for (int k = 0; k < newspace_num.size() - 1; k++)
    {
        FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
    }
    for (int i = 0; i < temp; i++)
    {
        if (wbegin == 256)
        {
            wbegin = 0;
            address = FileCluster[address].next_num;
        }
        FileCluster[address].data[wbegin] = buf[i];
        wbegin++;
    }

    // 更新写指针
    FileState[curID][index].write_poit = wbegin;
    cout << "缓冲区写入成功!" << endl;
    return;
}

// 清空命令行界面
void clear()
{
    system("cls");
}


#endif
