/********************************************************************
filename : OCXRoleMgr.h
author : s00191067
created : 2013/01/14
description : ��ɫ������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/01/14 ��ʼ�汾
*********************************************************************/


#ifndef __OCX_ROLE_MGR_H_
#define __OCX_ROLE_MGR_H_

#include "hwsdk.h"
#include "IVSCommon.h"

class COCXRoleMgr
{
public:
    COCXRoleMgr(void);
    ~COCXRoleMgr(void);

public:
    // ���ӽ�ɫ
    static CString AddRole(IVS_INT32 iSessionID, LPCTSTR pRoleInfo);

    // ɾ����ɫ
    static LONG DeleteRole(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulRoleID);

    // �޸Ľ�ɫ
    static LONG ModifyRole(IVS_INT32 iSessionID, LPCTSTR pRoleInfo);

    // ��ѯ��ɫ�б�
    static CString GetRoleList(IVS_INT32 iSessionID, LPCTSTR pDomainCode);

    // ���ݽ�ɫID��ȡ��ɫ��ϸ��Ϣ
    static CString GetRoleInfoByRoleID(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulRoleID);

    // �����û�ID��ȡ��ɫ��ϸ��Ϣ
    static CString GetRoleInfoByUserID(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserID);

    // ��ȡĬ���˺Ź���
    static CString GetDefaultRoleAccountRule(IVS_INT32 iSessionID, LPCTSTR pDomainCode);

    // ��ȡ�û�Ȩ����Ϣ
    static CString GetOperationRightGroup(IVS_INT32 iSessionID, LPCTSTR pDomainCode);

};

#endif
