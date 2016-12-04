//SocketMFC.h���� CSocket��ͬ���׽����ࣩ��CAsyncSocket���첽�׽����ࣩ�ķ�װ
#pragma once

#include <atlconv.h>
#include "ISocketEventHandler.h"

/******************************************************************************\
�׽����첽 TCP/UDP ͨѶ
\******************************************************************************/
class CSocketAsync : public CAsyncSocket
{
protected:
    enum { UDP_MAX_PKG = 65507 };   //UDP ���ݰ�����ֽ���
    class SendToData
    {
    public:
        std::string sAddr;
        std::string sData;
    };
    ISocketEventHandler*   m_pEventHandler;       //�׽����¼�������
    std::string     m_sSend;        //Send   ��������ķ�������
    std::list<SendToData>
                    m_lstSendTo;    //SendTo ��������ķ�������
public:
    CSocketAsync(ISocketEventHandler*pEvent = NULL)
    {
        m_pEventHandler    =   pEvent;
    }
    void SetEventHandler(ISocketEventHandler*pEvent)
    {
        m_pEventHandler    =   pEvent;
    }
    /**************************************************************************\
    ת�� IP ��ַ��ʽ
    szIP    [in]    �ַ�����ʽ�� IP ��ַ���磺192.168.1.200
    nPort   [in]    �˿ڣ���Χ [0,65535]���磺2001
    addr    [out]   ��ַ
    \**************************************************************************/
    static bool IpAddress(LPCTSTR szIP,UINT nPort,SOCKADDR_IN&addr)
    {
        USES_CONVERSION;        
        memset(&addr,0,sizeof(addr));
        char*szIpA  =   T2A((LPTSTR)szIP);
        if(szIpA)
        {
            addr.sin_addr.s_addr    =   inet_addr(szIpA);
            if(addr.sin_addr.s_addr == INADDR_NONE)
            {
                LPHOSTENT lphost    =   gethostbyname(szIpA);
                if(lphost)
                {
                    addr.sin_addr.s_addr    =   ((LPIN_ADDR)lphost->h_addr)->s_addr;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            addr.sin_addr.s_addr    =   htonl(INADDR_BROADCAST);
        }
        addr.sin_family =   AF_INET;
        addr.sin_port   =   htons((u_short)nPort);
        return true;
    }
public:
    //���ڷ��� TCP ����
    virtual int Send(const void*pData,int nLen,int nFlags = 0)
    {
        if(pData)
        {
            if(nLen < 0)
            {
                nLen    =   strlen((const char*)pData);
            }
            if(nLen > 0)
            {
                if(m_sSend.empty())
                {//�������� m_sSend Ϊ�գ���������
                    int n       =   0;  //���η��͵��ֽ���
                    int nSum    =   0;  //���͵��ۼ��ֽ���

                    for(;;)
                    {
                        n   =   nLen - nSum;
                        n   =   send(m_hSocket,(const char*)pData + nSum,n,0);
                        if(n > 0)
                        {
                            nSum   +=   n;
                            if(nSum >= nLen)
                            {
                                return nLen;
                            }
                        }
                        else if(n == SOCKET_ERROR)
                        {
                            if(WSAGetLastError() == WSAEWOULDBLOCK)
                            {//�����ݼ��뻺��
                                m_sSend    +=   std::string((const char*)pData + nSum,nLen - nSum);
                                return nLen;
                            }
                            else
                            {
                                return nSum;
                            }
                        }
                    }
                }
                else
                {//�������� m_sSend ��Ϊ�գ�ֱ�ӽ����ݼ��뻺��
                    m_sSend    +=   std::string((const char*)pData,nLen);
                    return nLen;
                }
            }
        }
        return 0;
    }
    //���ڷ��� UDP ����
    int SendTo(const void* lpBuf, int nBufLen,UINT nHostPort, LPCTSTR lpszHostAddress = NULL, int nFlags = 0)
    {        
        SOCKADDR_IN sockAddr;
        if(IpAddress(lpszHostAddress,nHostPort,sockAddr))
        {
            return SendTo(lpBuf,nBufLen,(SOCKADDR*)&sockAddr,sizeof(sockAddr), nFlags);
        }
        return 0;
    }
    //���ڷ��� UDP ����
    int SendTo(const void*pData,int nLen,const SOCKADDR*lpSockAddr,int nSockAddrLen,int nFlags = 0)
    {
        if(pData && lpSockAddr && nSockAddrLen > 0)
        {
            if(nLen < 0)
            {
                nLen    =   strlen((const char*)pData);
            }
            if(nLen > 0)
            {
                SendToData  data;
                if(m_lstSendTo.empty())
                {//�޻������ݣ�����
                    int n       =   0;  //���η��͵��ֽ���
                    int nSum    =   0;  //���͵��ۼ��ֽ���
                    for(;;)
                    {
                        n   =   nLen - nSum;    //�����͵��ֽ���
                        if(n > UDP_MAX_PKG)
                        {//�����͵��ֽ�������̫��
                            n   =   UDP_MAX_PKG;
                        }
                        n   =   CAsyncSocket::SendTo((const char*)pData + nSum,n,lpSockAddr,nSockAddrLen,nFlags);
                        if(n > 0)
                        {
                            nSum  +=   n;   //�ۼ�
                            if(nSum >= nLen)
                            {//�������
                                return nLen;
                            }
                        }
                        else if(n == SOCKET_ERROR)
                        {
                            switch(GetLastError())
                            {
                          //case WSAEMSGSIZE:       //���� 65507 ���ֽ�
                            case WSAEWOULDBLOCK:    //����������
                                data.sAddr.assign((const char*)lpSockAddr,nSockAddrLen);    //��ַ
                                data.sData.assign((const char*)pData + nSum,nLen - nSum);   //����
                                m_lstSendTo.push_back(data);                                //�����ַ������
                                return nLen;
                            default:
                                return nSum;
                            }
                        }
                    }
                }
                else
                {//�л������ݣ�ֱ�ӻ���
                    data.sAddr.assign((const char*)lpSockAddr,nSockAddrLen);    //��ַ
                    data.sData.assign((const char*)pData,nLen);                 //����
                    m_lstSendTo.push_back(data);                                //�����ַ������
                    return nLen;
                }
            }
        }
        return 0;
    }
protected:
    virtual void OnConnect(int nErrorCode)
    {//�����Ϸ�������
        if(m_pEventHandler)
        {
            m_pEventHandler->OnConnect(this,nErrorCode);
        }
    }
    virtual void OnReceive(int nErrorCode)
    {//���յ�����
        if(m_pEventHandler)
        {
            m_pEventHandler->OnReceive(this,nErrorCode);
        }
    }
    virtual void OnSend(int nErrorCode)
    {//���ͻ��������ˣ����Է���������
        if(m_sSend.length() > 0)
        {//���ͻ������� m_sSend
            int n       =   0;                  //���η��͵��ֽ���
            int nSum    =   0;                  //���͵��ۼ��ֽ���
            int nTotal  =   m_sSend.length();   //�����͵����ֽ���

            for(;;)
            {
                n   =   nTotal - nSum;  //�����͵��ֽ���
                n   =   send(m_hSocket,m_sSend.c_str() + nSum,n,0);
                if(n > 0)
                {                    
                    nSum   +=   n;
                    if(nSum >= nTotal)
                    {
                        break;
                    }
                }
                else if(n == SOCKET_ERROR)
                {
                    //WSAGetLastError() == WSAEWOULDBLOCK
                    break;
                }
            }
            if(nSum > 0)
            {
                m_sSend =   m_sSend.substr(nSum);
            }
        }
        if(!m_lstSendTo.empty())
        {//���ͻ������� m_lstSendTo
            for(std::list<SendToData>::iterator it = m_lstSendTo.begin();it != m_lstSendTo.end();)
            {
                if(DoSendToData(*it))
                {
                    it  =   m_lstSendTo.erase(it);
                }
                else
                {
                    break;
                }
            }
        }
    }
    virtual void OnClose(int nErrorCode)
    {//���Ӷ���
        if(m_pEventHandler)
        {
            m_pEventHandler->OnClose(this,nErrorCode);
        }
    }
protected:
    //����һ������
    bool DoSendToData(SendToData&data)
    {
        int nTotal  =   data.sData.length();    //���ֽ���
        int nSum    =   0;  //�����ֽ������ۼ�ֵ        
        int n       =   0;  //���η��͵��ֽ���

        for(;;)
        {
            n   =   nTotal - nSum;
            if(n <= 0)
            {
                return true;    //��һ�����ݷ��������
            }
            if(n > UDP_MAX_PKG)
            {//ÿ�η��͵��ֽ������ܹ���
                n    =   UDP_MAX_PKG;
            }
            n   =   sendto(m_hSocket,data.sData.c_str() + nSum,n,0
                          ,(const struct sockaddr*)data.sAddr.c_str(),data.sAddr.length());
            if(n > 0)
            {                
                nSum  +=   n;
            }
            else if(n == SOCKET_ERROR)
            {
                data.sData  =   data.sData.substr(nSum);
                //WSAGetLastError() == WSAEWOULDBLOCK
                break;
            }
        }
        return false;   //��һ������û�з������
    }
};

/******************************************************************************\
�׽���ͬ�� TCP/UDP ͨѶ
\******************************************************************************/
class CSocketSync : public CSocket
{
protected:
    enum { UDP_MAX_PKG = 65507 };       //UDP ���ݰ�����ֽ���
    ISocketEventHandler*   m_pEventHandler;    //�׽����¼�������
public:
    CSocketSync(ISocketEventHandler*pEvent = NULL)
    {
        m_pEventHandler    =   pEvent;
    }
    void SetEventHandler(ISocketEventHandler*pEvent)
    {
        m_pEventHandler    =   pEvent;
    }
public:
    //���ڷ��� UDP ����
    int SendTo(const void* lpBuf, int nBufLen,UINT nHostPort, LPCTSTR lpszHostAddress = NULL, int nFlags = 0)
    {        
        SOCKADDR_IN sockAddr;
        if(CSocketAsync::IpAddress(lpszHostAddress,nHostPort,sockAddr))
        {
            return SendTo(lpBuf,nBufLen,(SOCKADDR*)&sockAddr,sizeof(sockAddr),nFlags);
        }
        return 0;
    }
    //���ڷ��� UDP ����
    int SendTo(const void*pData,int nLen,const SOCKADDR*lpSockAddr,int nSockAddrLen,int nFlags = 0)
    {
        if(pData && lpSockAddr && nSockAddrLen > 0)
        {
            if(nLen < 0)
            {
                nLen    =   strlen((const char*)pData);
            }
            if(nLen > 0)
            {
                int n       =   0;  //���η��͵��ֽ���
                int nSum    =   0;  //���͵��ۼ��ֽ���
                for(;;)
                {
                    n   =   nLen - nSum;    //�����͵��ֽ���
                    if(n > UDP_MAX_PKG)
                    {//�����͵��ֽ�������̫��
                        n   =   UDP_MAX_PKG;
                    }
                    n   =   CSocket::SendTo(pData,n,lpSockAddr,nSockAddrLen,nFlags);
                    if(n > 0)
                    {
                        nSum  +=   n;   //�ۼ�
                        if(nSum >= nLen)
                        {//�������
                            return nLen;
                        }
                    }
                    else if(n == SOCKET_ERROR)
                    {
                        return nSum;
                    }
                }
            }
        }
        return 0;
    }
protected:
    virtual void OnReceive(int nErrorCode)
    {//���յ�����ʱ���ᴥ�����¼�
        if(m_pEventHandler)
        {
            m_pEventHandler->OnReceive(this,nErrorCode);
        }
    }
    virtual void OnClose(int nErrorCode)
    {//���Ӷ��ˣ��ᴥ�����¼�
        if(m_pEventHandler)
        {
            m_pEventHandler->OnClose(this,nErrorCode);
        }
    }
};

/******************************************************************************\
TCP �׽���֮���������������ͻ��˷����� TCP ��������
\******************************************************************************/
class CSocketTcpListen : public CAsyncSocket
{
public:
    CSocketTcpListen(ISocketEventHandler*pEvent = NULL)
    {
        m_pEventHandler    =   pEvent;
    }
    void SetEventHandler(ISocketEventHandler*pEvent)
    {
        m_pEventHandler    =   pEvent;
    }
protected:
    virtual void OnAccept(int nErrorCode)
    {//�ͻ��˷�����������ʱ���ᴥ�����¼�
        if(m_pEventHandler)
        {
            m_pEventHandler->OnAccept(this,nErrorCode);
        }
    }
public:
    ISocketEventHandler*m_pEventHandler;  //�׽����¼�������
};
