/********************************************************************
	filename	: 	OCXRealPlayXMLProcess.h
	author		:	songjianfeng(00193168)
	created		:	2012/12/03	
	description	:	����ý����Ƶ�طŵ�XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/
#ifndef __OCX_REALPLAY_XML_PROCESS_H__ 
#define __OCX_REALPLAY_XML_PROCESS_H__

#include "OCXXmlProcess.h"
#include "OCXDatatype.h"
#include "SDKDef.h"

class COCXRealPlayXMLProcess
{
private:
	COCXRealPlayXMLProcess();
public:
	~COCXRealPlayXMLProcess();

public:
	/******************************************************************
	function : GetMediaParamXML
	description: ��ȡý�������Ϣ;
	input : pMediaParamXml,ý�������ϢXML��mediaParam��ý�������Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static LONG GetMediaParamXML(LPCTSTR pRealPlayParamXml, IVS_REALPLAY_PARAM &realPlayParam);

    /******************************************************************
	function : EnableGPUParseClientConfig
	description: ��ȡCU�����ļ��е�ý�����;
	input : pMediaParamXml,ý�������ϢXML��mediaParam��ý�������Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static LONG EnableGPUParseClientConfig(LPCTSTR pParamXml, BOOL& bEnableGPU);


	static LONG BuildAsyncRealPlayXml(CXml &xml, long winID, long result, unsigned long transid);



};
#endif

