#ifndef _MFD_H_
#define _MFD_H_

#include"common.h"
#include"command.h"
using namespace std;
/*
	MFD.h ���ڶ������ݽṹ�����ָ��¼�빦��
*/


// 32���ֽ�
// MFD �ṹ�嶨�壬��ʾ���ļ�Ŀ¼��
typedef struct mfd
{
    char usrname[14];  // �û�����14���ֽ� 
    char usrpass[14];  // �û����룬14���ֽ� 
    int link;          // ���û���UFD���ڵ��̺�	4���ֽ� 
} MFD;

// UFD �ṹ�嶨�壬��ʾ�û��ļ�Ŀ¼��
typedef struct ufd
{
    char filename[20];  // �ļ�����20���ֽ� 
    int mode;           // �ļ�Ȩ�ޣ�0 - readonly;  1 - writeonly;  2 - read / write  4���ֽ� 
    int length;         // �ļ����ȣ��ֽ�����
    int addr;           // �ļ���ʼ������ַ 4���ֽ� 
} UFD;

// UOF �ṹ�嶨�壬��ʾ�û����ļ���
typedef struct uof
{
    char filename[20];  // �ļ�����20���ֽ� 
    int mode;           // �ļ�Ȩ��
    int state;          // �ļ�״̬��0δ��, 1��
    int write_poit;     // дָ��λ��
} UOF;

// Cluster �ṹ�嶨�壬��ʾ�����ϵ�һ�������
typedef struct cluster
{
    int next_num;       // ��һ�������ı��
    int is_data;        // ��ʶ�Ƿ�Ϊ���ݿ�
    char data[257];     // �洢���ݵ����飬��СΪ256�ֽ�
} Cluster;

// func ���Ͷ��壬��ʾ����ָ������
typedef void(*func)(void);

// HAND_TO �ṹ�嶨�壬��ʾ�����������Ϣ
typedef struct hand
{
    char *pname;        // ������
    func handler;       // ����ָ��
} Hand_to;

// COMMAND �ṹ�嶨�壬��ʾ������Ϣ
typedef struct cmd
{
    string cmd_num[4];  // ������ĸ�����

} Command;


// ȫ�ֱ�������ʾ��ǰ�������Ϣ
Command cmd_in;

// �ⲿ��������ʾ��������б� <---- main.cpp
extern Hand_to handlerlist[];

// �������壬���ڽ������������
void cmd_in_parse()
{
    string s;
    // �ӱ�׼�����ȡһ������
    getline(cin, s);
    
    for(int i=0; i < 4 ; i++)
    	cmd_in.cmd_num[i]="";
	
	// ���û���������ִ�� 
	if(s.length()==0){
		cout<<endl;
		return ;
	}
    // ʹ���ַ�������������
    stringstream stream(s);
    int i = 0;

    // ��������� cmd_in ��
    while (stream >> cmd_in.cmd_num[i])
        i++;

    // ���������б�
    int flag = 0;
    for (int i = 0; handlerlist[i].pname != NULL; i++)
    {
        // �ж������Ƿ��������б���
        if (strcmp(handlerlist[i].pname, cmd_in.cmd_num[0].c_str()) == 0)
        {
            flag = 1;
            // ������Ӧ���������
            handlerlist[i].handler();
            break;
        }
    }

    // ���δ�ҵ��������ʾ
    if (!flag)
        cout << "������Ч!!!" << endl;
}



#endif _MFD_H_
