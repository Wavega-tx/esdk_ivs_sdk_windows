/*****************************************************************
 filename    :    common_struct.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ���ýṹ��
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#ifndef _COMMON_STRUCT_H_A9366BA0_5095_4557_970B_4D3881F526A6_
#define _COMMON_STRUCT_H_A9366BA0_5095_4557_970B_4D3881F526A6_

#include "VideoPane.h"

#define CAMERA_ID_LEN 32

//����SDKԶ��¼�����ػỰ������;
typedef struct _stPORT_SESSIONID_INFO
{
    int     iHsSessionID;
    ULONG   iHsHandle;
}PORT_SESSIONID_INFO;

typedef struct _stHSHANDLE_INFO
{
    TCHAR   szRecordPath[MAX_PATH];    // ¼������λ��
    ULONG   ulPlayedTime;              // ��ǰ����ʱ��
    ULONG   ulDownloadedSize;          // ��ǰ����¼���С
}HSHANDLE_INFO , *LPHSHANDLE_INFO;

typedef struct  _stCaptureMsgData
{
    unsigned long ulRetCode;
    unsigned long ulSuccessPicNum;
    unsigned long ulTotalPicNum;
}CAPTURE_MSG_DATA, *LPCAPTURE_MSG_DATA;


typedef std::map <ULONG , PORT_SESSIONID_INFO>  DOWNLOAD_HANDLE_MAP;    //����SDKԶ��¼������map��;
typedef std::map <ULONG, CVideoPane*>           MAP_VIDEOPANE;          // �������map��
typedef MAP_VIDEOPANE::iterator                 MAP_VIDEOPANE_ITER;          
typedef std::pair<const ULONG, CVideoPane*>     PairVideoPane;
typedef std::map <HWND, CVideoPane*>           WATER_MASK_VIDEO_PANE_MAP;      // ˮӡmap��(���ھ��->����)
typedef std::map <std::string, HWND>                 WATER_MASK_CAMERA_MAP;      // ˮӡmap��(CameraID->���ھ��)
typedef std::list<CRect> RECTLIST;
typedef RECTLIST::iterator RECTLISTITER;


typedef std::map<HWND, ULONG*> HWND_WID_MAP;

#endif // _COMMON_STRUCT_H_A9366BA0_5095_4557_970B_4D3881F526A6_
