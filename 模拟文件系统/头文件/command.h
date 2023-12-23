#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "common.h"
#include "MFD.h"
#include "Fileoperate.h"
#include<string.h>

// �����ⲿȫ�ֱ��������ڴ洢�ļ���Ϣ���û���Ϣ���ļ�״̬
extern vector<vector<UFD>> FileInfo;    // ��ά�����洢�û��ļ�Ŀ¼��Ϣ
extern UFD FileInput;                   // ���������ļ���Ϣ�Ľṹ�����
extern vector<Cluster> FileCluster;     // �洢�ļ�����Ϣ
extern vector<MFD> UsrInfo;             // �洢���ļ�Ŀ¼��Ϣ
extern vector<vector<UOF>> FileState;   // ��ά�����洢�û��ļ�״̬��Ϣ
extern UOF StateInput;                   // ���������ļ�״̬��Ϣ�Ľṹ�����
extern Cluster ClusterInput;            // ���������ļ�����Ϣ�Ľṹ�����


// �޸��ļ�Ȩ��
void chmod()
{
    int temp,index=-1;

    // �����ļ���Ϣ���ҵ�Ŀ���ļ����޸�Ȩ��
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
		cout<<"�ļ��������ڣ�����"<<endl;
		return ;
	}

    // �������ļ�״̬���ҵ�Ŀ���ļ����޸�Ȩ��
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            FileState[curID][i].mode = temp;
            break;
        }
    }

    cout << "�޸�Ȩ�޳ɹ�!" << endl;
}

// �������ļ�
void rename()
{
	if(cmd_in.cmd_num[2]==cmd_in.cmd_num[1]){
		cout<<"���ļ���Ӧ�����ļ�����ͬ������"<<endl;
		return ;
	}
	
    for(int i=0;i<FileInfo[curID].size();i++){
    	if(strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str())==0){
    		cout<<"���ļ����������ļ����ظ������������ã�����"<<endl;
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
		cout<<"���ļ��������ڣ�����"<<endl;
		return ;
	}
	else{
		
		// �޸��ļ���
            strcpy(FileInfo[curID][index].filename, cmd_in.cmd_num[2].c_str());
            
		// �޸Ĵ��ļ�״̬���е��ļ���
	    for (int i = 0; i < FileState[curID].size(); i++)
	    {
	        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
	        {
	            strcpy(FileState[curID][i].filename, cmd_in.cmd_num[2].c_str());
	            break;
	        }
	    }
	
	    cout << "�������ļ��ɹ�" << endl;
	}
    
}


// ��ʱд��������
void copy_write();

// �����ļ������Ŀ���ļ��Ѵ����򴴽��µĸ���
void copy()
{
    string Tempbuff;
    int addre;

    // ����Դ�ļ��ĵ�ַ
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            addre = FileInfo[curID][i].addr;
            break;
        }
    }

    int index;
    // ����Դ�ļ��ڴ��ļ����е�����
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }

    // ѭ����ȡԴ�ļ�������
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

    // �����µ�д��������
    cmd_in.cmd_num[0] = "write";
    cmd_in.cmd_num[1] = cmd_in.cmd_num[2];
    cmd_in.cmd_num[2] = Tempbuff;
    
    // ��ȡ�����ݵĳ���
    stringstream ss;
    ss << Tempbuff.length();
    string curtp;
    ss >> curtp;
    cmd_in.cmd_num[3] = curtp;

    // ִ��д����
    copy_write();
}

// �����ļ�
void create();

// �����ļ�����
void copy_check()
{
    int address=-1;

    // ����Դ�ļ��ĵ�ַ
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }
	if(address==-1){
		cout<<"Դ�ļ������ڣ�����"<<endl;
		return ;
	}

    int flag = 0;
    // ���Ŀ���ļ��Ƿ��Ѵ���
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

    // �µ��ļ�������
    if (flag)
    {
        copy();
    }
    else // �µ��ļ��������ڵĻ�Ҫ�½�һ��
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

// �г���ǰ�û����ļ��б�
void ls()
{
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        cout << FileInfo[curID][i].filename << "  ";
    }
    cout << endl;
}

// �鿴�ļ�����
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
        cout << "�ļ��������ڣ�����" << endl;
        return;
    }
	
	// �ж��û�Ȩ���Ƿ����� 
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        // �ж��ļ����Ƿ�ƥ��
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            if (FileInfo[curID][i].mode == 0 || FileInfo[curID][i].mode == 2)
            {
                break;
            }
            else
            {
                cout << "û�ж���Ȩ��!" << endl;
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
        cout << "���ȴ��ļ�������" << endl;
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

// �޸�����
void chage_psw()
{
    // Passwd oldPwd newPwd
    // ���������Ƿ���ȷ
    if (strcmp(UsrInfo[curID].usrpass, cmd_in.cmd_num[1].c_str()) == 0)
    {
        // ��������Ϊ������
        strcpy(UsrInfo[curID].usrpass, cmd_in.cmd_num[2].c_str());
        cout << "�����޸ĳɹ�" << endl;
    }
    else
    {
        cout << "��������" << endl;
    }
}

// �����ļ���Ϣ������
void save()
{
    out_to_file();
    cout << "д�뵽���̳ɹ�!" << endl;
}

// �û���¼
void login()
{
    // Login userName pwd
    int flag = 0;
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0 && strcmp(cmd_in.cmd_num[2].c_str(), UsrInfo[i].usrpass) == 0)
        {
            curID = i; // ȫ�ּ�¼��½�ߵ�ID
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        cout << "���Ѿ���½�ɹ���" << endl;
        return;
    }
    else
    {
        cout << "�����û��������벻��ȷ,�����µ�½!" << endl;
    }
}

// �˳�ϵͳ�������ļ���Ϣ������
void exit()
{
    save();
    cout << "�ɹ��˳�!" << endl;
    exit(0);
}

// ע����ǰ�û��������ļ���Ϣ�����̣�������ǰ�û�ID����Ϊ0
void logout()
{
    save();
    curID = 0;
    cout << "�ǳ��ɹ�!" << endl;
}


// �������û�ע�ắ�������ڴ������û���
void Register()
{
	if(cmd_in.cmd_num[1].length()==0 || cmd_in.cmd_num[2].length()==0){
		cout<<"�û��������벻��Ϊ�գ�������ע�ᣡ����"<<endl;
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

			//Ϊ�µ��û����ٿռ�
			vector<UFD> t;
			FileInfo.push_back(t);

			vector<UOF> temp;
		    FileState.push_back(temp);

			cout << "�����û��������ɹ�!" << endl;

		}
		else
		{
			cout << "�û����Ѿ����ڣ�������ע��" << endl;
		}
	}
	else
		cout << "�������������޷�ע��" << endl;
}

void create()
{
    // �����ļ� create name mode
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
            cout << "���ܽ���ͬ���ļ���" << endl;
            return;
        }

        // ��ʼ���ļ���Ϣ
        strcpy(FileInput.filename, cmd_in.cmd_num[1].c_str());
        stringstream ss;
        ss << cmd_in.cmd_num[2];
        int temp;
        ss >> temp;
        FileInput.mode = temp;
        FileInput.length = 0;

        // �洢�ļ����ݵ�����鴦��
        for (int i = 0; i < FileCluster.size(); i++)
        {
            if (FileCluster[i].is_data == 0) // ����û������
            {
                FileInput.addr = i;
                FileCluster[i].is_data = 1;
                break;
            }
        }

        // ���ļ���Ϣ��ӵ��û��ļ�Ŀ¼
        FileInfo[curID].push_back(FileInput);
        cout << "�ļ������ɹ�" << endl;

        // �ļ�״̬����
        strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
        StateInput.state = 0;
        StateInput.mode = FileInput.mode;
        StateInput.write_poit = 0;

        // ���ļ�״̬��Ϣ��ӵ��û��ļ�״̬Ŀ¼
        FileState[curID].push_back(StateInput);
    }
    else
    {
        cout << "�����ļ����ޣ�" << endl;
    }
}

void create_temp()
{
    // �����ļ� create name mode
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
            cout << "���ܽ���ͬ���ļ���" << endl;
            return;
        }

        // ��ʼ���ļ���Ϣ
        strcpy(FileInput.filename, cmd_in.cmd_num[1].c_str());
        stringstream ss;
        ss << cmd_in.cmd_num[2];
        int temp;
        ss >> temp;
        FileInput.mode = temp;
        FileInput.length = 0;

        // �洢�ļ����ݵ�����鴦��
        for (int i = 0; i < FileCluster.size(); i++)
        {
            if (FileCluster[i].is_data == 0) // ����û������
            {
                FileInput.addr = i;
                FileCluster[i].is_data = 1;
                break;
            }
        }

        // ���ļ���Ϣ��ӵ��û��ļ�Ŀ¼
        FileInfo[curID].push_back(FileInput);

        // �ļ�״̬����
        strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
        StateInput.state = 0;
        StateInput.mode = FileInput.mode;
        StateInput.write_poit = 0;

        // ���ļ�״̬��Ϣ��ӵ��û��ļ�״̬Ŀ¼
        FileState[curID].push_back(StateInput);
    }
    else
    {
        cout << "�����ļ����ޣ�" << endl;
    }
}

// ɾ���ļ�
void rd_temp()
{
    int flag = 0;
    vector<UFD>::iterator it;

    // �����ļ��Ƿ����
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

        // ����ļ�����
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

        // ���ô�״̬
        for (int w = 0; w < reset_addr.size(); w++)
        {
            FileCluster[reset_addr[w]].is_data = 0;
            FileCluster[reset_addr[w]].next_num = reset_addr[w];
        }

        // ����ļ���Ϣ
        for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
        {
            if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileInfo[curID].erase(it);
                break;
            }
        }

        // ���״̬����Ϣ
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
        cout << "�ļ�������!" << endl;
}

// ɾ���ļ�
void rd()
{
    int flag = 0;
    vector<UFD>::iterator it;

    // �����ļ��Ƿ����
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

        // ����ļ�����
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

        // ���ô�״̬
        for (int w = 0; w < reset_addr.size(); w++)
        {
            FileCluster[reset_addr[w]].is_data = 0;
            FileCluster[reset_addr[w]].next_num = reset_addr[w];
        }

        // ����ļ���Ϣ
        for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
        {
            if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileInfo[curID].erase(it);
                break;
            }
        }

        // ���״̬����Ϣ
        vector<UOF>::iterator it1;
        for (it1 = FileState[curID].begin(); it1 != FileState[curID].end(); it1++)
        {
            if (strcmp((*it1).filename, cmd_in.cmd_num[1].c_str()) == 0)
            {
                FileState[curID].erase(it1);
                break;
            }
        }

        cout << "ɾ���ɹ�!" << endl;
    }
    else
        cout << "�ļ�������!" << endl;
}

// д�ļ�-������Ϊ����д���� �����ţ�34#-100#������67�飬ÿ���С256B 
void write_temp()
{
    // Write filename -m nbytes 
	
    // ���建���������ڶ�ȡ�ֽ����ı���
    const char* buf;
    stringstream ss;
    ss << cmd_in.cmd_num[3];
    int temp;
    ss >> temp;

    int index=-1,state;
    // ������ǰ�û����ļ�״̬��Ϣ�������ļ���״̬���е�����id 
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }
    
   	if(index==-1){
		cout<<"�ļ��������ڣ������������������"<<endl;
		return ;
	} 
	

	
    // ��ȡ�ļ�����ʼ������ַ
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
	cout << "�������д������ݣ�" ;
    getline(cin,s);  // ��ȡ�û�����Ĵ�д������
	
	if(s.length()>1024){
		cout<<"�������ݳ���1024���ֽڣ��޷����룡����"<<endl;
		return ;
	}
		
	buf = s.c_str();
	cout<<buf<<endl;
	
    // ��ȡ�ļ�״̬���е�дָ��λ��
    int wbegin;
    wbegin = FileState[curID][index].write_poit;
	
    // �ҵ�дָ�����ڵ����һ�����̿�
    while (FileCluster[address].next_num != address)
        address = FileCluster[address].next_num;

    vector<int> newspace_num;  // ���㽫Ҫռ�õ�����������
    newspace_num.clear();
	
	temp = temp>strlen(buf) ? strlen(buf) : temp;
	
    // ������Ҫռ�õ�������������
    int num;
    if (temp <= 256 - wbegin)
        num = 0;
    else
    {
        num = ceil((temp - (256 - wbegin)) * 1.0 / 256);
    }

    newspace_num.push_back(address);

    // �����ļ��أ��ҵ���ռ���µ������
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

    // ���µ��������������
    for (int k = 0; k < newspace_num.size() - 1; k++)
    {
        FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
    }

    // ������д�������
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

    // ����дָ��
    FileState[curID][index].write_poit = wbegin;
    cout << "������д��ɹ�!" << endl;
    return;
}

// д�ļ� �����ţ�34#-100#������67�飬ÿ���С256B 
void write()
{
    // Write filename -m nbytes 
	
    // ���建���������ڶ�ȡ�ֽ����ı���
    const char* buf;

    stringstream ss;
    ss << cmd_in.cmd_num[3];
    int temp,mode;
    ss >> temp;

    // �ж��û�Ȩ���Ƿ����� 
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        // �ж��ļ����Ƿ�ƥ��
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)
            {
            	mode = FileInfo[curID][i].mode;
                break;
            }
            else
            {
                cout << "û��д��Ȩ��!" << endl;
                return;
            }
        }
    }
    

    int index=-1,state;
    // ������ǰ�û����ļ�״̬��Ϣ�������ļ���״̬���е�����id 
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }
    
   	if(index==-1){
		cout<<"�ļ��������ڣ������������������"<<endl;
		return ;
	} 
	
    cout<<"��ѡ�� ����д<1> �� ׷��д<2>��";
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
	
    // ��ȡ�ļ�����ʼ������ַ
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
	cout << "�������д������ݣ�" ;
    getline(cin,s);  // ��ȡ�û�����Ĵ�д������
	
	if(s.length()>1024){
		cout<<"�������ݳ���1024���ֽڣ��޷����룡����"<<endl;
		return ;
	}
		
	buf = s.c_str();
	cout<<buf<<endl;
	
    // ��ȡ�ļ�״̬���е�дָ��λ��
    int wbegin;
    wbegin = FileState[curID][index].write_poit;

    // �ҵ�дָ�����ڵ����һ�����̿�
    while (FileCluster[address].next_num != address)
        address = FileCluster[address].next_num;

    vector<int> newspace_num;  // ���㽫Ҫռ�õ�����������
    newspace_num.clear();
	
	temp = temp>strlen(buf) ? strlen(buf) : temp;
	
    // ������Ҫռ�õ�������������
    int num;
    if (temp <= 256 - wbegin)
        num = 0;
    else
    {
        num = ceil((temp - (256 - wbegin)) * 1.0 / 256);
    }

    newspace_num.push_back(address);

    // �����ļ��أ��ҵ���ռ���µ������
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

    // ���µ��������������
    for (int k = 0; k < newspace_num.size() - 1; k++)
    {
        FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
    }

    // ������д�������
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

    // ����дָ��
    FileState[curID][index].write_poit = wbegin;
    cout << "������д��ɹ�!" << endl;
    return;
}


void help()
{
	cout << "\t\t\t***********************************************************************" << endl;
	cout << "\t\t\t*     exit                                     �˳�����               *" << endl;
	cout << "\t\t\t*     save                                     ͬ��������             *" << endl;
	cout << "\t\t\t*     cls                                      ����                   *" << endl;
	cout << "\t\t\t*     logout	                               �ǳ�                   *" << endl;
	cout << "\t\t\t*     ls                                       �г������ļ�           *" << endl;
	cout << "\t\t\t*     rd  	filename                       ɾ���ļ�               *" << endl;
	cout << "\t\t\t*     cat       filename                       ��ʾ�ļ�����           *" << endl;
	cout << "\t\t\t*     login	userName   pwd	               ��¼                   *" << endl;
	cout << "\t\t\t*     register  usrName    pwd                 ע��                   *" << endl;
	cout << "\t\t\t*     passwd	oldPwd     newPwd              �޸�����               *" << endl;
	cout << "\t\t\t*     create	filename   mode	               �����ļ�               *" << endl;
	cout << "\t\t\t*     chmod	filename   mode  	       �ı��ļ�����           *" << endl;
	cout << "\t\t\t*     rename	srcFile    desFile             �ı��ļ���             *" << endl;
	cout << "\t\t\t*     cp        srcFile    desFile             �ļ�����               *" << endl;
	cout << "\t\t\t*     write	filename   -m   nbytes         д�ļ�                 *" << endl;
	cout << "\t\t\t***********************************************************************" << endl;
}

// ����д����
void copy_write()
{
    char buf[1024*1024];
    stringstream ss;
    ss << cmd_in.cmd_num[3];
    int temp;
    ss >> temp;

    // �����ļ�
    int index;
    for (int i = 0; i < FileState[curID].size(); i++)
    {
        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            index = i;
            break;
        }
    }

	cout<<"��ѡ�� ����Copy<1> �� ׷��Copy<2>��";
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
	
    // ��ʼ�����
    int address;
    for (int i = 0; i < FileInfo[curID].size(); i++)
    {
        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
        {
            address = FileInfo[curID][i].addr;
            break;
        }
    }

    // �����ַ���
    strcpy(buf, cmd_in.cmd_num[2].c_str());

    int wbegin;
    wbegin = FileState[curID][index].write_poit;

    // �ҵ�дָ�����ڵ����һ������
    while (FileCluster[address].next_num != address)
        address = FileCluster[address].next_num;

    vector<int> newspace_num; // ���㽫Ҫռ�õ�����������
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

    // ����дָ��
    FileState[curID][index].write_poit = wbegin;
    cout << "������д��ɹ�!" << endl;
    return;
}

// ��������н���
void clear()
{
    system("cls");
}


#endif
