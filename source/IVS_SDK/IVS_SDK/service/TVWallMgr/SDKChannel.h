#pragma once
#include "decoderchannel.h"

class CSDKChannel :
	public CDecoderChannel
{
public:
	CSDKChannel(IVS_ULONG ulAsyncSeq, 
				const IVS_CHAR* pDecoderID, 
				IVS_UINT32 uiChannel,
				CTVWallMgr* pTVWallMgr);
	~CSDKChannel(void);
protected:
	CSDKChannel();
	// ��ʼ��ͨ��
	virtual IVS_INT32 InitChannel(IVS_UINT32 /*uiProtocolType*/, MEDIA_ADDR& /*stDecoderAddr*/){return 1;}
	// ����ͨ����ʼ����
	virtual IVS_INT32 StartChannel(const std::string strURL, const MEDIA_ADDR stSourceMediaAddr){return 1;} //lint !e715 !e830
	// ����ͨ������
	virtual IVS_INT32 StopChannel(){return 1;}
	// ����ͨ������
	virtual IVS_INT32 StartChannelSound(){return 1;}
	// ֹͣͨ������
	virtual IVS_INT32 StopChannelSound(){return 1;}
	// ����ͨ����������
	virtual IVS_INT32 SetChannelVolume(IVS_UINT32 /*uiVolumeValue*/){return 1;}
};

