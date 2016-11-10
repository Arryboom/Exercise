//ON_WM_XXX.h
#pragma once

//���ļ����������� ON_WM_XXX()

/******************************************************************************\
ON_WM_ACTIVATEAPP()
vc6 ����Ӧ����Ϊ void OnActivateApp(BOOL bActive, HTASK hTask) 
�� vc2002 ��ʼ�ڶ��������� HTASK ��Ϊ DWORD
���û��ຯ�� CWnd::OnActivateApp
#if _MSC_VER >= 1300
    CWnd::OnActivateApp(BOOL.DWORD);
#else
    CWnd::OnActivateApp(BOOL.HTASK);
#endif
\******************************************************************************/

/******************************************************************************\
ON_WM_TIMER()
vc6 ����Ӧ����Ϊ void OnTimer(UINT nIDEvent) 
�� vc2002 ��ʼ��һ�������� UINT ��Ϊ UINT_PTR
\******************************************************************************/

/******************************************************************************\
ON_WM_NCHITTEST()
vc6 ����Ӧ����Ϊ UINT OnNcHitTest(CPoint point)
�� vc2005 ��ʼ������ֵ�� UINT ����Ϊ LRESULT
\******************************************************************************/
#if _MSC_VER == 1300 || _MSC_VER == 1310
    #undef  ON_WM_NCHITTEST
    #define ON_WM_NCHITTEST() {WM_NCHITTEST,0,0,0,AfxSig_wp,(AFX_PMSG)(AFX_PMSGW)(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(CPoint) > (OnNcHitTest))},
#endif

/******************************************************************************\
����� MODIFY_MESSAGE_MAP�������޸���Ϣӳ��������
ע�⣺���¶����˺� DECLARE_MESSAGE_MAP �� BEGIN_MESSAGE_MAP
ʾ���������£�
BEGIN_MESSAGE_MAP(CDlgMain,CDialog)
    ON_MESSAGE(-1,OnMsg)
END_MESSAGE_MAP()

BOOL CDlgMain::OnInitDialog() 
{
    //�������佫��Ϣ ID �� -1 ����Ϊ WM_USER + 100
    MODIFY_MESSAGE_MAP(CDlgMain,-1,WM_USER + 100)
    CDialog::OnInitDialog();
    return TRUE;
}
#if _MSC_VER >= 1400  //VC++8.0(VC2005)�������ϰ汾
    #undef  BEGIN_MESSAGE_MAP
    #define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    PTM_WARNING_DISABLE \
    const AFX_MSGMAP* theClass::GetMessageMap() const\
        { return GetThisMessageMap(); }\
    const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap()\
    {\
        typedef theClass ThisClass;\
        typedef baseClass TheBaseClass;\
        static AFX_MSGMAP_ENTRY _messageEntries[] =\
        {

    #define MODIFY_MESSAGE_MAP(theClass,uOld,uNew) \
    {\
        const AFX_MSGMAP_ENTRY* lpEntries =   theClass::GetThisMessageMap()->lpEntries;\
        if(lpEntries)\
        {\
            for(int i = 0;;){\
                const UINT&uMsg   =   lpEntries[i++].nMessage;\
                if(!uMsg) { break; }\
                if(uMsg == (UINT)(uOld)){ (UINT&)uMsg = (UINT)(uNew); break; }\
            }\
        }\
    }\

#else
    #undef DECLARE_MESSAGE_MAP
    #undef BEGIN_MESSAGE_MAP
    #ifdef _AFXDLL
        #define DECLARE_MESSAGE_MAP() \
        private: \
            static AFX_MSGMAP_ENTRY _messageEntries[]; \
        protected: \
            static AFX_DATA const AFX_MSGMAP messageMap; \
            static const AFX_MSGMAP* PASCAL _GetBaseMessageMap(); \
            virtual const AFX_MSGMAP* GetMessageMap() const; \

        #define BEGIN_MESSAGE_MAP(theClass, baseClass) \
            const AFX_MSGMAP* PASCAL theClass::_GetBaseMessageMap() \
                { return &baseClass::messageMap; } \
            const AFX_MSGMAP* theClass::GetMessageMap() const \
                { return &theClass::messageMap; } \
            AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
            { &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
            AFX_COMDAT AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
            { \

    #else
        #define DECLARE_MESSAGE_MAP() \
        private: \
            static AFX_MSGMAP_ENTRY _messageEntries[]; \
        protected: \
            static AFX_DATA const AFX_MSGMAP messageMap; \
            virtual const AFX_MSGMAP* GetMessageMap() const; \

        #define BEGIN_MESSAGE_MAP(theClass, baseClass) \
            const AFX_MSGMAP* theClass::GetMessageMap() const \
                { return &theClass::messageMap; } \
            AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
            { &baseClass::messageMap, &theClass::_messageEntries[0] }; \
            AFX_COMDAT AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
            { \

    #endif
    #define MODIFY_MESSAGE_MAP(theClass,uOld,uNew) \
    {\
        const AFX_MSGMAP_ENTRY* lpEntries =   theClass::_messageEntries;\
        if(lpEntries)\
        {\
            for(int i = 0;;){\
                const UINT&uMsg   =   lpEntries[i++].nMessage;\
                if(!uMsg) { break; }\
                if(uMsg == (UINT)(uOld)){ (UINT&)uMsg = (UINT)(uNew); break; }\
            }\
        }\
    }\

#endif
\******************************************************************************/
