//Replacer.h
#pragma once

class CReplacer
{
public:
    CReplacer();
   ~CReplacer();
public:
    bool Load(LPCTSTR szFile);
    bool Save(LPCTSTR szFile);
    void Replace(LPCTSTR szOld,LPCTSTR szNew);
    void Empty();
protected:
    char*   m_pData;    //����
    int     m_nBytes;   //�ֽ���
};
