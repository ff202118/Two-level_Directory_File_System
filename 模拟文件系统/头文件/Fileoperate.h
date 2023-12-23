#ifndef _FILEOPERATE_H_
#define _FILEOPERATE_H_


#include "common.h"
#include "MFD.h"
using namespace std;

// �����ⲿȫ�ֱ��������ڴ洢�û���Ϣ�������������û���Ϣ��ͷ���������ļ�״̬��Ϣ��
// �����ļ���Ϣ�������ļ�״̬��Ϣ�������ļ�����Ϣ���û��ļ�Ŀ¼��Ϣ���ļ�����Ϣ
extern vector<MFD> UsrInfo;                // �洢���ļ�Ŀ¼��Ϣ
extern int num;                            // ���ڼ���������
extern MFD UsrInput;                       // ���������û���Ϣ�Ľṹ�����
extern int Headnum;                         // �洢ͷ������������
extern vector<vector<UOF>> FileState;      // ��ά�����洢�û��ļ�״̬��Ϣ
extern UFD FileInput;                      // ���������ļ���Ϣ�Ľṹ�����
extern UOF StateInput;                      // ���������ļ�״̬��Ϣ�Ľṹ�����
extern Cluster ClusterInput;                // ���������ļ�����Ϣ�Ľṹ�����
extern vector<vector<UFD>> FileInfo;       // ��ά�����洢�û��ļ�Ŀ¼��Ϣ
extern vector<Cluster> FileCluster;        // �洢�ļ�����Ϣ

// ����: �Ӵ����ļ� "disk.txt" �ж�ȡ��ʼ����Ϣ�������û���Ϣ���ļ���Ϣ�ʹ��̿���Ϣ��
void disk2mem()
{
    FILE *streamInit;

    // �򿪴����ļ��������ʧ�������������Ϣ������
    if ((streamInit = fopen("disk.txt", "r")) == NULL)
    {
        cout << "�Ӵ����ж���ʱʧ�ܣ�" << endl;
        return;
    }

    // �����û���Ϣ
    fscanf(streamInit, "%d", &Headnum);
    int alreadynum;
    int ret;
    alreadynum = 0;

    // ���ж�ȡ�û���Ϣ������Ϣ�洢�� UsrInfo ��
    while (alreadynum < Headnum && (ret = fscanf(streamInit, "%s %s %d", &UsrInput.usrname, &UsrInput.usrpass, &UsrInput.link)) != -1)
    {
        UsrInfo.push_back(UsrInput);
        alreadynum++;
    }

    // ��ʼ���ļ���Ϣ
    vector<UFD> t;
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        FileInfo.push_back(t);
    }

    // ��ʼ���ļ�״̬��Ϣ
    vector<UOF> temp;
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        FileState.push_back(temp);
    }

    // ���û���ȡ�ļ���Ϣ
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        fscanf(streamInit, "%d", &Headnum);
        alreadynum = 0;

        // ���ж�ȡ�ļ���Ϣ������Ϣ�洢�� FileInfo ��
        while (alreadynum < Headnum && (ret = fscanf(streamInit, "%s %d %d %d", &FileInput.filename, &FileInput.mode, &FileInput.length, &FileInput.addr)) != -1)
        {
            FileInfo[i].push_back(FileInput);
            alreadynum++;
        }
    }

    // ���û���ȡ�ļ�״̬��Ϣ
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        fscanf(streamInit, "%d", &Headnum);
        alreadynum = 0;

        // ���ж�ȡ�ļ�״̬��Ϣ������Ϣ�洢�� FileState ��
        while (alreadynum < Headnum && (ret = fscanf(streamInit, "%s %d %d %d", &StateInput.filename, &StateInput.mode, &StateInput.state, &StateInput.write_poit)) != -1)
        {
            FileState[i].push_back(StateInput);
            alreadynum++;
        }
    }

    alreadynum = 0;
    char Tempbuf[258];

    // ����ȡ���̿���Ϣ������Ϣ�洢�� FileCluster ��
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

    // �ر��ļ���
    fclose(streamInit);
}


// ��ϵͳ��Ϣ������ļ�
void out_to_file()
{
    FILE* fd;
    // ���ļ���д��ģʽ
    fd = fopen("disk.txt", "w");

    // ����û���Ϣ
    fprintf(fd, "%d%c", UsrInfo.size(), ' ');
    for (int i = 0; i < UsrInfo.size(); i++)
    {
        fprintf(fd, "%s %s %d%c", UsrInfo[i].usrname, UsrInfo[i].usrpass, UsrInfo[i].link, ' ');
    }
    fprintf(fd, "\n");

    // ����ļ���Ϣ
    for (int i = 0; i < FileInfo.size(); i++)
    {
        fprintf(fd, "%d%c", FileInfo[i].size(), ' ');
        for (int j = 0; j < FileInfo[i].size(); j++)
        {
            fprintf(fd, "%s %d %d %d%c", FileInfo[i][j].filename, FileInfo[i][j].mode, FileInfo[i][j].length, FileInfo[i][j].addr, ' ');
        }
        fprintf(fd, "\n");
    }

    // ����ļ�״̬��Ϣ
    for (int i = 0; i < FileState.size(); i++)
    {
        fprintf(fd, "%d%c", FileState[i].size(), ' ');
        for (int j = 0; j < FileState[i].size(); j++)
        {
            fprintf(fd, "%s %d %d %d%c", FileState[i][j].filename, FileState[i][j].mode, FileState[i][j].state, FileState[i][j].write_poit, ' ');
        }
        fprintf(fd, "\n");
    }

    // �������Ϣ
    for (int i = 0; i < FileCluster.size(); i++)
    {
        fprintf(fd, "%d %d%c", FileCluster[i].next_num, FileCluster[i].is_data, ' ');
        fputs(FileCluster[i].data, fd);
        fprintf(fd, "\n");
    }

    // �ر��ļ�
    fclose(fd);
}


#endif
