/********************************************************************
	filename	: 	OCXXMLProcess.h
	author		:	z00193167
	created		:	2012/12/03	
	description	:	XML���ö���
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/
#ifndef __OCX_XML_PROCESS_H__
#define __OCX_XML_PROCESS_H__

#include <afx.h>
#include "QueryFiled.h"
#include "Utility.h"
#include "ToolsHelp.h"
#include "SDKDef.h"


#define QUERYFILED CQueryFiled::GetInstance()


/* 
*  ��ȡ�����Ϣ-�ַ�
*  elem        �����ҵ�Ԫ�أ���"DEV_INFO"
*  szXMLValue  ��Ž��Ԫ�ص�ָ��
*  NodeInfo    ������Ԫ�ص�ֵ
*  iMaxLen     �����������ֵ
*  xml         ������XML
*/
#define GET_ELEM_VALUE_CHAR(elem, szXMLValue, NodeInfo, iMaxLen, xml)                  \
	szXMLValue = NULL      ;                                                            \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
	{                                                                                  \
	(IVS_VOID)CToolsHelp::Memcpy((char *)NodeInfo, iMaxLen, szXMLValue, strlen(szXMLValue));  \
	}                                                                                  \
}

/* 
*  ��ȡ�����Ϣ-��ֵ
*  elem        �����ҵ�Ԫ�أ���"DEV_INFO"
*  szXMLValue  ��Ž��Ԫ�ص�ָ��
*  NodeInfo    ������Ԫ�ص�ֵ
*  xml         ������XML
*/
#define GET_ELEM_VALUE_NUM(elem, szXMLValue, NodeInfo, xml)                            \
	NodeInfo = 0 ;                                                                   \
	if (xml.FindElem(elem))                                                                \
{			                                                                            \
	NodeInfo = 0;                                                                                \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = atoi(szXMLValue);                                                    \
   }                                                                                   \
}

#define GET_ELEM_VALUE_NUM_FOR_UINT(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = static_cast<IVS_UINT32>(atoi(szXMLValue));                                                    \
   }                                                                                   \
}

#define GET_ELEM_VALUE_FLOAT(elem, szXMLValue, NodeInfo, xml)                                 \
    NodeInfo = 0;                                                                                        \
    if (xml.FindElem(elem))                                                                \
{																	                   \
    szXMLValue = xml.GetElemValue();                                                   \
    if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = static_cast<IVS_FLOAT>(atof(szXMLValue));                                                    \
   }                                                                                   \
}

#define GET_ELEM_VALUE_DOUBLE(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = static_cast<IVS_DOUBLE>(atof(szXMLValue));                                                    \
   }                                                                                   \
}


class COCXXmlProcess
{
public:
	/******************************************************************
	function : GetVideoNumAndResourceXML
	description: ��ѯMU����ԴԤ���Ƿ�ﵽ���޼���ǰ�������Ƶ���Ƿ�����
	input :  iRet ResultCode
			 bReachLimit �Ƿ�ﵽ����
	         bWhetherCited �Ƿ�����
	output : strOutXml ����XML
	return : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetVideoNumAndResourceXML(IVS_INT32 iRet,IVS_BOOL bReachLimit,IVS_BOOL bWhetherCited, std::string& strOutXml);

	/******************************************************************
	function : GetUnifiedFormatQueryParseXML
	description: ͨ�õĲ�ѯxml���ڵ������֮��Ҫɾ����pReqBuffer(IVS_DELETE(pReqBuffer, MUTII))
	input :  CXml & xmlReq
	         IVS_INT32 iFiedNum
	output : IVS_VOID *  & pReqBuffer
	return : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetUnifiedFormatQueryParseXML(IVS_VOID* &pReqBuffer,CXml& xmlReq,IVS_INT32 iFiedNum);

    /******************************************************************
	function : GetUnifiedFormatQueryParseXML
	description: ͨ�õĲ�ѯxml���ڵ������֮��Ҫɾ����pReqBuffer(IVS_DELETE(pReqBuffer, MUTII))
	input :  CXml & xmlReq
	         IVS_INT32 iFiedNum
	output : IVS_VOID *  & pReqBuffer
    output : IVS_CHAR *  & pDomainCode �����
	return : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetUnifiedFormatQueryParseXMLEx(IVS_VOID* &pReqBuffer,CXml& xmlReq,IVS_INT32 iFiedNum,IVS_CHAR* pDomainCode);

	
	/*****************************************************************
	     function   : GetAlarmErrString
	     description: ��ѯ�澯�����б��ص�xml
		 intput     : IVS_INT32 iRet
	     output     : CString & strResult  ���صı���
	     return     : NA
	*****************************************************************/
	static IVS_VOID GetAlarmErrString( CString &strResult, IVS_INT32 iRet );

	/*****************************************************************
	     function   : AlloceQueryStructMemory
	     description: ����ͨ�ø�ʽ�ڴ��С
	     output     : IVS_VOID *  & pReqBuffer �����ڴ��ָ��
	     output     : IVS_INT32 iFieldNum      �����ڴ��ø���
	     return     : �ɹ�����0��ʧ�ܷ�����Ӧ�Ĵ�����
	*****************************************************************/
	static IVS_INT32 AlloceQueryStructMemory( IVS_VOID* &pReqBuffer, IVS_INT32 iFieldNum );

	/*****************************************************************
	     function   : GetIsUp
	     description: �ж��Ƿ�Ϊ����
	     intput     : const IVS_CHAR * pQueryUp  ��Ҫ�Ƚϵ��ַ���
	     return     : �� ����1  �� ����0
	*****************************************************************/
	static IVS_BOOL GetIsUp(const IVS_CHAR* pQueryUp);

	/*****************************************************************
	     function   : GetQueryType
	     description: �ж��Ƿ�Ϊ��ȷ
	     output     : const IVS_CHAR * pQueryType  ��Ҫ�Ƚϵ��ַ���
	     return     : �� ����1  �� ����0
	*****************************************************************/
	static IVS_BOOL GetQueryType(const IVS_CHAR* pQueryType);
	
	/*****************************************************************
	     function   : GetErrString
	     description: ƴװ�������xml
	     output     : CString & strResult ���ظ�CU�ı���
	     input      : IVS_INT32 iRet  ������
	     return     : NA
	*****************************************************************/
	static IVS_VOID GetErrString( CString &strResult, IVS_INT32 iRet );

	/*****************************************************************
	     function   : GetErrorResultXML
	     description: ƴװ�������xml
	     input      : IVS_INT32 iRetCord  ������
	     output     : CXml & xml ƴװ���xml
	     return     : 
	*****************************************************************/
	static IVS_VOID GetErrorResultXML(IVS_INT32 iRetCord, CXml &xml);

	/*****************************************************************
	     function   : GetFieldNum
	     description: ��ȡ��ѯ�ֶεĸ���
	     input      : CXml & xmlReq  ��Ҫ���ҵı���
	     return     : ��ѯ���ĸ���
	*****************************************************************/
	static IVS_INT32 GetFieldNum(CXml &xmlReq);

	/*****************************************************************
	     function   : GetResultXML
	     description: ƴװ�����뵽xml
	     input      : IVS_INT32 & iRet ������
	     input      : const IVS_CHAR * pRspXml
	     output     : CString & strResult ƴװ���xml
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 GetResultXML(IVS_INT32& iRet, const IVS_CHAR* pRspXml, CString& strResult);

	/*****************************************************************
	     function   : GetElemCount
	     description: ��ȡָ��·�������ڵ����ƣ��ڵ���
	     input      : const IVS_CHAR * pElemPath �ڵ�·��
		 input      :  CXml & xml                xml����
	     output     : IVS_UINT32 & uiNum         �ڵ����
	     return     : �ɹ�����0��ʧ�ܷ���1
	*****************************************************************/
	static IVS_UINT32 GetElemCount(const IVS_CHAR* pElemPath,IVS_UINT32 &uiNum, CXml &xml);
	
	/*****************************************************************
	     function   : ParseDevCode
	     description: ����CU��������CODE
	     input      : const IVS_CHAR * pDevBigCode  CU��������CODE
	     output     : IVS_CHAR * pDevCode           ��������豸����
	     output     : IVS_CHAR * pDomainCode        ������������
	     return     : �ɹ�����0��ʧ�ܷ�����Ӧ�Ĵ�����
	*****************************************************************/
	static IVS_INT32 ParseDevCode(const IVS_CHAR *pDevBigCode, IVS_CHAR *pDevCode, IVS_CHAR* pDomainCode );
    
	/******************************************************************
    function : GetXmlDevinfoNum
    description: ��ѯ�豸�����ĸ���
    input : CXml & xml
    return : ���ؽڵ�ĸ���
    *******************************************************************/
    static IVS_UINT32 GetXmlDevinfoNum(CXml &xml);

	/*****************************************************************
	     function   : AddResultCodeUnderContent
	     description: ��content�ڵ������һ��resultcode�ڵ�
	     input      : iResultCode  �����
	                  strInXml ����XML
	     output     : strOutXml ���ƴװ���
	     return     : �ɹ�����0��ʧ�ܷ�����Ӧ�Ĵ�����
	*****************************************************************/
	static IVS_INT32 AddResultCodeUnderContent(IVS_INT32 iResultCode, const std::string& strInXml, std::string& strOutXml);

	// ת��int���͵�BOOL
	static bool TransIntToBool(unsigned int iRet);

	// ת��0,1���ִ�ON,OFF
	static std::string TransIntToOnOff(unsigned int iRet);

	// ת��ON,OFF��0,1��
	static unsigned int TransOnOffToInt(const std::string &str);

	// �ַ���ת����long
	static long TransStrToLong(const std::string& str);

	// �ַ���0,1������0,1��ת��
	static  int TransStrToInt(const std::string& str);

	// �����ַ����鵽�ִ���ת������Ҫ�������ݽṹ���ַ����ֵ�ת��
	static void SetElemValueStr(const IVS_CHAR* pXmlValue,IVS_UINT32 ulLength,CXml &xml);

	// ���彫ʮ���Ƶ�����ת��Ϊ�������ִ�
	static std::string TransDecToBinStr(unsigned int ulValue);

	//ƴװOCX�����¼���XML
	static void GetEventXML(CXml &xml,const std::string &strEvent,ULONG ulWndID,const std::string &strStatus = "");

    static void GetTalkbackXML(CString &strResult, IVS_INT32 iRet, IVS_ULONG ulHandle);

	static int BuildShareCatalogResultXml(IVS_INT32 iRet, const IVS_CHAR* pRspXml, CString& strResult);
};

#endif // __OCX_XML_PROCESS_H__
