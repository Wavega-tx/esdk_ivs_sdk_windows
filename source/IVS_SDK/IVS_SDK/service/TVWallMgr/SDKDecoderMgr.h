/********************************************************************
 filename    :    SDKDecoderMgr.h
 author      :    g00209332
 created     :    2013/1/18
 description :    SDK������������,�����̹߳����������������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/1/18 ��ʼ�汾
*********************************************************************/
#pragma once
#include "SDKDef.h"
#include "vos.h"
#include "hwsdk.h"
#include "IVS_SDKTVWall.h"

typedef struct 
{
	IVS_CHAR szDecoderID[IVS_TVWALL_DECODER_CODE_LEN];	//������ID
	IVS_CHAR szDecoderIP[IVS_IP_LEN];					//������IP
	IVS_USHORT usDecoderPort;							//�������˿�
	IVS_CHAR szUserName[IVS_NAME_LEN];					//��������½��
	IVS_CHAR szPWD[IVS_PWD_LEN];						//��½����
	IVS_ULONG ulIdentifyID;								//��½�ɹ����ID
	IVS_BOOL bOnline;									//�Ƿ�����
}SDK_DECODER_INFO;


enum LINK_MODE
{
	LINK_MODE_AUTO = 0,		// �Զ�
	LINK_MODE_MANUAL = 1,	// �ֶ�
	LINK_MODE_MIX = 2		// ���
};

class CSDKDecoderMgr;
typedef std::list<CSDKDecoderMgr*> SDK_DECODER_MGR_LIST;				//����������ʵ���б� 

class CSDKDecoderMgr
{
	typedef std::map<std::string, SDK_DECODER_INFO*> SDK_DECODER_INFO_MAP;	//��������ϢMAP
public:
	CSDKDecoderMgr(void);
	~CSDKDecoderMgr(void);
	// ��ʼ��������SDK
	IVS_INT32 InitSDK(PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, void* pUserData);
	// �ͷŽ�����SDK
	IVS_INT32 CleanupSDK();
	//// ���ý�������Ϣ
	//IVS_INT32 SetDecoderInfo(const SDK_DECODER_INFO* pDecoderInfo, IVS_UINT32 uiDecoderNum);
protected:
	// �̵߳��ú���
	static void invoke(void *argc);
	// SDK�ص�����
	static IVS_LONG __stdcall SDKEventCallBack(void *argc);
	// ��½������
	IVS_VOID  LoginToDecoder();
	// ��ȡ������ע����Ϣ
	IVS_INT32 GetDecoderRegInfo(IVS_ULONG /*ulID*/, const std::string& /*strDecoderID*/, 
								const std::string& /*strDecoderIP*/, 
								CUMW_NOTIFY_REMOTE_DECODER_STATUS* pRegInfo) const;
private:
	// �Ƿ�����õ�¼��Ϣ������
	IVS_BOOL IsIncludeID(IVS_ULONG ulIdentifyID);
	// ����һ���������������
	static IVS_VOID AddDecMgrObj(CSDKDecoderMgr* pDecoder);
	// �Ƴ�һ���������������
	static IVS_VOID RemoveDecMgrObj(const CSDKDecoderMgr* pDecoder);
	// ����֪ͨ
	IVS_VOID NotifyDisConnect(IVS_ULONG ulIdentifyID);
	// �Ƴ���������Ϣ
	IVS_VOID RemoveDecoderInfo();
	PCUMW_CALLBACK_NOTIFY m_pCallBackFun;	// �ص�����
	void* m_pUserData;						// �û�����
	SDK_DECODER_INFO_MAP m_decoderInfoMap;	// �����õĽ�������Ϣ
	VOS_Mutex* m_pDecoderInfoMapMutex;		// ��������ϢMap��
	VOS_Thread* m_pVosThread;				// �̶߳���
	IVS_BOOL m_bThreadExit;					// �߳��˳�
	time_t m_lLastLoginTime;    		    // �ϴε�¼ʱ��
	IVS_BOOL m_bInit;						// �Ƿ��ʼ����
	static SDK_DECODER_MGR_LIST m_DecMgrObjList;// ����������ʵ���б�SDK�ص���������ȷ���ĸ�ʵ����ֻ���þ�̬��������
};
