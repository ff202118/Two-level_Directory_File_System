#define _CRT_SECURE_NO_WARNINGS

#include "menu.h"
#include "common.h"
#include "command.h"
#include "MFD.h"
#include "Fileoperate.h"
using namespace std;

// ����ȫ�ֱ�������ʾ�û���Ϣ���ļ���Ϣ���ļ�״̬���ļ��ص����ݽṹ
vector<MFD> UsrInfo;              // ���ļ�Ŀ¼������
vector<vector<UFD>> FileInfo;     // �û��ļ�Ŀ¼������������ -----> ��ǰ��¼�û�����һά���µ� Ŀ¼���ڶ�ά�� 
vector<vector<UOF>> FileState;     // �û����ļ������������� -----> ��ǰ��¼�û�����һά���µ� �ļ����ڶ�ά�� 
vector<Cluster> FileCluster;       // �������������

// ���嵥�������������ṹ��
MFD UsrInput;                     // �û���������ļ�Ŀ¼��
UFD FileInput;                    // �û�������û��ļ�Ŀ¼��
UOF StateInput;                   // �û�������û����ļ���
Cluster ClusterInput;             // �û�����Ĵ��������

int num;                          // ѡ��ѡ�����
int Headnum;                      // ͷ�ڵ���
int curID;                        // ��ǰ�û��ı��


// ��������б�
Hand_to handlerlist[] = {
    {"chmod", chmod},{"rename", rename},{"cp", copy_check},
	{"cat", cat},{"passwd", chage_psw},{"login", login},
	{"logout", logout},{"create", create},{"rd", rd}, 
	{"write", write},{"help", help},{"ls", ls},{"save", save},
	{"register", Register},{"exit", exit},{"cls", clear},
    {NULL, NULL}
};


// ���������
int main()
{
	system("color F0");
    loginWelcome();      // ��½��ӭ��Ϣ
    disk2mem();     // ��ʼ���ļ�ϵͳ���ڴ�

    // ��ѭ��
    while (1)
    {
        // Ĭ����root�û���½
        cout << "\\" << UsrInfo[curID].usrname << "> ";
        cmd_in_parse();    // �����û����������
    }

    return 0;
}

