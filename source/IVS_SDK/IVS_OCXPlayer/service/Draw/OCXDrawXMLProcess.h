/********************************************************************
filename 	: OCXDrawXMLProcess.h
author   	: z00227377
created 	: 2012/12/25
description : ��ͼXML
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/25 ��ʼ�汾
*********************************************************************/


#ifndef __OCX_DRAW_XML_PROCESS_H__ 
#define __OCX_DRAW_XML_PROCESS_H__

#include "SDKDef.h"
#include "IVS_IADrawAPI.h"




class COCXDrawXMLProcess
{
private:
	COCXDrawXMLProcess();
public:
	~COCXDrawXMLProcess();

public:
	// ��������ͼ�ε�XML
	static IVS_INT32 SetGraphicsPraseXML(IA_GRAPHICS *pGraphicsArr, ULONG &ulNum, LPCTSTR pGraphicsXml, const SIZE &szGraphic, BOOL bPersent);

	// ������ȡͼ�ε�XML
	static IVS_INT32 GetGraphicsGetXML(const IA_GRAPHICS *pGraphicsArr, IVS_INT32 iResultCode, ULONG ulNum, CXml& xmlGraphics, const SIZE &szGraphic);

	// ������ȡͼ��ʧ�ܵ�XML
	static IVS_INT32 GetGraphicsGetErrXML(IVS_INT32 iResultCode, CXml& xmlGraphics);
	
	// ������ͼǰͼ����������XML
	static IVS_INT32 DrawGraphicsPraseXML(IA_GRAPHICS &GraphicAttribute, ULONG &ulGroupDrawMaxNum, LPCTSTR pGraphicsXml);

	// ����ѡ��ͼ����ϢXML
	static IVS_INT32 SelectGraphicGetXML(CString &xmlToCU, IVS_ULONG ulGraphicGroup);
	
	// ����ȥѡ��ͼ����ϢXML
	static IVS_INT32 UnSelectGraphicGetXML(CString &xmlToCU);

};
#endif //__OCX_DRAW_XML_PROCESS_H__
