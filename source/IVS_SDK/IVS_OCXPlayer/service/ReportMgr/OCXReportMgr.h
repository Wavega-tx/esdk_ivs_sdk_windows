/********************************************************************
filename : OCXReportMgr.h
author : s00191067
created : 2013/3/7
description : ���������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/3/7 ��ʼ�汾
*********************************************************************/


#ifndef __OCX_REPORT_MGR_H_
#define __OCX_REPORT_MGR_H_

#include "hwsdk.h"
#include "IVSCommon.h"

class COCXReportMgr
{
public:
    COCXReportMgr(void);
    ~COCXReportMgr(void);

public:
    // ʵʱ�����ѯ
    static CString OMUQueryDevReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ʵʱ���������ѯ
    static CString OMUQueryDevReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ʵʱ�����ѯ
    static CString OMUQueryRealtimeReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ʵʱ���������ѯ
    static CString OMUQueryRealtimeReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ��Ƶ������ϲ�ѯ
    static CString OMUQueryQDReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ��Ƶ������������ѯ
    static CString OMUQueryQDReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ¼��ִ�мƻ������ѯ
    static CString OMUQueryRecordPlanReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ¼��ִ�мƻ����������ѯ
    static CString OMUQueryRecordPlanReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ¼�������Ա����ѯ
    static CString OMUQueryRecordIntegralityReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ¼�������Ա��������ѯ
    static CString OMUQueryRecordIntegralityReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // �����ʱ����ѯ
    static CString OMUQueryPacketLossRateReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // �����ʱ��������ѯ
    static CString OMUQueryPacketLossRateReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml);

    // ���̴洢ռ���ʱ����ѯ
    static CString OMUQueryDiskUsageRateReport(IVS_INT32 iSessionID, LPCTSTR pReqXml);

};

#endif
