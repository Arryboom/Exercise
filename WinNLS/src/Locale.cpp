//Locale.cpp
#include "StdAfx.h"
#include "WinNLS.h"
#include "winnls.h"
#include "Locale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLocale::StrType   CLocale::s_mapStrType;
CLocale::IdTypeStr CLocale::s_mapLocale;

CLocale::CLocale()
{
    if(s_mapStrType.empty())
    {
        CType t;

#define MAP_STR_TYPE(x,e,c) t.m_nType = x; t.m_sENU = e; t.m_sCHS = c; s_mapStrType[_T(#x)] = t;

        MAP_STR_TYPE(LOCALE_ILANGUAGE               ,_T("language id")                                  ,_T("���� ID")              )//0x00000001
        MAP_STR_TYPE(LOCALE_SLANGUAGE               ,_T("localized name of language")                   ,_T("������(����)")         )//0x00000002
        MAP_STR_TYPE(LOCALE_SENGLANGUAGE            ,_T("English name of language")                     ,_T("������(Ӣ��)")         )//0x00001001
        MAP_STR_TYPE(LOCALE_SABBREVLANGNAME         ,_T("abbreviated language name")                    ,_T("������(��д)")         )//0x00000003
        MAP_STR_TYPE(LOCALE_SNATIVELANGNAME         ,_T("native name of language")                      ,_T("������(����)")         )//0x00000004
        MAP_STR_TYPE(LOCALE_ICOUNTRY                ,_T("country code")                                 ,_T("���Ҵ���")             )//0x00000005
        MAP_STR_TYPE(LOCALE_SCOUNTRY                ,_T("localized name of country")                    ,_T("������(����)")         )//0x00000006
        MAP_STR_TYPE(LOCALE_SENGCOUNTRY             ,_T("English name of country")                      ,_T("������(Ӣ��)")         )//0x00001002
        MAP_STR_TYPE(LOCALE_SABBREVCTRYNAME         ,_T("abbreviated country name")                     ,_T("������(��д)")         )//0x00000007
        MAP_STR_TYPE(LOCALE_SNATIVECTRYNAME         ,_T("native name of country")                       ,_T("������(����)")         )//0x00000008
        MAP_STR_TYPE(LOCALE_IDEFAULTLANGUAGE        ,_T("default language id")                          ,_T("ȱʡ���� ID")          )//0x00000009
        MAP_STR_TYPE(LOCALE_IDEFAULTCOUNTRY         ,_T("default country code")                         ,_T("ȱʡ���Ҵ���")         )//0x0000000A
        MAP_STR_TYPE(LOCALE_IDEFAULTCODEPAGE        ,_T("default oem code page")                        ,_T("ȱʡ OEM ����ҳ")      )//0x0000000B
        MAP_STR_TYPE(LOCALE_IDEFAULTANSICODEPAGE    ,_T("default ansi code page")                       ,_T("ȱʡ ANSI ����ҳ")     )//0x00001004
        MAP_STR_TYPE(LOCALE_IDEFAULTMACCODEPAGE     ,_T("default mac code page")                        ,_T("ȱʡ MAC ����ҳ")      )//0x00001011
        MAP_STR_TYPE(LOCALE_SLIST                   ,_T("list item separator")                          ,_T("")                     )//0x0000000C
        MAP_STR_TYPE(LOCALE_IMEASURE                ,_T("0 = metric, 1 = US")                           ,_T("��λ��0-�� 1-����")    )//0x0000000D
        MAP_STR_TYPE(LOCALE_SDECIMAL                ,_T("decimal separator")                            ,_T("С����ָ���")         )//0x0000000E
        MAP_STR_TYPE(LOCALE_STHOUSAND               ,_T("thousand separator")                           ,_T("ǧλ�ָ���")           )//0x0000000F
        MAP_STR_TYPE(LOCALE_SGROUPING               ,_T("digit grouping")                               ,_T("���ַ���")             )//0x00000010
        MAP_STR_TYPE(LOCALE_IDIGITS                 ,_T("number of fractional digits")                  ,_T("С��λ��")             )//0x00000011
        MAP_STR_TYPE(LOCALE_ILZERO                  ,_T("leading zeros for decimal")                    ,_T("ʮ����ǰ����")         )//0x00000012
        MAP_STR_TYPE(LOCALE_INEGNUMBER              ,_T("negative number mode")                         ,_T("����ģʽ")             )//0x00001010
        MAP_STR_TYPE(LOCALE_SNATIVEDIGITS           ,_T("native ascii 0-9")                             ,_T("����ASCII 0-9")        )//0x00000013
        MAP_STR_TYPE(LOCALE_SCURRENCY               ,_T("local monetary symbol")                        ,_T("���ҷ���(����)")       )//0x00000014
        MAP_STR_TYPE(LOCALE_SINTLSYMBOL             ,_T("intl monetary symbol")                         ,_T("���ҷ���(����)")       )//0x00000015
        MAP_STR_TYPE(LOCALE_SMONDECIMALSEP          ,_T("monetary decimal separator")                   ,_T("����С�������")       )//0x00000016
        MAP_STR_TYPE(LOCALE_SMONTHOUSANDSEP         ,_T("monetary thousand separator")                  ,_T("����ǧλ�ָ���")       )//0x00000017
        MAP_STR_TYPE(LOCALE_SMONGROUPING            ,_T("monetary grouping")                            ,_T("������")               )//0x00000018
        MAP_STR_TYPE(LOCALE_ICURRDIGITS             ,_T("# local monetary digits")                      ,_T("")                     )//0x00000019
        MAP_STR_TYPE(LOCALE_IINTLCURRDIGITS         ,_T("# intl monetary digits")                       ,_T("")                     )//0x0000001A
        MAP_STR_TYPE(LOCALE_ICURRENCY               ,_T("positive currency mode")                       ,_T("��������ģʽ")         )//0x0000001B
        MAP_STR_TYPE(LOCALE_INEGCURR                ,_T("negative currency mode")                       ,_T("���Ҹ���ģʽ")         )//0x0000001C
        MAP_STR_TYPE(LOCALE_SDATE                   ,_T("date separator")                               ,_T("���ڷָ���")           )//0x0000001D
        MAP_STR_TYPE(LOCALE_STIME                   ,_T("time separator")                               ,_T("ʱ��ָ���")           )//0x0000001E
        MAP_STR_TYPE(LOCALE_SSHORTDATE              ,_T("short date format string")                     ,_T("�����ڸ�ʽ")           )//0x0000001F
        MAP_STR_TYPE(LOCALE_SLONGDATE               ,_T("long date format string")                      ,_T("�����ڸ�ʽ")           )//0x00000020
        MAP_STR_TYPE(LOCALE_STIMEFORMAT             ,_T("time format string")                           ,_T("ʱ���ʽ")             )//0x00001003
        MAP_STR_TYPE(LOCALE_IDATE                   ,_T("short date format ordering")                   ,_T("")                     )//0x00000021
        MAP_STR_TYPE(LOCALE_ILDATE                  ,_T("long date format ordering")                    ,_T("")                     )//0x00000022
        MAP_STR_TYPE(LOCALE_ITIME                   ,_T("time format specifier")                        ,_T("")                     )//0x00000023
        MAP_STR_TYPE(LOCALE_ITIMEMARKPOSN           ,_T("time marker position")                         ,_T("")                     )//0x00001005
        MAP_STR_TYPE(LOCALE_ICENTURY                ,_T("century format specifier (short date)")        ,_T("")                     )//0x00000024
        MAP_STR_TYPE(LOCALE_ITLZERO                 ,_T("leading zeros in time field")                  ,_T("")                     )//0x00000025
        MAP_STR_TYPE(LOCALE_IDAYLZERO               ,_T("leading zeros in day field (short date)")      ,_T("")                     )//0x00000026
        MAP_STR_TYPE(LOCALE_IMONLZERO               ,_T("leading zeros in month field (short date)")    ,_T("")                     )//0x00000027
        MAP_STR_TYPE(LOCALE_S1159                   ,_T("AM designator")                                ,_T("")                     )//0x00000028
        MAP_STR_TYPE(LOCALE_S2359                   ,_T("PM designator")                                ,_T("")                     )//0x00000029
        MAP_STR_TYPE(LOCALE_ICALENDARTYPE           ,_T("type of calendar specifier")                   ,_T("")                     )//0x00001009
        MAP_STR_TYPE(LOCALE_IOPTIONALCALENDAR       ,_T("additional calendar types specifier")          ,_T("")                     )//0x0000100B
        MAP_STR_TYPE(LOCALE_IFIRSTDAYOFWEEK         ,_T("first day of week specifier")                  ,_T("")                     )//0x0000100C
        MAP_STR_TYPE(LOCALE_IFIRSTWEEKOFYEAR        ,_T("first week of year specifier")                 ,_T("")                     )//0x0000100D
        MAP_STR_TYPE(LOCALE_SDAYNAME1               ,_T("long name for Monday")                         ,_T("��һȫ��")             )//0x0000002A
        MAP_STR_TYPE(LOCALE_SDAYNAME2               ,_T("long name for Tuesday")                        ,_T("�ܶ�ȫ��")             )//0x0000002B
        MAP_STR_TYPE(LOCALE_SDAYNAME3               ,_T("long name for Wednesday")                      ,_T("����ȫ��")             )//0x0000002C
        MAP_STR_TYPE(LOCALE_SDAYNAME4               ,_T("long name for Thursday")                       ,_T("����ȫ��")             )//0x0000002D
        MAP_STR_TYPE(LOCALE_SDAYNAME5               ,_T("long name for Friday")                         ,_T("����ȫ��")             )//0x0000002E
        MAP_STR_TYPE(LOCALE_SDAYNAME6               ,_T("long name for Saturday")                       ,_T("����ȫ��")             )//0x0000002F
        MAP_STR_TYPE(LOCALE_SDAYNAME7               ,_T("long name for Sunday")                         ,_T("����ȫ��")             )//0x00000030
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME1         ,_T("abbreviated name for Monday")                  ,_T("��һ���")             )//0x00000031
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME2         ,_T("abbreviated name for Tuesday")                 ,_T("�ܶ����")             )//0x00000032
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME3         ,_T("abbreviated name for Wednesday")               ,_T("�������")             )//0x00000033
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME4         ,_T("abbreviated name for Thursday")                ,_T("���ļ��")             )//0x00000034
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME5         ,_T("abbreviated name for Friday")                  ,_T("������")             )//0x00000035
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME6         ,_T("abbreviated name for Saturday")                ,_T("�������")             )//0x00000036
        MAP_STR_TYPE(LOCALE_SABBREVDAYNAME7         ,_T("abbreviated name for Sunday")                  ,_T("���ռ��")             )//0x00000037
        MAP_STR_TYPE(LOCALE_SMONTHNAME1             ,_T("long name for January")                        ,_T("һ��ȫ��")             )//0x00000038
        MAP_STR_TYPE(LOCALE_SMONTHNAME2             ,_T("long name for February")                       ,_T("����ȫ��")             )//0x00000039
        MAP_STR_TYPE(LOCALE_SMONTHNAME3             ,_T("long name for March")                          ,_T("����ȫ��")             )//0x0000003A
        MAP_STR_TYPE(LOCALE_SMONTHNAME4             ,_T("long name for April")                          ,_T("����ȫ��")             )//0x0000003B
        MAP_STR_TYPE(LOCALE_SMONTHNAME5             ,_T("long name for May")                            ,_T("����ȫ��")             )//0x0000003C
        MAP_STR_TYPE(LOCALE_SMONTHNAME6             ,_T("long name for June")                           ,_T("����ȫ��")             )//0x0000003D
        MAP_STR_TYPE(LOCALE_SMONTHNAME7             ,_T("long name for July")                           ,_T("����ȫ��")             )//0x0000003E
        MAP_STR_TYPE(LOCALE_SMONTHNAME8             ,_T("long name for August")                         ,_T("����ȫ��")             )//0x0000003F
        MAP_STR_TYPE(LOCALE_SMONTHNAME9             ,_T("long name for September")                      ,_T("����ȫ��")             )//0x00000040
        MAP_STR_TYPE(LOCALE_SMONTHNAME10            ,_T("long name for October")                        ,_T("ʮ��ȫ��")             )//0x00000041
        MAP_STR_TYPE(LOCALE_SMONTHNAME11            ,_T("long name for November")                       ,_T("ʮһ��ȫ��")           )//0x00000042
        MAP_STR_TYPE(LOCALE_SMONTHNAME12            ,_T("long name for December")                       ,_T("ʮ����ȫ��")           )//0x00000043
        MAP_STR_TYPE(LOCALE_SMONTHNAME13            ,_T("long name for 13th month (if exists)")         ,_T("ʮ����ȫ��")           )//0x0000100E
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME1       ,_T("abbreviated name for January")                 ,_T("һ�¼��")             )//0x00000044
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME2       ,_T("abbreviated name for February")                ,_T("���¼��")             )//0x00000045
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME3       ,_T("abbreviated name for March")                   ,_T("���¼��")             )//0x00000046
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME4       ,_T("abbreviated name for April")                   ,_T("���¼��")             )//0x00000047
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME5       ,_T("abbreviated name for May")                     ,_T("���¼��")             )//0x00000048
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME6       ,_T("abbreviated name for June")                    ,_T("���¼��")             )//0x00000049
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME7       ,_T("abbreviated name for July")                    ,_T("���¼��")             )//0x0000004A
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME8       ,_T("abbreviated name for August")                  ,_T("���¼��")             )//0x0000004B
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME9       ,_T("abbreviated name for September")               ,_T("���¼��")             )//0x0000004C
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME10      ,_T("abbreviated name for October")                 ,_T("ʮ�¼��")             )//0x0000004D
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME11      ,_T("abbreviated name for November")                ,_T("ʮһ�¼��")           )//0x0000004E
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME12      ,_T("abbreviated name for December")                ,_T("ʮ���¼��")           )//0x0000004F
        MAP_STR_TYPE(LOCALE_SABBREVMONTHNAME13      ,_T("abbreviated name for 13th month (if exists)")  ,_T("ʮ���¼��")           )//0x0000100F
        MAP_STR_TYPE(LOCALE_SPOSITIVESIGN           ,_T("positive sign")                                ,_T("��������")             )//0x00000050
        MAP_STR_TYPE(LOCALE_SNEGATIVESIGN           ,_T("negative sign")                                ,_T("��������")             )//0x00000051
        MAP_STR_TYPE(LOCALE_IPOSSIGNPOSN            ,_T("positive sign position")                       ,_T("��������λ��")         )//0x00000052
        MAP_STR_TYPE(LOCALE_INEGSIGNPOSN            ,_T("negative sign position")                       ,_T("��������λ��")         )//0x00000053
        MAP_STR_TYPE(LOCALE_IPOSSYMPRECEDES         ,_T("mon sym precedes pos amt")                     ,_T("")                     )//0x00000054
        MAP_STR_TYPE(LOCALE_IPOSSEPBYSPACE          ,_T("mon sym sep by space from pos amt")            ,_T("")                     )//0x00000055
        MAP_STR_TYPE(LOCALE_INEGSYMPRECEDES         ,_T("mon sym precedes neg amt")                     ,_T("")                     )//0x00000056
        MAP_STR_TYPE(LOCALE_INEGSEPBYSPACE          ,_T("mon sym sep by space from neg amt")            ,_T("")                     )//0x00000057
        MAP_STR_TYPE(LOCALE_FONTSIGNATURE           ,_T("font signature")                               ,_T("")                     )//0x00000058
        MAP_STR_TYPE(LOCALE_SISO639LANGNAME         ,_T("ISO abbreviated language name")                ,_T("������(ISO ��д)")     )//0x00000059
        MAP_STR_TYPE(LOCALE_SISO3166CTRYNAME        ,_T("ISO abbreviated country name")                 ,_T("������(ISO ��д)")     )//0x0000005A
        MAP_STR_TYPE(LOCALE_IDEFAULTEBCDICCODEPAGE  ,_T("default ebcdic code page")                     ,_T("ȱʡ EBCDIC ����ҳ")   )//0x00001012
        MAP_STR_TYPE(LOCALE_IPAPERSIZE              ,_T("0 = letter, 1 = a4, 2 = legal, 3 = a3")        ,_T("")                     )//0x0000100A
        MAP_STR_TYPE(LOCALE_SENGCURRNAME            ,_T("english name of currency")                     ,_T("������(Ӣ��)")         )//0x00001007
        MAP_STR_TYPE(LOCALE_SNATIVECURRNAME         ,_T("native name of currency")                      ,_T("������(����)")         )//0x00001008
        MAP_STR_TYPE(LOCALE_SYEARMONTH              ,_T("year month format string")                     ,_T("")                     )//0x00001006
        MAP_STR_TYPE(LOCALE_SSORTNAME               ,_T("sort name")                                    ,_T("����")                 )//0x00001013
        MAP_STR_TYPE(LOCALE_IDIGITSUBSTITUTION      ,_T("0 = none, 1 = context, 2 = native digit")      ,_T("")                     )//0x00001014

#undef  MAP_STR_TYPE

        Enum();
    }
}

BOOL CALLBACK CLocale::EnumLocalesProc(LPTSTR szLocaleStr)
{
    s_mapLocale[_tcstoul(szLocaleStr,NULL,16)];
    return TRUE;
}

//ö�ٴ���ҳ�� s_mapLocale
void CLocale::Enum()
{
    {//ö��
        s_mapLocale.clear();
        EnumSystemLocales(EnumLocalesProc,LCID_INSTALLED);    
    }
    {
        int     i   =   0;
        int     n   =   0;        ;
        CString s;
        for(IdTypeStr::iterator it1 = s_mapLocale.begin();it1 != s_mapLocale.end();++it1)
        {
            for(StrType::iterator it2 = s_mapStrType.begin();it2 != s_mapStrType.end();++it2)
            {                
                n   =   GetLocaleInfo(it1->first,it2->second.m_nType,NULL,0);
                if(n > 0)
                {
                    GetLocaleInfo(it1->first,it2->second.m_nType,s.GetBuffer(n),n);
                    s.ReleaseBuffer();
                }
                else
                {
                    s.Empty();
                }
                TypeStr&li  =   it1->second;
                li[it2->second.m_nType] =   s;
            }
        }
    }
}

CString CLocale::Value(LCID id,LCTYPE type)
{
    CString s;
    IdTypeStr::iterator it1 = s_mapLocale.find(id);
    if(it1 != s_mapLocale.end())
    {        
        TypeStr::iterator it2 = it1->second.find(type);
        if(it2 != it1->second.end())
        {
            s   =   it2->second;
        }
    }
    return s;
}

CString CLocale::Value(LCID id,const CString&type)
{
    CString s;
    StrType::iterator it = s_mapStrType.find(type);
    if(it != s_mapStrType.end())
    {
        s   =   Value(id,it->second.m_nType);
    }
    return s;
}
