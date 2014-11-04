/*****************************************************************
 filename    :    OCXDeviceParaConfigXMLProcess.h
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/
#ifndef __OCX_DEVICE_PARA_CONFIG_XML_PROCESS_H__
#define __OCX_DEVICE_PARA_CONFIG_XML_PROCESS_H__

#pragma once

#include "SDKDef.h"

//sdk�ڲ������
#define IVS_SDK_DEV_CODE_LEN 32

class COCXDeviceParaConfigXMLProcess
{
public:
	COCXDeviceParaConfigXMLProcess();
	~COCXDeviceParaConfigXMLProcess();
   
	// �������豸������������XML
    IVS_INT32 SetMainDevConfigParseXml(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ���豸��������ƴ��XML
    IVS_INT32 GetMainDevConfigGetXML(IVS_VOID* &pBuffer, const IVS_CHAR *pDevCode, CXml& xml)const;

	// ��ѯ�������������ƴ��XML
	IVS_INT32 GetCameraStreamCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ���������������������XML
	IVS_INT32 SetCameraStreamCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�����ͼ����ʾ����ƴ��XML
	IVS_INT32 GetCameraDisplayCfgGetXml(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ���������ͼ����ʾ��������XML
	IVS_INT32 SetCameraDisplayCfgParseXml(IVS_VOID*& pBuffer, CXml& xml)const;
	
	// ��ѯ�������������ƴ��XML
	IVS_INT32 GetCameraCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;
	
	// ���������������������xml
	IVS_INT32 SetCameraCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ȡ��̨����ƴ��XML
	IVS_INT32 GetPTZCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ��ѯ�������豸��ϸ��Ϣƴ��XML
	IVS_INT32 GetAudioCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ������̨��������XML
	IVS_INT32 SetCameraPTZCfgParseXml(IVS_VOID*& pBuffer, CXml &xml)const;

	// �����������豸��������XML
	IVS_INT32 SetAudioCfgParseXml(IVS_VOID*& pBuffer, CXml &xml)const;

	// ����DevCode
	IVS_INT32 ParseDevCode(const IVS_CHAR *pDevBigCode,
		IVS_CHAR *pDevCode,
		IVS_CHAR* pDomainCode)const;

	// ���ø澯�������豸����XML
	IVS_INT32 SetAlarmInCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�澯�������豸ƴ��XML
	IVS_INT32 GetAlarmInCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ���ø澯������豸����XML
	IVS_INT32 SetAlarmOutCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�澯������豸ƴ��XML
	IVS_INT32 GetALarmOutCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ��ѯOSD����ƴ��XML
	IVS_INT32 GetOSDConfigGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ƴװ��˽����xml
	IVS_INT32 GetVideoMaskConfigGetXml(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �����޸���˽����xml
	IVS_INT32 SetVideoMaskConfigParseXml(IVS_VOID*& pBuffer, CXml &xml)const;
	
	// ����OSD��������XML
	IVS_INT32 SetCameraOsgCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ����ͼ���ڵ�����XML
	IVS_INT32 SetVideHideAlarmParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯͼ���ڵ�ƴ��XML
	IVS_INT32 GetVideoHideAlarmGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ����ǰ���ƶ����
	IVS_INT32 SetMotionDetecParseXML(IVS_VOID*& pBuffer, CXml& xml)const;

	// ��ѯ�ƶ�����������
	IVS_INT32 GetMotionDetecAreaInfoNum(CXml& xml, IVS_INT32& iNum)const;

	// ��ѯǰ���ƶ����ƴ��XML
	IVS_INT32 GetMotioinDetecGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �޸��豸�����������XML
	IVS_INT32 SetDevNetCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ���豸�������ƴ��XML
	IVS_INT32 GetDevNetCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �޸����豸ʱ���������XML
	IVS_INT32 SetDevTimeCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ���豸ʱ�����ƴ��XML
	IVS_INT32 GetDevTimeCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �޸Ĵ������豸��������XML
	IVS_INT32 SetSerialChannelCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�������豸��ϸ��Ϣƴ��XML
	IVS_INT32 GetSerialChannelCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �����������չͼ���������XML
	IVS_INT32 SetCameraImgCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�������չͼ�����ƴ��XML
	IVS_INT32 GetCameraImgCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �����������չ��������XML
	IVS_INT32 SetCameraExCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�������չ����ƴ��XML
	IVS_INT32 GetCameraExCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ���������ץ�Ĳ�������XML
	IVS_INT32 SetCameraSnapshotCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�����ץ�Ĳ���XML
	IVS_INT32 GetCameraSnapshotCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// �����豸����ͳһ����XML
	IVS_INT32 SetDeviceConfigParseXml(IVS_VOID* &pBuffer, IVS_UINT32 uiConfigType, CXml &xml);

	// ��ѯ�豸����ͳһƴ��XML
	IVS_INT32 GetDeviceConfigGetXml(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, IVS_UINT32 uiConfigType, CXml &xml);

	// ���������ǰ��¼��������������XML
	IVS_INT32 SetCameraRecordStreamCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�����ǰ��¼����������XML
	IVS_INT32 GetCameraRecordStreamCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

	// ���������FEC��������XML
	IVS_INT32 SetCameraFECCfgParseXML(IVS_VOID*& pBuffer, CXml &xml)const;

	// ��ѯ�����FEC����XML
	IVS_INT32 GetCameraFECCfgGetXML(IVS_VOID* &pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const;

private:

    

	typedef int (__thiscall COCXDeviceParaConfigXMLProcess::*FnSetDeviceParaParseXML)(IVS_VOID*& pBuffer, CXml &xml)const; 

	typedef int (__thiscall COCXDeviceParaConfigXMLProcess::*FnGetDeviceParaGetXML)(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, CXml &xml)const; 

	typedef struct
	{
		uint32_t reqID;
		FnSetDeviceParaParseXML fnSetDevicePara;
		FnGetDeviceParaGetXML fnGetDevicePara;
	} REQUEST_NAME;
	static REQUEST_NAME m_reqNames[];
	FnSetDeviceParaParseXML GetFnSetDevicePara(uint32_t enReqID)const;
	FnGetDeviceParaGetXML GetFnGetDevicePara(uint32_t enReqID)const;

};


#endif
