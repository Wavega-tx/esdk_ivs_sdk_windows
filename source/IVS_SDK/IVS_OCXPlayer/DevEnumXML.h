/*****************************************************************
 filename    :    DevEnumXML.h
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/
#ifndef __DEV_ENUM_H__
#define __DEV_ENUM_H__

#pragma once
#include "hwsdk.h"


class CDevEnumXML
{
private:
    CDevEnumXML();
public:
    // �������ͻ�ȡ
    static IVS_INT32 GetDevTypeEnum(const IVS_CHAR* pDeviceType);

    // �������ͻ�ȡ
    static const IVS_CHAR* GetDevType(IVS_INT32 iDeviceType);

    // ��ȡ��Ƶ��ʽ
    static IVS_INT32 GetVideoFormatEnum(const IVS_CHAR* pProtocolType);
    
	// ��ȡ��Ƶ��ʽ��ö����
    static const IVS_CHAR* GetVideoFormatType( const IVS_INT32 iFormatType );
  
    // ��ȡͨ��״̬
    static IVS_BOOL GetCommStatusBool(const IVS_CHAR* pCommStatus);

	// ��ȡͨ��״̬
    static const IVS_CHAR* GetCommStatusType( IVS_BOOL bEnable );

    //��ȡ�����ö����
    static IVS_BOOL GetFieldEnum(const IVS_CHAR* pFieldType);

	// ��ȡ�澯����
    static const IVS_BOOL GetAlamTypeEnum(const IVS_CHAR* cAlam );

	// ��ȡ�澯����
	static const IVS_CHAR* GetAlamTypeCH(  IVS_BOOL bAlam );

	// ��ȡ���������
    static const IVS_CHAR *GetPosNegDirCtlCH(bool bPosNegDirCtl);

	// ��ȡ��ͷ�����ֱ���
    static const IVS_CHAR *GetResolutionCH(IVS_INT32 resolution);

	// ��ȡ�����ʽ
    static const IVS_CHAR *GetEnCodeModeCH(IVS_INT32 enCodeMode);

	// ��ȡ��������
    static const IVS_CHAR *GetBitRateTypeCH(IVS_INT32 bitRateType);

	// ��ȡ�����ʽΪH264��Profile
    static const IVS_CHAR *GetH264ProfileCH(IVS_INT32 profile);

	// ��ȡ�����ʽΪMJPEG��Profile
    static const IVS_CHAR *GetMjpegProfileCH(IVS_INT32 profile);

	// ��ȡ���豸Э������
	static const IVS_CHAR *GetDevProtocolTypeCH(IVS_INT32 typeCode);

	// ��ȡ���豸�ṩ������
	static const IVS_CHAR *GetDevVendorTypeCH(IVS_INT32 typeCode);

	// ��ȡͨ������
    static IVS_VOID  GetChannelType( IVS_INT32 iNum ,IVS_CHAR *pchTemp);

	// ��ȡ���������
    static IVS_INT32 GetPosNegDirCtlEnum(const IVS_CHAR* posNegDirectControl);

	// ��ȡ��ͷ�����ֱ���
    static IVS_INT32 GetResolutionEnum(const IVS_CHAR* resolution);

	// ��ȡ�����ʽ
    static IVS_INT32 GetEnCodeModeEnum(const IVS_CHAR* enCodeMode);

	// ��ȡ��������
    static IVS_INT32 GetBitRateTypeEnum(const IVS_CHAR* bitRateType);

	// ��ȡ�����ʽΪH264��Profile
    static IVS_INT32 GetH264ProfileEnum(const IVS_CHAR* profile);

	// ��ȡ�����ʽΪMJPEG��Profile
    static IVS_INT32 GetMjpegProfileEnum(const IVS_CHAR* profile);

	// ��ȡ�����ö����
    static IVS_BOOL GetOrderTypeBool( const IVS_CHAR* pLoginType );

	// ��ȡ���豸Э������
	static IVS_INT32 GetDevProtocolTypeEnum( const IVS_CHAR* typeCode );

	// ��ȡ���豸�ṩ������
	static IVS_INT32 GetDevVendorTypeEnum( const IVS_CHAR* typeCode );

	//��ȡ��������
	static IVS_INT32 GetSerialTypeEnum( const IVS_CHAR* typeCode );

	//��ȡ�������Ͷ�Ӧ���ַ���
	static const IVS_CHAR *GetSerialTypeCH(IVS_INT32 typeCode);

	// ��ȡ�ɿ��������ʹ��ַ���������
	static IVS_INT32 GetReliableTransTypeINT(const IVS_CHAR* pTransType);

	// ��ȡ�ɿ��������ʹ����ε��ַ���
	static const IVS_CHAR* GetReliableTransTypeCHAR(IVS_INT32 iTransType);

	// ��ȡ���ݸ�ʽ���ʹ��ַ���������
	static IVS_INT32 GetDataFormatINT(const IVS_CHAR* pDataFormat);

	// ��ȡ���ݸ�ʽ���ʹ����ε��ַ���
	static const IVS_CHAR* GetDataFormatCHAR(IVS_INT32 iDataFormat);

	// ��ȡ���������
	static const IVS_CHAR* GetCameraTypeEnum(IVS_INT32 iCameraType);

	// ��ȡͨ���Ƿ����
	static const IVS_CHAR* GetCommIsOrNotType(IVS_INT32 iType);

	// ��ȡ��������
	static const IVS_CHAR* GetNetType(IVS_INT32 iNetType);

	// ��ȡ��·��Ƶ
	static const IVS_CHAR* GetVoiceInEnable(IVS_INT32 iVoiceInEnable);

	// ��ȡ�豸���ô�С
	static IVS_UINT32 GetSizeOfConfigType(IVS_UINT32 uiConfigType);

	// ��ȡͨ��ģʽ
	static IVS_INT32 GetCommMode(const IVS_CHAR* pCommMode);

	// ��ȡ�ع�����
	static IVS_INT32 GetExposurePriority(const IVS_CHAR* pExposurePriority);

	// ��ȡͨ�ù���ģʽ
	static IVS_INT32 GetCommExImgMode(const IVS_CHAR* pCommExImgMode);

	// ��ȡ�ع�����
	static const IVS_CHAR* GetExposurePriorityStr(IVS_INT32 iExposurePriority);

	// ��ȡͨ��ģʽ
	static const IVS_CHAR* GetCommModeStr(IVS_INT32 iCommMode);

	// ��ȡͨ�ù���ģʽ
	static const IVS_CHAR* GetCommExImgModeStr(IVS_INT32 iCommExImgMode);
};

#endif
