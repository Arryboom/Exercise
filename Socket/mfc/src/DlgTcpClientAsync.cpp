//DlgTcpClientAsync.cpp
#include "stdafx.h"
#include "mfcsocket.h"
#include "DlgMain.h"
#include "DlgTcpClientAsync.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgTcpClientAsync::CDlgTcpClientAsync(CWnd* pParent /*=NULL*/)
:CDialog(CDlgTcpClientAsync::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTcpClientAsync)
	//}}AFX_DATA_INIT
    m_Socket.SetEventHandler(this);
    m_bConn =   false;
}

void CDlgTcpClientAsync::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTcpClientAsync)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTcpClientAsync, CDialog)
	//{{AFX_MSG_MAP(CDlgTcpClientAsync)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgTcpClientAsync::OnInitDialog() 
{
	CDialog::OnInitDialog();
    SetWindowText(_T("TCP �첽ͨѶ�ͻ���"));
    SetDlgItemText(IDC_TXT_IP,_T("127.0.0.1"));
    SetDlgItemInt(IDC_TXT_PORT,2001);	
    EnableControls();
	return TRUE;
}

void CDlgTcpClientAsync::OnOK() 
{
}

void CDlgTcpClientAsync::OnCancel() 
{
}

void CDlgTcpClientAsync::OnClose() 
{
    DestroyWindow();
}

void CDlgTcpClientAsync::PostNcDestroy() 
{
    Disconnect();
	theApp.m_pMain->RemoveDialog(this);	
}

void CDlgTcpClientAsync::OnBtnConnect() 
{
    {//�Ͽ�����
        Disconnect();
        m_sRecv =   "";
        SetDlgItemText(IDC_TXT_LOCAL,_T(""));
        SetDlgItemText(IDC_TXT_RECV ,_T(""));
    }
    if(m_Socket.Socket())
    {//�����׽��ֳɹ�
        CString sIP;
        GetDlgItemText(IDC_TXT_IP,sIP);
        int     nPort   =   GetDlgItemInt(IDC_TXT_PORT);
        
        if(m_Socket.Connect(sIP,nPort))
        {//���ӷ������ɹ�
            OnConnect(&m_Socket,0);
        }
        else if(GetLastError() == WSAEWOULDBLOCK)
        {//���Ӳ������������Ӳ������ʱ����� OnConnect ����
        }
        else
        {
            m_Socket.Close();
            SetDlgItemText(IDC_TXT_LOCAL,_T("����ʧ��"));
        }
    }
}

void CDlgTcpClientAsync::Disconnect()
{
    if(m_Socket.m_hSocket != INVALID_SOCKET)
    {
        m_Socket.ShutDown(CAsyncSocket::both);
        m_Socket.Close();
    }
    m_bConn =   false;
}

void CDlgTcpClientAsync::OnBtnDisconnect() 
{
    Disconnect();
    EnableControls();
}

void CDlgTcpClientAsync::OnBtnSend() 
{
    std::string s   =   GetWndText(::GetDlgItem(m_hWnd,IDC_TXT_SEND));
    if(!s.empty())
    {
        m_Socket.Send(s.c_str(),s.length());
    }
}

void CDlgTcpClientAsync::EnableControls()
{
    bool bSocket    =   m_Socket.m_hSocket != INVALID_SOCKET;
    if(bSocket)
    {
        if(m_bConn)
        {//�Ѿ�����            
        }
        else
        {
            SetDlgItemText(IDC_TXT_LOCAL,_T("��������..."));
        }
    }
    else
    {
        SetDlgItemText(IDC_TXT_LOCAL,_T(""));        
    }
    ((CEdit*)GetDlgItem(IDC_TXT_IP  ))->SetReadOnly(bSocket ? TRUE : FALSE);
    ((CEdit*)GetDlgItem(IDC_TXT_PORT))->SetReadOnly(bSocket ? TRUE : FALSE);
    ((CEdit*)GetDlgItem(IDC_TXT_SEND))->SetReadOnly((bSocket && m_bConn) ? FALSE : TRUE);
    GetDlgItem(IDC_BTN_CONNECT   )->EnableWindow(bSocket ? FALSE: TRUE);
    GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(bSocket ? TRUE : FALSE);
    GetDlgItem(IDC_BTN_SEND      )->EnableWindow((bSocket && m_bConn) ? TRUE : FALSE);
}

void CDlgTcpClientAsync::OnConnect(CAsyncSocket*pSender,int nErrorCode)
{
    if(0 == nErrorCode)
    {
        m_bConn =   true;
        SetDlgItemText(IDC_TXT_LOCAL,GetSockName(&m_Socket));
        EnableControls();
    }
}

void CDlgTcpClientAsync::OnReceive(CAsyncSocket*pSender,int nErrorCode)
{
    char    buf[1024];
    int     nRead   =   0;
    while((nRead = m_Socket.Receive(buf,sizeof(buf))) > 0)
    {
        m_sRecv +=  std::string((const char*)buf,nRead);
    }
    if(m_sRecv.length() > MAXRECV)
    {
        m_sRecv =   m_sRecv.substr(m_sRecv.length() - MAXRECV);
    }
    SetEditText(::GetDlgItem(m_hWnd,IDC_TXT_RECV)
               ,CString(m_sRecv.c_str(),m_sRecv.length()));
}

void CDlgTcpClientAsync::OnClose(CAsyncSocket*pSender,int nErrorCode)
{
    OnBtnDisconnect();
}
