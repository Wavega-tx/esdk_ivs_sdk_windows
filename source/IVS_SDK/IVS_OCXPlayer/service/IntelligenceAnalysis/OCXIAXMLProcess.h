/********************************************************************
filename 	: IAXMLProcess.h
author   	: z00227377
created 	: 2012/12/22
description : �켣�������XML
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/22 ��ʼ�汾
*********************************************************************/


#ifndef __OCX_IA_XML_PROCESS_H__
#define __OCX_IA_XML_PROCESS_H__

#include "ivs_xml.h"
#include "hwsdk.h"
#include "IVS_IATrackAPI.h"
#include "GlobalIAMessageMgr.h"



class CIAXMLProcess
{
private:
	CIAXMLProcess();
public:
	~CIAXMLProcess();

public:

	// �������ܷ�������켣����
	static IVS_INT32 GetAnalyzeResultBuildXML(const IA_REQ_PARAM &IAReqParam, CXml &ReqXML);

	// �������ܷ�������켣����
	static IVS_INT32 GetAnalyzeResultPraseXML(LPCTSTR pRspXML, IVS_ULONG &ulResultCode, IA_REQ_PARAM &IAReqParam);

	// �������ܷ����켣���ͱ���
	static IVS_INT32 TrackInfoContentPraseXML(LPCTSTR pTrackXml, IA_LAYOUT_MESSAGE &pIALayoutMessage, IA_REQ_PARAM &IAReqParameters);
	
	// ����ֹͣ���ܷ�������켣����
	static IVS_INT32 StopAnalyzeResultBuildXML(IVS_ULONG ulRequestID, IVS_ULONG ulRuleType, IVS_ULONG ulQueryType, const IVS_CHAR *CameraID, CXml &ReqXML);
	
	// ����ֹͣ���ܷ�������켣����
	static IVS_INT32 StopAnalyzeResultPraseXML(LPCTSTR pRspXML, IVS_ULONG &ulResultCode);

private:

	// �����ֱ��ʳ����ֶ�
	static IVS_INT32 PraseFrameSize(char *tempBuf, IA_LAYOUT_MESSAGE &pIALayoutMessage);

	// ������ɫ�ֶ�
	static IVS_INT32 PraseColor(char *tempBuf, COLORREF &ulColor);

	// �����������ֶ�
	static IVS_INT32 PrasePoint(char *tempBuf, POINT &point);

	// �������б��ֶ�
	static IVS_INT32 PrasePointList(char *tempBuf, POINT *PointList, ULONG &ulListPointsNum);

};


#endif // __OCX_IA_XML_PROCESS_H__
