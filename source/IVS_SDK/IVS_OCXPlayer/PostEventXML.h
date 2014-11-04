/*****************************************************************
 filename    :    PostEventXML.h
 author      :    yKF75928
 created     :    2012/11/19
 description :    ��ȡ������Ϣ���ĵ���;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/

#ifndef _POSTEVENTXML_H_D1CDDE33_F3A9_4EF5_8875_555FF7E39AD7_
#define _POSTEVENTXML_H_D1CDDE33_F3A9_4EF5_8875_555FF7E39AD7_

#pragma once

#include <afx.h>
#include <string>
#include <map>
#include "hwsdk.h"
#include "SDKDef.h"

#define PostEventXMLImpl PostEventXML::Instance()

class PostEventXML
{
private:
	std::map<ULONGLONG,std::string> m_mapRealtimeToolBarBtnName;
	std::map<ULONGLONG,std::string> m_mapAnalysisToolBarBtnName;
	PostEventXML(void);                 //lint !e1704
public:
	static PostEventXML& Instance();
public:
    ~PostEventXML(void);

    IVS_VOID PostTalkbackEvent(CString &xmlToCU, ULONG ulResult);
	
    /*****************************************************************
         function   : GetActiveWndChangeEventMsgXML
         description: ��õ�ǰ����������Ϣ����
         output     : CString & xmlToCU		����
         input      : ULONG ulWinID			����ID
		 return     : NA
    *****************************************************************/
    IVS_VOID GetActiveWndChangeEventMsgXML(CString &xmlToCU, ULONG ulWinID);

	
    /*****************************************************************
         function   : GetExchangeWndEventMsgXML
         description: ��ý��������¼���Ϣ����
         output     : CString & xmlToCU   ����
         intput     : ULONG ulSrcWnd      Դ���ھ��
                      ULONG ulDstWnd      Ŀ�괰�ھ��
         return     : NA
    *****************************************************************/
    IVS_VOID GetExchangeWndEventMsgXML(CString &xmlToCU, ULONG ulSrcWnd, ULONG ulDstWnd);

    /*****************************************************************
         function   : GetMouseClickEventXML
         description: ��ȡ������¼�����Ϣ����
         output     : xmlToCU		����
         intput     : ULONG ulWinID ����ID
                      ULONG ulX     ������ڵ�X����
                      ULONG ulY     ������ڵ�Y����
         return     : NA
    *****************************************************************/
    IVS_VOID GetMouseClickEventXML(CString &xmlToCU, ULONG ulWinID, ULONG ulX, ULONG ulY);

    /*****************************************************************
         function   : GetStopButtonClickMsgXML
         description: ��ȡֹͣ�������Ϣ����
         output     : CString & xmlToCU  ����
         output     : ULONG ulWinID      ����ID
         return     :  NA
    *****************************************************************/
    IVS_VOID GetStopButtonClickMsgXML(CString &xmlToCU, ULONG ulWinID);

	/*****************************************************************
	     function   : GetPTZConfig
	     description: ��ȡ�ƾ���������
	     output     : const std::string & strXML   ����
	     intput     : long * lResultCode           ������
	                  unsigned int * iSpeed: 1-����  2-����
	     return     : �ɹ�����0, ʧ�ܷ���1
	*****************************************************************/
	IVS_LONG GetPTZConfig(const std::string &strXML,IVS_LONG *lResultCode , IVS_UINT32 *iSpeed);

    /*****************************************************************
         function   : GetOcxEventMsgStrXML
         description: ��ȡ�¼���Ϣ����
         output     : std::string & xmlToCU
         output     : PTCHAR pMsg
         output     : LPCTSTR pMsgSeq
         output     : ULONG ulResultCode
         output     : ULONG ulWndID
         return     : IVS_VOID
    *****************************************************************/
    IVS_VOID GetOcxEventMsgStrXML(std::string &xmlToCU, LPCTSTR pMsg, LPCTSTR pMsgSeq, ULONG ulResultCode, ULONG ulWndID);

	IVS_VOID GetBtnName(std::string &strBtnName ,ULONG ulBarType,ULONGLONG ulBtnTtype);

	IVS_VOID GetToolBarClickEventMsg(std::string &xml,const std::string &strEvent,ULONG ulWndID,const std::string &strStatus);

	IVS_VOID GetLadderLayoutEventMsg(std::string &xml,ULONG ulWndNum);

	IVS_VOID GetToolBarEMapClickEventMsg(std::string &xml,const std::string &strEvent,ULONG ulWndID,CRect& rect);

	IVS_VOID GetVolumeEventMsg(CString &xmlToCU, ULONG ulWndID, ULONG ulEventType, ULONG ulVolumeValue) const;

	IVS_VOID GetInstantReturnEventMsg(std::string &xml,ULONG ulWndID);

	IVS_VOID GetFullSCreenXML(std::string &xmlToCU, ULONG ulResultCode, ULONG ulWndID);
};
#endif // _POSTEVENTXML_H_D1CDDE33_F3A9_4EF5_8875_555FF7E39AD7_

