#include "stdafx.h"
#include "SDKDef.h"
#include "IVS_OCXPlayerCtrl.h"
#include "hwsdk.h"
#include "ToolsHelp.h"
#include "ivs_error.h"
#include "OCXReportMgr.h"
#include "IVS_Trace.h"
#include "OCXXmlProcess.h"

COCXReportMgr::COCXReportMgr(void)
{
}

COCXReportMgr::~COCXReportMgr(void)
{
}

// ʵʱ�����ѯ
CString COCXReportMgr::OMUQueryDevReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryDevReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryDevReport SDK Interface fail", "NA");
		if (NULL != pRspXml)
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ʵʱ���������ѯ
CString COCXReportMgr::OMUQueryDevReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryDevReportDetail(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryDevReportDetail SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ʵʱ�����ѯ
CString COCXReportMgr::OMUQueryRealtimeReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryRealtimeReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryRealtimeReport SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ʵʱ���������ѯ
CString COCXReportMgr::OMUQueryRealtimeReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryRealtimeReportDetail(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryRealtimeReportDetail SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ��Ƶ������ϲ�ѯ
CString COCXReportMgr::OMUQueryQDReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryQDReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryQDReport SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ��Ƶ������������ѯ
CString COCXReportMgr::OMUQueryQDReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryQDReportDetail(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryQDReportDetail SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ¼��ִ�мƻ������ѯ
CString COCXReportMgr::OMUQueryRecordPlanReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryRecordPlanReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryRecordPlanReport SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ¼��ִ�мƻ����������ѯ
CString COCXReportMgr::OMUQueryRecordPlanReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryRecordPlanReportDetail(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryRecordPlanReportDetail SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ¼�������Ա����ѯ
CString COCXReportMgr::OMUQueryRecordIntegralityReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryRecordIntegralityReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryRecordIntegralityReport SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ¼�������Ա��������ѯ
CString COCXReportMgr::OMUQueryRecordIntegralityReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryRecordIntegralityReportDetail(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryRecordIntegralityReportDetail SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// �����ʱ����ѯ
CString COCXReportMgr::OMUQueryPacketLossRateReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryPacketLossRateReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryPacketLossRateReport SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// �����ʱ��������ѯ
CString COCXReportMgr::OMUQueryPacketLossRateReportDetail(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryPacketLossRateReportDetail(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryPacketLossRateReportDetail SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}

// ���̴洢ռ���ʱ����ѯ
CString COCXReportMgr::OMUQueryDiskUsageRateReport(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_CHAR* pRspXml = NULL;
    IVS_INT32 iRet = IVS_SDK_OMUQueryDiskUsageRateReport(iSessionID, pReqXml, &pRspXml);
    if (IVS_SUCCEED != iRet || NULL == pRspXml)
    {
        BP_RUN_LOG_ERR(iRet, "OMUQueryDiskUsageRateReport SDK Interface fail", "NA");
        if (NULL != pRspXml)
        {
            IVS_SDK_ReleaseBuffer(pRspXml);
        }
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    std::string strInXml = pRspXml;
    std::string strOutXml;
    iRet = COCXXmlProcess::AddResultCodeUnderContent(iRet, strInXml, strOutXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add ResultCode Under Content fail", "NA");
		if (NULL != pRspXml)//lint !e774
		{
			IVS_SDK_ReleaseBuffer(pRspXml);
		}
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_SDK_ReleaseBuffer(pRspXml);
    return strOutXml.c_str();
}
