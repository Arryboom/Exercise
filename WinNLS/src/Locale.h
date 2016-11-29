//Locale.h
#pragma once

/******************************************************************************\
����ö�ٴ���ҳ
\******************************************************************************/
class CLocale
{
public:
    class CType
    {
    public:
        LCTYPE  m_nType;
        CString m_sENU; //Ӣ��˵��
        CString m_sCHS; //����˵��
    };
public:
    typedef std::map<CString,CType>     StrType;
    typedef std::map<LCTYPE,CString>    TypeStr;
    typedef std::map<LCID,TypeStr>      IdTypeStr;
public:
    CLocale();
public:
    static void     Enum();
    static CString  Value(LCID id,LCTYPE type);
    static CString  Value(LCID id,const CString&type);
private:    
    static BOOL CALLBACK EnumLocalesProc(LPTSTR szLocaleStr);
public:
    static StrType   s_mapStrType;
    static IdTypeStr s_mapLocale;
};
