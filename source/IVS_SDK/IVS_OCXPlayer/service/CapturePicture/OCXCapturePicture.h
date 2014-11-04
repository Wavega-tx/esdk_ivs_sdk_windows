/********************************************************************
	filename	: 	OCXCapturePicture.h
	author		:	s00193168
	created		:	2013/1/25	
	description	:	����ͼƬץ����;
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/1/25 ��ʼ�汾;
*********************************************************************/
#ifndef __OCX_CAPTUREPICTURE_H__ 
#define __OCX_CAPTUREPICTURE_H__

#include "hwsdk.h"
#include "stdafx.h"
#include "SDKDef.h"

class OCXCapturePicture
{
public:
	OCXCapturePicture(void);
	~OCXCapturePicture(void);

	// ��ȡƽ̨ץ���б�
	/*****************************************************************
	     function   : GetSnapshotList
	     description: ��ȡƽ̨ץ���б�
		 input      : IVS_INT32 iSessionId  �ỰID
		 const IVS_CHAR * pQueryParam  ץ��ͼƬ��ѯ����
	     output     : CString & strResult  ץ��ͼƬ�б���Ϣ
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	
	static IVS_INT32 GetSnapshotList(IVS_INT32 iSessionId, const IVS_CHAR* pQueryParam, CString &strResult);

	/**********************************************************************
	* name			: SetSnapshotPlan
	* description	: ����ƽ̨ץ�ļƻ�
	* input			: strResult ƽ̨ץ�ļƻ�xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static LONG SetSnapshotPlan(const CString& strSnapshotPlan,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: GetSnapshotPlan
	* description	: ��ѯƽ̨ץ�ļƻ�
	* input			: strCameraCode ��������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString GetSnapshotPlan(const CString& strCameraCode,const IVS_INT32 iSessionID);

	
	/*****************************************************************
	     function   : DelSnapshotPlan
	     description: ɾ��ƽ̨ץ�ļƻ�
	     output     : IVS_INT32 iSessionId   �ỰID
	     output     : const IVS_CHAR * pCameraCode ���������
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 DelSnapshotPlan(IVS_INT32 iSessionId, const IVS_CHAR* pCameraCode);

	/*****************************************************************
	     function   : DeleteSnapshot
	     description: ɾ��ƽ̨ץ��ͼƬ
	     inputv     : IVS_INT32 iSessionID           �ỰID
	     input      : const IVS_CHAR * pCameraCode   ���������
	     input      : IVS_UINT32 uiPictureID         ͼƬID
	     return     : �ɹ�-0 ʧ��-������
	*****************************************************************/
	static IVS_INT32 DeleteSnapshot(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPictureID);

};


#endif

