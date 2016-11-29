//CodePage.h
#pragma once

/******************************************************************************\
����ö�ٴ���ҳ
\******************************************************************************/
class CCodePage  
{
public:
    typedef std::map<UINT,CPINFOEX> UintInfo;
public:
    CCodePage();   
public:
    static void    Enum();
    static CString GetCodeRange(UINT nCodePage);
private:
    static BOOL CALLBACK EnumCodePagesProc(LPTSTR szCodePageStr);
public:
    static UintInfo s_mapCodePage;
};
