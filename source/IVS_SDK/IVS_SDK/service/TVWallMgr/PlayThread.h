/********************************************************************
 filename    :    PlayThread.h
 author      :    g00209332
 created     :    2013/1/8
 description :    �����߳���;���󲥷�ʱ���������߳̽��д�����ֹ��������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/1/8 ��ʼ�汾
*********************************************************************/
#pragma once
#include "vos.h"
#include "hwsdk.h"
#include "DecoderChannel.h"
#include "TVWallMgr.h"

class CPlayThread
{
public:
    CPlayThread(void);
    virtual ~CPlayThread(void);
    IVS_INT32 run();
	// ���������Code
    void SetCameraCode(const IVS_CHAR* pCameraCode);
	// ���ý���������ͨ��
    void SetDecoderChannel(CDecoderChannel* pCh);
	// ���õ���ǽ�������
	void SetTVWallMgr(CTVWallMgr* pTVWallMgr);
	// ���õ���ǽ����
    void SetTVWallParam(const IVS_TVWALL_PARAM* pTVWallParam);
protected:
	//��ʼ������Ƶ��ʵ�����طţ�
    virtual IVS_INT32 StartPlay() = 0;
    IVS_CHAR m_szCameraCode[IVS_DEV_CODE_LEN+1];	//�����Code
    IVS_TVWALL_PARAM m_stTVWallParam;				//����ǽ����
    CDecoderChannel* m_pChannel;					//������ͨ������
	CTVWallMgr* m_pTVWallMgr;						//����ǽ�������
private:
	//�̵߳��ú���
    static void invoke(void *argc);					
    VOS_Thread* m_pVosThread;						//�̶߳���
};

class CRealPlayThread : public CPlayThread
{
public:
    CRealPlayThread(void);
    virtual ~CRealPlayThread(void);
	// ����ʵ������
    void SetRealPlayParam(const IVS_REALPLAY_PARAM* pRealPlayParam);
protected:
	//��ʼ����ʵ��
    virtual IVS_INT32 StartPlay();
private:
    IVS_REALPLAY_PARAM m_stRealPlayParam;			//ʵ������
};

class CPlayBackThread : public CPlayThread
{
public:
    CPlayBackThread(void);
    virtual ~CPlayBackThread(void);
protected:
	//��ʼ���Żط�
    virtual IVS_INT32 StartPlay();
private:
    IVS_PLAYBACK_PARAM m_stRealPlayParam;			//�طŲ���
};
