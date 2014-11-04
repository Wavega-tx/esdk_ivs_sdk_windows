#pragma once
#include "decoderchannel.h"

class CRDAChannel :
	public CDecoderChannel
{
public:
	CRDAChannel(IVS_ULONG ulAsyncSeq, 
				const IVS_CHAR* pDecoderID, 
				IVS_UINT32 uiChannel,
				CTVWallMgr* pTVWallMgr);
	~CRDAChannel(void);
protected:
	// ��ʼ��ͨ��
	virtual IVS_INT32 InitChannel(IVS_UINT32 uiProtocolType, MEDIA_ADDR& stDecoderAddr);
	// ����ͨ����ʼ����
	virtual IVS_INT32 StartChannel(const std::string strURL, const MEDIA_ADDR stSourceMediaAddr);
	// ����ͨ������
	virtual IVS_INT32 StopChannel();
	// ����ͨ������
	virtual IVS_INT32 StartChannelSound();
	// ֹͣͨ������
	virtual IVS_INT32 StopChannelSound();
	// ����ͨ����������
	virtual IVS_INT32 SetChannelVolume(IVS_UINT32 /*uiVolumeValue*/);
private:
	CRDAChannel();
	IVS_ULONG m_ulBusinessID;				// ������ҵ��ID
	IVS_UCHAR m_ucTransportProtocol;		// ������ʹ�õ�Э��
};
