/*****************************************************************
 filename    :    OCXUpgradeMgr.h
 author      :    wanglei 00165153
 created     :    2012/11/19
 description :    ʵ���豸��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/11/13 ��ʼ�汾
*****************************************************************/
#ifndef __OCX_UPGRADE_MGR_H__
#define __OCX_UPGRADE_MGR_H__

#include <afx.h>
#include "SDKDef.h"

class COCXUpgradeMgrMgr
{
private:
     COCXUpgradeMgrMgr();
    ~COCXUpgradeMgrMgr();
public:
    // �����豸��
    static IVS_INT32 GetCUVersion(IVS_INT32 iSessionID, CXml& xmlReq, CString& strResult);
};

#endif