//Account.cpp
#include "StdAfx.h"
#include "Account.h"
#include <atlbase.h>

/******************************************************************************\
��ѯ���в����˻�����Ա���� m_vItem ��
\******************************************************************************/
void CAccount::Query()
{
    HKEY    hKeyAcc =   NULL;

    m_vItem.clear();
    if(RegOpenKeyEx(HKEY_CURRENT_USER
                   ,_T("Software\\Microsoft\\Office\\Common\\Blog\\Accounts")
                   ,0,KEY_READ,&hKeyAcc) == ERROR_SUCCESS)
    {
        TCHAR   szStr[MAX_PATH];
        HKEY    hKey    =   NULL;
        DWORD   dwSize  =   0;
        Item    t;
        int     nFlag   =   0;

        for(DWORD dwIndex = 0;RegEnumKey(hKeyAcc,dwIndex,szStr,MAX_PATH) == ERROR_SUCCESS;++dwIndex)
        {
            if(RegOpenKeyEx(hKeyAcc,szStr,0,KEY_READ,&hKey) == ERROR_SUCCESS)
            {
                t.sID   =   szStr;  //GUID
                nFlag   =   0;
                dwSize  =   sizeof(szStr);
                if(ERROR_SUCCESS == RegQueryValueEx(hKey,_T("FriendlyName"),NULL,NULL,(LPBYTE)szStr,&dwSize))
                {
                    t.sName =   szStr;  //FriendlyName
                    nFlag  |=   1;
                }
                dwSize  =   sizeof(szStr);
                if(ERROR_SUCCESS == RegQueryValueEx(hKey,_T("PostUrl"),NULL,NULL,(LPBYTE)szStr,&dwSize))
                {
                    t.sUrl  =   szStr;  //PostUrl
                    nFlag  |=   2;
                }
                RegCloseKey(hKey);
                if(3 == nFlag)
                {
                    m_vItem.push_back(t);
                }
            }
        }
        RegCloseKey(hKeyAcc);
    }    
}

/******************************************************************************\
�޸Ĳ����˻��� FriendlyName
nIndex  [in]    ���������㿪ʼ���
szName  [in]    �µ��Ѻ�����
���أ��Ƿ��޸ĳɹ�
\******************************************************************************/
bool CAccount::ModifyFriendlyName(int nIndex,LPCTSTR szName)
{
    bool bModify    =   false;
    if(szName && szName[0]  //������Ч
    && nIndex >= 0 && nIndex <  m_vItem.size())//������Ч
    {
        Item&item =   m_vItem[nIndex];
        if(item.sName != szName)
        {
            HKEY    hKey    =   NULL;
            if(RegOpenKeyEx(HKEY_CURRENT_USER
                           ,(_T("Software\\Microsoft\\Office\\Common\\Blog\\Accounts\\") + item.sID).c_str()
                           ,0,KEY_WRITE,&hKey) == ERROR_SUCCESS)
            {
                if(RegSetValueEx(hKey,_T("FriendlyName"),0,REG_SZ,(CONST BYTE*)szName
                               ,(lstrlen(szName) + 1) * sizeof(TCHAR)) == ERROR_SUCCESS)
                {
                    item.sName  =   szName;
                    bModify     =   true;
                }
                RegCloseKey(hKey);
            }
        }
    }
    return bModify;
}

/******************************************************************************\
���ݲ����˻���Ϣ���ļ�
szFile  [in]    �ļ���
���أ��Ƿ񱸷ݳɹ�
\******************************************************************************/
bool CAccount::Backup(LPCTSTR szFile)
{
    bool bOK = false;
    if(szFile && szFile[0])
    {
        FILE*fp =   _tfopen(szFile,_T("wb"));
        if(fp)
        {//�ж��ļ��Ƿ��д
            fclose(fp);
            DeleteFile(szFile);
            TCHAR   szCmd[1024];
            szCmd[0]        =   _T('\"');
            GetWindowsDirectory((TCHAR*)szCmd + 1,MAX_PATH);
            int     nLen    =   lstrlen(szCmd);
            if(szCmd[nLen - 1] != _T('\\'))
            {//���һ���ַ����Ƿ�б�ܣ�������
                szCmd[nLen++]   =   _T('\\');
            }
            lstrcpy(szCmd + nLen,_T("regedit.exe\" /e \""));
            lstrcat(szCmd,szFile);
            lstrcat(szCmd,_T("\" HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\Common\\Blog\\Accounts"));

            STARTUPINFO         si;
            memset(&si,0,sizeof(si));
            si.cb   =   sizeof(si);
            PROCESS_INFORMATION pi;
            memset(&pi,0,sizeof(pi));
            if(CreateProcess(NULL,szCmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
            {
                WaitForSingleObject(pi.hProcess,INFINITE);  //�ȴ����̽���
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                bOK =   GetFileAttributes(szFile) != 0xFFFFFFFF;
            }
        }
    }
    return bOK;
}

/******************************************************************************\
��ԭ�����˻���Ϣ
szFile  [in]    �ļ���
���أ��Ƿ�ԭ�ɹ�
\******************************************************************************/
bool CAccount::Restore(LPCTSTR szFile)
{
    bool bOK = false;
    if(szFile && szFile[0])
    {
        if(GetFileAttributes(szFile) == 0xFFFFFFFF)
        {//�����ļ������ڣ�ɾ��ע�����
            CRegKey Key;
            if(Key.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Office\\Common\\Blog"),KEY_ALL_ACCESS) == ERROR_SUCCESS)
            {
                bOK =   Key.RecurseDeleteKey(_T("Accounts")) == ERROR_SUCCESS;
                Key.Close();
            }
            else
            {
                bOK =   true;
            }
        }
        else
        {
            TCHAR   szCmd[1024];
            szCmd[0]        =   _T('\"');
            GetWindowsDirectory((TCHAR*)szCmd + 1,MAX_PATH);
            int     nLen    =   lstrlen(szCmd);
            if(szCmd[nLen - 1] != _T('\\'))
            {//���һ���ַ����Ƿ�б�ܣ�������
                szCmd[nLen++]   =   _T('\\');
            }
            lstrcpy(szCmd + nLen,_T("regedit.exe\" /s \""));
            lstrcat(szCmd,szFile);
            lstrcat(szCmd,_T("\""));
            STARTUPINFO         si;
            memset(&si,0,sizeof(si));
            si.cb   =   sizeof(si);
            PROCESS_INFORMATION pi;
            memset(&pi,0,sizeof(pi));
            if(CreateProcess(NULL,szCmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
            {
                WaitForSingleObject(pi.hProcess,INFINITE);  //�ȴ����̽���
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                bOK =   true;
            }
        }
    }
    return bOK;
}
