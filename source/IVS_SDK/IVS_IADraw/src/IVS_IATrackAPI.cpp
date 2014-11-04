#include "IVS_IATrackAPI.h"
#include "IVSCommon.h"
#include "IVS_IATrack.h"
#include "ivs_log.h"
#include "ivs_error.h"


// �������Ч��
#define CHECK_HANDLE(h) \
	do { IVS_ASSERT(h); if (!(h)) return IVS_FAIL;} while(0)


//// ��ʼ�����ܷ�����
//IVS_IATRACK_API INT IATrack_LibInit()
//{
//	int iRet = 0; 
//	iRet = IVS_LogInit("../log/", "mp_log.conf" , "IVS_IA_Track", "ia_track", 103, NULL);
//	return iRet;
//}

// �������ܷ�����
// IVS_IATRACK_API VOID IATrack_LibClean()
// {
// 	IVS_LogClean();
// 	return;
// }

// ��ȡIADraw���
IVS_IATRACK_API IATRACK_HANDLE IATrack_GetHandle()
{
	CIATrack *pIATrack = IVS_NEW(pIATrack);
	if (NULL == pIATrack)
	{
		IVS_LOG(IVS_LOG_ERR, "Get Handle", "ALLOC MEM ERROR.");
		return (IATRACK_HANDLE)NULL;
	}
	IVS_LOG(IVS_LOG_DEBUG, "Get Handle API", "IATRACK_HANDLE: %x", pIATrack);
	return (IATRACK_HANDLE)pIATrack;
}

// ��ʼ�����ܷ����켣
IVS_IATRACK_API LONG IATrack_Init(IATRACK_HANDLE handle, HWND hMsgWnd, ULONG ulVideoType)
{
	CHECK_HANDLE(handle);
	CIATrack *pIATrack = (CIATrack *)handle;
	return pIATrack->InitIATrack(hMsgWnd, ulVideoType);
}

// �������ܷ����켣��Ϣ
IVS_IATRACK_API LONG IATrack_InsertIALayoutMessage(IATRACK_HANDLE handle, const IA_LAYOUT_MESSAGE *LayoutMessage)
{
	CHECK_HANDLE(handle);
	CIATrack *pIATrack = (CIATrack *)handle;
	IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message API", "IATRACK_HANDLE: %x", handle);
	return pIATrack->InsertIALayoutMessage(LayoutMessage);
}

// ��ʾ���ܷ����켣��Ϣ
IVS_IATRACK_API LONG IATrack_ShowIATrack(IATRACK_HANDLE handle, HDC hDC, const RECT *rc, ULONG64 ulFrameID)
{
	CHECK_HANDLE(handle);
	CIATrack *pIATrack = (CIATrack *)handle;
	return pIATrack->ShowIATrack(hDC, rc, ulFrameID);
}

// ��ȡ��ǰ�켣��Ϣ����
IVS_IATRACK_API LONG IATrack_GetCurrentElementNum(IATRACK_HANDLE handle)
{
	CHECK_HANDLE(handle);
	CIATrack *pIATrack = (CIATrack *)handle;
	return pIATrack->GetCurrentElementNum();
}


// �ͷ�IADraw���
IVS_IATRACK_API LONG IATrack_FreeHandle(IATRACK_HANDLE handle)
{
	CHECK_HANDLE(handle);
	IVS_LOG(IVS_LOG_DEBUG, "Free Handle API", "IATRACK_HANDLE: %x", handle);
	CIATrack *pIATrack = (CIATrack *)handle;
	IVS_DELETE(pIATrack);
	return IVS_SUCCEED;
}

