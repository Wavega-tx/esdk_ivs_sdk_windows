#pragma once
#include <afx.h>
#include <list>
#include "IVS_SDKTVWall.h"
#include "SDKDef.h"
#include "vos.h"

//����ǽ����ṹ
typedef struct  
{
	//IVS_CHAR szChannelCode[IVS_TVWALL_DECODER_CODE_LEN];//�󶨵�ͨ������
	unsigned long ulChannelID;							//�󶨵�ͨ������
	IVS_CHAR szChannelName[IVS_TVWALL_DECODER_CODE_LEN];//ͨ������
	IVS_UINT32 uiChannelType;
}TVWALL_PANE_CHANNEL;
typedef std::list<TVWALL_PANE_CHANNEL> TVWALL_PANE_CHANNEL_LIST;
typedef struct  
{
	IVS_ULONG ulWndID;									//����ID
    // add by wanglei 00165153:2013.7.26 DTS2013071507953
    // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
    // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
    // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
    // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
    IVS_ULONG ulPaneName;                              // �ñ�ʶ����סԭ�ȵĴ���ţ��Ա㽨��������ϵ
	IVS_CHAR szDecoderID[IVS_TVWALL_DECODER_CODE_LEN];	//�󶨵Ľ�����ID
	//IVS_UINT32 uiChannel;								//�󶨵�ͨ��
	CRect rect;											//����λ��
	IVS_UINT32 uiChannelNum;							//����󶨵�ͨ����
	TVWALL_PANE_CHANNEL_LIST listChanel;				//ͨ��list
}TVWALL_PANE;
typedef std::list<TVWALL_PANE> TVWALL_PANE_LIST;

//����ǽ����ͨ�������ⴰ��ID
typedef struct
{
	ULONG ulParentWndID;
	ULONG ulChildWndID;
}TVWall_VPANEID;//����ǽ���ⴰ��
typedef std::list<TVWall_VPANEID> TVWall_VPANEID_LIST;

class COCXTVWallXMLProcess
{
public:
	~COCXTVWallXMLProcess(void);

	static COCXTVWallXMLProcess& GetInst();

	// ��ȡ����ǽ�б�
	IVS_VOID GetTVWallList(CString &strResult)const;
	// ��ȡ����ǽ��Ϣ
	IVS_VOID GetTVWallByID(IVS_UINT32 uiTVWallID, CString& strResult)const;
	// ���ӵ���ǽ
	CString AddTVWall(const CString& strTVWall);
	// ɾ������ǽ
	IVS_INT32 DeleteTVWall(IVS_ULONG uiTVWallID);
	// ���ĵ���ǽ��Ϣ
	IVS_INT32 ModifyTVWallInfo(const CString& strResult);
	// ���ĵ���ǽ����
	IVS_INT32 ModifyTVWallLayout(const CString& strResult);
	// ���ݴ��ںŻ�ȡ��������ͨ����
	IVS_INT32 GetDecoderIDAndChannelNo(IVS_ULONG ulTVWallID, const TVWall_VPANEID& vPaneID, IVS_TVWALL_PARAM& stTVWallParam);
	//������ע����ϢXML
	IVS_INT32 GetRemoteDecoderRegXML(const CUMW_NOTIFY_REMOTE_DECODER_STATUS& stDecoderInfo, char* pXmlBuf, UINT bufLen)const;
	//������������ϢXML
	IVS_INT32 GetRemoteDecoderEndXML(const CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO& stInitInfo, char* pXmlBuf, UINT bufLen)const;
    //��Ƶ�����첽֪ͨ
    IVS_INT32 GetNotifyAsyncRetXML(const CUMW_NOTIFY_ASYNC_RET_EX& stAsyncRet, char* pXmlBuf, UINT bufLen)const;
    //����ʵʱ�������XML
    IVS_INT32 RealPlayParamParseXML(const IVS_CHAR* pRealplayParam, IVS_REALPLAY_PARAM& RealplayParam)const;
    //����ƽ̨�طŲ���XML
    IVS_INT32 PlatformPlaybackParamParseXML(const IVS_CHAR* pPlaybackParam, IVS_PLAYBACK_PARAM& PlaybackParam)const;
	//��ȡ��������Ϣ�б������б�
	IVS_INT32 GetDecoderInfoList(CUMW_NOTIFY_REMOTE_DECODER_STATUS* pDecoderInfoList, IVS_UINT32 uiDecoderNum, CString& strResult)const;
	//���ݵ���ǽ��Ż�ȡ������Ϣ
	IVS_INT32 GetTVWallPaneList(IVS_ULONG ulTVWallID, TVWALL_PANE_LIST& paneList, IVS_UINT32& uiHonCount, IVS_UINT32& uiVerCount)const;
    //��ȡ����ǽ�ط��쳣��XML
    IVS_INT32 GetRemotePlayBackXML(int iWinID, unsigned long ulErrorCode, char* pXmlBuf, UINT bufLen)const;
    //��ȡ���е���ǽID
    IVS_INT32 GetTVWallIdList(std::list<int>& listTVWallId)const;
    //��ʼ��ǽ���첽֪ͨ
    IVS_INT32 GetNotifyStartRealPlayTVWallRetXML(ULONG ulTVWallID, ULONG ulWndID, IVS_INT32 iErrCode, ULONG ulTransID, char* pXmlBuf, UINT bufLen)const;
	//��������������ļ�
	IVS_INT32 GetNetKeyBoardConfig(IVS_INT32& iMin, IVS_INT32& iMax)const;
private:
	COCXTVWallXMLProcess(void);

	/**********************************************************************
	* name			: MakeSureTVWallDirExist
	* description	: ������ǽ���õ��ļ��У�Ĭ��·����app data/ivs/tvwall/
	* input			: NA
	* output     	: NA
	* return     	: std::string - �ļ��е�Ŀ¼
	* remark     	: NA   
	*********************************************************************/
	std::string MakeSureTVWallDirExist()const;
	IVS_INT32 AddTVWall(IVS_INT32& iNewID, const CString& strTVWall, CXml& xml)const;
	IVS_INT32 DeleteTVWall(IVS_ULONG uiTVWallID, CXml& xml)const;
	IVS_INT32 ModifyTVWallInfo(const CString& strResult, CXml& xml)const;
	IVS_INT32 ModifyTVWallLayout(const CString& strResult, CXml& xml)const;
	IVS_INT32 GetTVWallByID(IVS_UINT32 uiTVWallID, CString& strResult, CXml& xml)const;

	//���ݴ���ID����ʱ������ȡ��Ӧ�Ľ�������ͨ��
	IVS_INT32 GetDecoderIDAndChannelNo(IVS_ULONG ulTVWallID, const TVWall_VPANEID& vPaneID, IVS_TVWALL_PARAM& stTVWallParam, CXml& xmlTVWall)const;
	//��ȡ���ӵ���ǽ�ķ���XML
	CString GetAddTVWallResultXML(IVS_INT32 iTVWallID, IVS_INT32 iRet)const;
	//����Layout�ŵ�list��
	IVS_INT32 ParesTVWallLayoutXML2List(CXml& xml, TVWALL_PANE_LIST& paneList, IVS_UINT32& urHonCount, IVS_UINT32& uiVerCount)const;
	// ����һ���������ڵ�
	IVS_VOID AddDecoderNode(CXml& xml, const CUMW_NOTIFY_REMOTE_DECODER_STATUS& stDecoderInfo, bool bChild, bool bOnline)const;

private:
	//��д����ǽXML�����ļ�����
	VOS_Mutex *m_TVWallXmlMutex;    

	//�������ڴ�������еĵ���ǽXML������Ϣ
	TVWALL_PANE_LIST m_paneList;

	//�Ƿ������ǽ��XML������Ϣ�����ڴ���
	bool m_bSaveinMem;
};

