/********************************************************************
 filename    :    AutoResumeTrigger.h
 author      :    g00209332
 created     :    2013/1/7
 description :    ����ǽ�Զ��ָ���������;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/1/7 ��ʼ�汾
*********************************************************************/
#pragma once
#include "sdktimer.h"

class CTVWallMgr;//lint !e763
class CAutoResumeTrigger : public ITrigger
{
public:
    CAutoResumeTrigger(void);
    ~CAutoResumeTrigger(void);
    virtual void onTrigger(void * /*pArg*/, ULONGLONG /*ullScales*/, TriggerStyle /*enStyle*/);
	//���õ���ǽ�������
	void SetTVWallMgr(CTVWallMgr* pTVWallMgr);
private:
	CTVWallMgr* m_pTVWallMgr;		//����ǽ�������
};
