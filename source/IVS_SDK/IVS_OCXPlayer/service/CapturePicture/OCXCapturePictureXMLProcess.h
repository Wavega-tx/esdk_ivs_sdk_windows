/********************************************************************
	filename	: 	OCXCapturePictureXMLProcess.h
	author		:	songjianfeng(00193168)
	created		:	2013/1/23	
	description	:	����ͼƬץ�ĵ�XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/1/23 ��ʼ�汾
*********************************************************************/
#ifndef __OCX_CAPTUREPICTURE_XML_PROCESS_H__ 
#define __OCX_CAPTUREPICTURE_XML_PROCESS_H__

#include "OCXDatatype.h"
#include "SDKDef.h"

class OCXCapturePictureXMLProcess
{
private:
	OCXCapturePictureXMLProcess();
public:
	~OCXCapturePictureXMLProcess();

public:


	/******************************************************************
	function : SetSnapshotPlanGetXML
	description: ����ƽ̨ץ�ļƻ�:������XMLת��Ϊ�ṹ��;
	input : pSnapShotPlan,ץ�ļƻ���Ϣ;pCameraCode����������;xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 SetSnapshotPlanGetXML(IVS_CHAR* pCameraCode,IVS_SNAPSHOT_PLAN* pSnapShotPlan, CXml &xml);

	/******************************************************************
	function : GetSnapshotPlanGetXML
	description: ��ѯƽ̨ץ�ļƻ�:����Ӧ�ṹ��ת��Ϊ���XML;
	input : pCameraCode,�������ţ�pSnapShotPlan����Ӧץ�ļƻ���Ϣ�ṹ�壻iRetCode������룻xml����Ӧxml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetSnapshotPlanGetXML(IVS_INT32 iRet,const IVS_CHAR* pCameraCode,const IVS_SNAPSHOT_PLAN* pSnapShotPlan,CXml &xml);
	
	/*****************************************************************
	     function   : GetSnapshotListParseXml
	     description: ��ȡͨ����
	     output     : CXml & xml    ץ��ͼƬ��ѯ����xml
	     output     : IVS_QUERY_SNAPSHOT_PARAM * pQueryParam  ץ��ͼƬ��ѯ����
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 GetSnapshotListParseXml(CXml &xml, IVS_QUERY_SNAPSHOT_PARAM *pQueryParam, const IVS_CHAR* pCameraCode);

	/*****************************************************************
	     function   : GetSnapshotListGetXml
	     description: ƴװץ��ͼƬ�澯��Ϣxml
	     output     : CXml & xml       ץ��ͼƬ�б���Ϣxml
	     output     : const IVS_SNAPSHOT_INFO_LIST * pSnapshotList   ץ��ͼƬ�б���Ϣ
	     input      : const IVS_CHAR * pCameraCode  ���������
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 GetSnapshotListGetXml(CXml &xml, const IVS_SNAPSHOT_INFO_LIST *pSnapshotList, IVS_UINT32 iTotalNum);
	
	/*****************************************************************
	     function   : LocalCaptureParseClientConfig
	     description: ��������ץ�Ĳ����ͻ�������xml
	     input      : LPCTSTR					pXmlStr				
	     output     : IVS_LOCAL_CAPTURE_PARAM	&LocalCaptuParam   ����ץ�Ĳ���
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 LocalCaptureParseClientConfig(LPCTSTR pXmlStr, IVS_LOCAL_CAPTURE_PARAM &LocalCaptuParam);
	
	/*****************************************************************
	     function   : LocalCaptureBuildXMLGeneric
	     description: ƴװ����ץ�Ĳ����ͻ�������xml
	     input      : LPCTSTR					pXmlStr				
	     output     : IVS_LOCAL_CAPTURE_PARAM	&LocalCaptuParam   ����ץ�Ĳ���
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 LocalCaptureBuildXMLGeneric(LONG iEventCode, ULONG iWndID, const IVS_CHAR* pFileName, CXml &xml);

};
#endif

