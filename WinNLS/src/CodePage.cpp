//CodePage.cpp
#include "StdAfx.h"
#include "WinNLS.h"
#include "winnls.h"
#include "CodePage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCodePage::UintInfo CCodePage::s_mapCodePage;

CCodePage::CCodePage()
{
    if(s_mapCodePage.empty())
    {
        Enum();
    }
}

BOOL CALLBACK CCodePage::EnumCodePagesProc(LPTSTR szCodePageStr)
{
    s_mapCodePage[_tcstoul(szCodePageStr,NULL,10)];
    return TRUE;
}

//ö�ٴ���ҳ�� s_mapCodePage
void CCodePage::Enum()
{
    {//ö��
        s_mapCodePage.clear();
        EnumSystemCodePages(CCodePage::EnumCodePagesProc,CP_INSTALLED);
      //EnumSystemCodePages(EnumCodePagesProc,CP_SUPPORTED);
    }
    {//��ȡÿ������ҳ����Ϣ        
        for(UintInfo::iterator it = s_mapCodePage.begin();it != s_mapCodePage.end();++it)
        {
            CPINFOEX&   ci  =   it->second;
            if(!GetCPInfoEx(it->first,0,&it->second))
            {
                memset(&ci,0,sizeof(ci));
                _stprintf(ci.CodePageName,_T("%u"),it->first);
            }
        }
    }
}

//��ȡĳ������ҳ�ı����ֽ�����ÿ���ֽڵı��뷶Χ
CString CCodePage::GetCodeRange(UINT nCodePage)
{
    const int   N       =   32; //���ֽ��ַ�������ֽ���    
    wchar_t     w       =   0;  //UTF-16 �ַ�
    BYTE        s[N];           //���ֽ��ַ�
    int         nA      =   0;  //���ֽ��ַ��ֽ���
    int         nAMin   =   100;//���ֽ��ַ��ֽ�������Сֵ
    int         nAMax   =   0;  //���ֽ��ַ��ֽ��������ֵ
    int         i       =   0;
    struct
    {
        int     nCount;
        BYTE    Min[N];
        BYTE    Max[N];
    }Range[N + 1];
    
    memset(Range,0,sizeof(Range));
    do
    {
        nA   =   WideCharToMultiByte(nCodePage,0,&w,1,(char*)s,sizeof(s),NULL,NULL);
        if(nA > 0)
        {
            if(nAMin > nA)
            {
                nAMin   =   nA;
            }
            if(nAMax < nA)
            {
                nAMax   =   nA;
            }
            if(Range[nA].nCount++)
            {
                for(i = 0;i < nA;++i)
                {
                    if(Range[nA].Min[i] > s[i])
                    {
                        Range[nA].Min[i]    =   s[i];
                    }
                    else if(Range[nA].Max[i] < s[i])
                    {
                        Range[nA].Max[i]    =   s[i];
                    }
                }
            }
            else
            {
                for(i = 0;i < nA;++i)
                {
                    Range[nA].Min[i]    =
                    Range[nA].Max[i]    =   s[i];
                }
            }
        }
    }while(++w);
    CString sText;
    CString sLine;
    TCHAR   szRange[32];
    for(nA = nAMin;nA <= nAMax;++nA)
    {
        if(Range[nA].nCount)
        {
            sLine.Format(_T("%d �ֽ� %5d ����"),nA,Range[nA].nCount);
            for(i = 0;i < nA;++i)
            {
                _stprintf(szRange,_T("\r\n    [0x%02X,0x%02X]"),Range[nA].Min[i],Range[nA].Max[i]);
                sLine  +=   szRange;
            }
            sText  +=   sLine + _T("\r\n");
        }
    }
    return sText;    
}
