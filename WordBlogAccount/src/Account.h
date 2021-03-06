//Account.h
#pragma once

#include <vector>
#include <map>

/******************************************************************************\
这个类用来读写 Word 的博客账户，具体的就是读写注册表项：
HKEY_CURRENT_USER\Software\Microsoft\Office\Common\Blog\Accounts
\******************************************************************************/
class CAccount
{
public:
    class Item
    {
    public:
        tstring sID;    //用户 GUID，如：7e41bc5a-8c48-4492-9a29-9c75373856bb
        tstring sName;  //FriendlyName
        tstring sUrl;   //PostUrl
    };
public:
    /**************************************************************************\
    查询所有博客账户至成员变量 m_vItem 里
    \**************************************************************************/
    void Query();
    /**************************************************************************\
    修改博客账户的 FriendlyName
    nIndex  [in]    索引，从零开始编号
    szName  [in]    新的友好名称
    返回：是否修改成功
    \**************************************************************************/
    bool ModifyFriendlyName(int nIndex,LPCTSTR szName);
    /**************************************************************************\
    备份博客账户信息至文件
    szFile  [in]    文件名
    返回：是否备份成功
    \**************************************************************************/
    bool Backup(LPCTSTR szFile);
    /**************************************************************************\
    还原博客账户信息
    szFile  [in]    文件名
    返回：是否还原成功
    \**************************************************************************/
    bool Restore(LPCTSTR szFile);
public:
    std::vector<Item>   m_vItem;
};
