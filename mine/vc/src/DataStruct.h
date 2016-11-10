//DataStruct.h
#pragma once

#define conIsMine       1   //�ǵ���
#define conDiscovered   2   //����
#define conMarked       4   //������?��
#define conFounded      8   //������(���Ϻ���)
#define conChoosed      16  //��ѡ��

#define STA_NONE        0   //��û�п�ʼ����
#define STA_WORKING     1   //��������
#define STA_OK          2   //�Ѿ��ں�
#define STA_DIED        3   //����

#define FACE_DOWN       0   //��������
#define FACE_OK         1   //ʤ������
#define FACE_DIED       2   //��������
#define FACE_WORK       3   //��������
#define FACE_WAIT       4   //�ȴ�����

typedef struct tagBlock
{
    BYTE Totalmines;
    BYTE Flag;
}BLOCK;

typedef struct tagIniInfo
{
    int     Difficulty;
    int     Height;
    int     Width;
    int     Mines;
    int     Mark;    
    int     Xpos;
    int     Ypos;
    int     Time1;
    int     Time2;
    int     Time3;
    CString Name1;
    CString Name2;
    CString Name3;
    int     Protect;
    //Color =1
}INIINFO;
