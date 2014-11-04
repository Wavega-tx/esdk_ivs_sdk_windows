/********************************************************************
	filename	: 	OCXMotionDetecMgrXMLProcess.h
	author		:	wWX155351
	created		:	2012/12/03	
	description	:	ƴװ�ͽ����ƶ�����xml
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __OCX_MOTIONDETECTION_MGR_XML_PROCESS_H__
#define __OCX_MOTIONDETECTION_MGR_XML_PROCESS_H__

#include "OCXXmlProcess.h"

class COCXMotionDetecMgrXMLProcess
{
public: 
	COCXMotionDetecMgrXMLProcess();
	~COCXMotionDetecMgrXMLProcess();

public:
	//����ƽ̨�ƶ����xml
	static IVS_INT32 ParseSetMotionDetecXml(IVS_MOTION_DETECTION* pMotionDetection, CXml& xml);

	/*****************************************************************
	     function   : GetMotionDetecAreaInfoNum
	     description: ��ȡ�˶�������Ӧ��Ϣ��AreaInfo������
	     input      : CXml & xml   ��Ӧ��Ϣxml
	     output     : IVS_INT32 & iNum  AreaInfo������
	     return     : AreaInfo������
	*****************************************************************/
	static IVS_INT32 GetMotionDetecAreaInfoNum(CXml& xml, IVS_INT32& iNum);

	// ƴװƽ̨�ƶ������Ӧ��Ϣxml
	static IVS_INT32 SetMotionDetecGetXML(const IVS_MOTION_DETECTION* pMotionDetec, CXml& xml);

	/*****************************************************************
	     function   : ParseGetMotionRangeDataPlatformXml
	     description: ������ѯƽ̨�˶�����������xml
	     input      : pStartTime����ѯ��ʼʱ�䣻
		              pEndTime����ѯ��ֹʱ�䣻
					  pMotionDetection����ѯƽ̨�˶����������ݽṹ�壻
					  iRet������룻
	     output     : CXml & xml   ��Ӧ��Ϣxml
	     return     : �����
	*****************************************************************/
	static IVS_INT32 ParseGetMotionRangeDataPlatformXml(const IVS_CHAR* pStartTime,const IVS_CHAR* pEndTime,
		const IVS_MOTION_RANGE_DATA_LIST* pMotionRangeData,IVS_INT32 iRetCode, CXml& xml);
};
#endif // __OCX_MOTIONDETECTION_MGR_XML_PROCESS_H__
