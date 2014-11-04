/********************************************************************
filename : OCXUserGroupMgr.h
author : s00191067
created : 2013/01/14
description : �û��������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/01/14 ��ʼ�汾
*********************************************************************/


#ifndef __OCX_USERGROUP_MGR_H_
#define __OCX_USERGROUP_MGR_H_

#include "hwsdk.h"

class COCXUserGroupMgr
{
public:
    COCXUserGroupMgr(void);
    ~COCXUserGroupMgr(void);

public:
    // �����û���
    static CString AddUserGroup(IVS_INT32 iSessionID, LPCTSTR pUserGroup);

    // ɾ���û���
    static LONG DeleteUserGroup(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserGroupID);

    // �޸��û���
    static LONG ModifyUserGroup(IVS_INT32 iSessionID, LPCTSTR pUserGroup);

    // �ƶ��û���
    static LONG MoveUserGroup(IVS_INT32 iSessionID, LPCTSTR pUserGroup);

    // ��ѯ�û����б�
    static CString GetUserGroupList(IVS_INT32 iSessionID, LPCTSTR pReqXml);

};

#endif
