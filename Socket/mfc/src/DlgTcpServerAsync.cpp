//DlgTcpServerAsync.cpp
#include "stdafx.h"
#include "mfcsocket.h"
#include "DlgMain.h"
#include "DlgTcpServerAsync.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgTcpServerAsync::CDlgTcpServerAsync(CWnd* pParent /*=NULL*/)
: CDialog(CDlgTcpServerAsync::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTcpServerAsync)
	//}}AFX_DATA_INIT
    m_SocketListen.SetEventHandler(this);
}

void CDlgTcpServerAsync::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTcpServerAsync)
	DDX_Control(pDX, IDC_LIST_CLIENT, m_lstClient);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTcpServerAsync, CDialog)
	//{{AFX_MSG_MAP(CDlgTcpServerAsync)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgTcpServerAsync::OnInitDialog() 
{
	CDialog::OnInitDialog();
    SetWindowText(_T("TCP �첽ͨѶ�����"));    
    SetDlgItemInt(IDC_TXT_PORT,2001);	
    EnableControls();
	return TRUE;
}

void CDlgTcpServerAsync::OnOK() 
{
}

void CDlgTcpServerAsync::OnCancel() 
{
}

void CDlgTcpServerAsync::OnClose() 
{
    DestroyWindow();
}

void CDlgTcpServerAsync::PostNcDestroy() 
{
    Disconnect();
	theApp.m_pMain->RemoveDialog(this);	
}

void CDlgTcpServerAsync::OnBtnConnect() 
{
    {//�ȶϿ�����
        Disconnect();
        m_sRecv.Empty();
        SetDlgItemText(IDC_TXT_RECV,_T(""));
        m_lstClient.ResetContent();
    }
    int                 nPort   =   GetDlgItemInt(IDC_TXT_PORT);
    CSocketTcpListen&   s       =   m_SocketListen;
    
    if(s.Create(nPort) && s.Listen())
    {//�����ɹ�
        EnableControls();
    }
    else
    {//����ʧ��
        s.Close();
        AfxMessageBox(_T("����ʧ�ܣ������Ƕ˿ڱ�ռ��"));
        GotoDlgCtrl(GetDlgItem(IDC_TXT_PORT));
    }    
}

void CDlgTcpServerAsync::Disconnect()
{
    CSocketAsync*pClient =   NULL;
    for(MapClient::iterator it = m_mapClient.begin();it != m_mapClient.end();++it)
    {//�Ͽ�������ͻ��˵� TCP ����
        if(pClient = it->first)
        {
            pClient->ShutDown(CAsyncSocket::both);
            pClient->Close();
            delete pClient;
        }
    }
    m_mapClient.clear();
    //ֹͣ����
    if(m_SocketListen.m_hSocket != INVALID_SOCKET)
    {
        m_SocketListen.ShutDown(CAsyncSocket::both);
        m_SocketListen.Close();        
    }
}

void CDlgTcpServerAsync::OnBtnDisconnect() 
{
    Disconnect();
    EnableControls();
}

void CDlgTcpServerAsync::OnBtnSend() 
{
    std::string s   =   GetWndText(::GetDlgItem(m_hWnd,IDC_TXT_SEND));
    if(!s.empty())
    {
        CSocketAsync*pClient =   NULL;
        for(int i = m_lstClient.GetCount() - 1;i >= 0;--i)        
        {//���͸��б���ѡ�еĿͻ���
            if(m_lstClient.GetSel(i)
            && (pClient = (CSocketAsync*)m_lstClient.GetItemData(i))
            )
            {
                pClient->Send(s.c_str(),s.length());
            }
        }
    }
}

void CDlgTcpServerAsync::EnableControls()
{
    bool bSocket    =   m_SocketListen.m_hSocket != INVALID_SOCKET;
    ((CEdit*)GetDlgItem(IDC_TXT_PORT))->SetReadOnly(bSocket ? TRUE  : FALSE);
    ((CEdit*)GetDlgItem(IDC_TXT_SEND))->SetReadOnly(bSocket ? FALSE : TRUE );
    GetDlgItem(IDC_BTN_CONNECT   )->EnableWindow(bSocket ? FALSE : TRUE );
    GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(bSocket ? TRUE  : FALSE);
    GetDlgItem(IDC_BTN_SEND      )->EnableWindow(bSocket ? TRUE  : FALSE);
}

//�ͻ�������
void CDlgTcpServerAsync::OnAccept(CAsyncSocket*pSender,int nErrorCode)
{
    if(0 == nErrorCode
  //&& pSender == &m_SocketListen
    )
    {
        CSocketAsync*pClient =   new CSocketAsync(this);
        if(m_SocketListen.Accept(*pClient))
        {
            CString sPeer           =   GetPeerName(pClient);
            m_mapClient[pClient]    =   sPeer;
            {//�ͻ��˼����б�
                int nIndex  =   m_lstClient.AddString(sPeer);
                m_lstClient.SetItemData(nIndex,(UINT_PTR)pClient);
                m_lstClient.SetSel(nIndex,1);
            }
            m_sRecv                +=   sPeer + _T(" > ����\r\n");
            SetEditText(::GetDlgItem(m_hWnd,IDC_TXT_RECV),m_sRecv);            
        }
        else
        {
            delete pClient;
        }
    }
}

//�ͻ��˷������ݹ���
void CDlgTcpServerAsync::OnReceive(CAsyncSocket*pSender,int nErrorCode)
{
    if(pSender)
    {
        CSocketAsync*pClient    =   (CSocketAsync*)pSender;
        char            buf[1024];
        int             nRead   =   0;
        std::string     s;

        while((nRead = pClient->Receive(buf,sizeof(buf))) > 0)
        {
            s  +=   std::string((const char*)buf,nRead);
        }
        if(!s.empty())
        {
            m_sRecv +=  GetPeerName(pClient) + _T(" > ") + CString(s.c_str(),s.length()) + _T("\r\n");
            if(m_sRecv.GetLength() > MAXRECV)
            {
                m_sRecv =   m_sRecv.Right(MAXRECV);
            }
            SetEditText(::GetDlgItem(m_hWnd,IDC_TXT_RECV),m_sRecv);
        }
    }
}

//�ͻ�������
void CDlgTcpServerAsync::OnClose(CAsyncSocket*pSender,int nErrorCode)
{
    if(pSender)
    {
        CSocketAsync*pClient    =   (CSocketAsync*)pSender;
        {//���б���ɾ���ͻ���
            for(int i = m_lstClient.GetCount() - 1;i >= 0;--i)
            {
                if(m_lstClient.GetItemData(i) == (UINT_PTR)pClient)
                {
                    m_lstClient.DeleteString(i);
                    break;
                }
            }        
        }
        MapClient::iterator it = m_mapClient.find(pClient);
        if(it != m_mapClient.end())
        {
            m_sRecv +=  it->second + _T(" > ����\r\n");
            m_mapClient.erase(pClient);
            SetEditText(::GetDlgItem(m_hWnd,IDC_TXT_RECV),m_sRecv);
        }
        pClient->ShutDown(CAsyncSocket::both);
        pClient->Close();
        delete pClient;        
    }
}
