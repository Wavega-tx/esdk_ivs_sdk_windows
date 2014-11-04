/********************************************************************
	filename	: 	OCXLogMgrXMLProcess.h
	author		:	songjianfeng(00193168)
	created		:	2012/12/03	
	description	:	����ý����Ƶ�طŵ�XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/
#ifndef __OCX_PLAYBACK_XML_PROCESS_H__ 
#define __OCX_PLAYBACK_XML_PROCESS_H__

#include "OCXXmlProcess.h"
#include "OCXDatatype.h"
#include "SDKDef.h"

/*
 * ͬ���طŲ����ṹ��
 */
typedef struct {
	time_t beg;
	time_t end;
} TIME_INFO;

typedef std::list<TIME_INFO>     TIME_SPAN_LIST;
typedef TIME_SPAN_LIST::iterator TIME_SPAN_ITER;

class CPlayInfo
{
public:
	unsigned long  ulWndID;
	char           CameraCode[IVS_DEV_CODE_LEN + 1];
	TIME_SPAN_LIST m_TimeList;
};

typedef std::list<CPlayInfo> PLAY_INFO_LIST;
typedef PLAY_INFO_LIST::iterator PLAY_INFO_ITER;

class SyncPlayInfo
{
public:
	unsigned int    m_uProto;
	PLAY_INFO_LIST  m_PlayInfoList;
};

class CRecordSnap
{
public:
	CRecordSnap ()
 	{
		RecordType = 0;
		begine     = 0;
		end        = 0;
		memset(CamCode, 0, sizeof(CamCode));
		memset(NvrCode, 0, sizeof(NvrCode));
		memset(MbuDomain, 0, sizeof(MbuDomain));
		memset(FileName, 0, sizeof(FileName));
		memset(FilePath, 0, sizeof(FilePath)); 
		memset(&RecTimeSpan,0,sizeof(IVS_TIME_SPAN));
	}//lint !e1401
	~CRecordSnap(){};

	int  RecordType;
	char CamCode[IVS_DEV_CODE_LEN + 1];
	char NvrCode[IVS_DEV_CODE_LEN + 1];
	char MbuDomain[IVS_DEV_CODE_LEN + 1];
	char FileName[256];
	char FilePath[256];
	IVS_TIME_SPAN  RecTimeSpan;
	time_t  begine;
	time_t  end;
	std::vector<time_t> SnapPoint;
};

typedef struct 
{
	char FileName[256];
	char SnapTime[IVS_TIME_LEN];
} RECORD_SNAP_NODE;

typedef std::vector<RECORD_SNAP_NODE> SNAP_VEC;
typedef SNAP_VEC::iterator            SNAP_VEC_ITER;


class COCXPlayBackXMLProcess
{
private:
	COCXPlayBackXMLProcess();
public:
	~COCXPlayBackXMLProcess();

public:
	/******************************************************************
	function : GetMediaParamXML
	description: ��ȡý�������Ϣ;
	input : pMediaParamXml,ý�������ϢXML��mediaParam��ý�������Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static LONG GetMediaParamXML(LPCTSTR pMediaParamXml, IVS_MEDIA_PARA &mediaParam);


	/******************************************************************
	function : GetPlayInfoEventMsgXML
	description: ��ѯý�岥����Ϣ�ϱ�XML;
	input : xmlToCU,�ϱ�xml��Ϣ��ulWinID������ID��mediaInfo��ý����Ϣ��XY��������Ϣ��resultcode�������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static void GetPlayInfoEventMsgXML(CString &xmlToCU, ULONG ulWinID,IVS_MEDIA_INFO &mediaInfo, CPoint& XY,IVS_INT32 resultcode);

	/******************************************************************
	function : GetWndIDXML
	description: ��ѯ����ID��XML;
	input : xmlToCU,�ϱ�xml��Ϣ��ulWinID������ID
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static void GetWndIDXML(CString &xmlToCU,ULONG ulWndID);

	/******************************************************************
	function : GetPlayBackTime
	description: ��ѯ�ط�ʱ��
	input : stPlaybackTime,�ط�ʱ����Ϣ��iResultCode����������룻xmlTime��ʱ��XML��
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static IVS_INT32 GetPlayBackTime(const IVS_PLAYBACK_TIME& stPlaybackTime, IVS_INT32 iResultCode, CXml& xmlTime);

	/******************************************************************
	 function   : GetPlayBackSpeed
	 description: ��ȡ�ط��ٶ�;
	 input      : float fSpeed			�����ٶ�;
	 input      : IVS_INT32 iResultCode	������;
	 output     : CXml & xmlSpeed		���ظ�CU��xml;
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetPlayBackSpeed(float fSpeed, IVS_INT32 iResultCode, CXml& xmlSpeed);

	/******************************************************************
	 function   : GetPlayBackStatus
	 description: ��ȡ�ط�״̬;
	 input      : IVS_INT32 iStatus		����״̬;
	 input      : IVS_INT32 iResultCode	������;
	 output     : CXml & xmlStatus		���ظ�CU��xml;
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetPlayBackStatus(IVS_INT32 iStatus, IVS_INT32 iResultCode, CXml& xmlStatus);

	/******************************************************************
	function : GetStreamExceptionMsgXML
	description: ��������쳣�ϱ�XML;
	input :  xmlToCU,�ϱ�xml��Ϣ��ulWinID������ID��iExceptionCode���쳣�룻iReseved1�������ֶ�
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static void GetStreamExceptionMsgXML(CString &xmlToCU, ULONG ulWinID, int iExceptionCode, int iReseved1);

	/******************************************************************
	function : GetPlaybackParam
	description: ��ûطŲ���;
	input : pPlayback,��������xml��stPlaybackParam���طŲ�����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static IVS_INT32 GetPlaybackParam(const char* pPlayback, IVS_PLAYBACK_PARAM& stPlaybackParam);

	/******************************************************************
	function : ParsePlayBackSync
	description: ����ͬ���طŽṹ�����;
	input : pPlayback,��������xml��stPlaybackParam���طŲ�����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 ParsePlayBackSync(const char *szReqXml, SyncPlayInfo &syncInfo);

	/******************************************************************
	function : ParseLocalPlayBackParam
	description: ��������¼��طŽṹ��;
	input : pPlayback,��������xml��stPlaybackParam���طŲ�����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 ParseLocalPlayBackParam(const char *szReqXml, IVS_LOCAL_PLAYBACK_PARAM &ParamInfo);
	
	/******************************************************************
	function : ParseLocalPlayBackParam
	description: ��������¼��طŽṹ��;
	input : pPlayback,��������xml��stPlaybackParam���طŲ�����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 PraseRecordSnapParam(const char *szReqXml, CRecordSnap &SnapInfo);

	static IVS_INT32 BuildRecordSnapXml(CXml &xml, IVS_INT32 result, SNAP_VEC &SnapList);

	/******************************************************************
	function : GetKeyEventXML
	description: ��ȡ���̰�����Ϣ
	input : xmlToCU,�ϱ�xml��Ϣ��ulWinID��ѡ�д���ID ulKeyType:������ϢID
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static void GetKeyEventXML(CString &xmlToCU,ULONG ulWndID,ULONG ulKeyType);

    /******************************************************************
	function : GetTalkbackParamXML
	description: ��ȡ�����Խ�������Ϣ;
	input : pTalkbackParamXml,�����Խ�������ϢXML��
            talkbackParam�������Խ�������Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static LONG GetTalkbackParamXML(LPCTSTR pTalkbackParamXml, IVS_VOICE_TALKBACK_PARAM &talkbackParam);

    //���׻�ȡý����Ϣ�¼�����I��ť����
	static void GetMediaInfoXML(CString &xmlToCU,ULONG ulWndID,CPoint& XY);
};
#endif
