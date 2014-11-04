/********************************************************************
	filename	: 	NetKeyBoardService.h
	author		:	
	created		:	2011/10/28	
	description	:	�������ʵ����
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/10/28 ��ʼ�汾
*********************************************************************/
#ifndef NET_KEY_BOARD_H
#define NET_KEY_BOARD_H

/*!< ����ʵ��ͷ�ļ� */
#include "SingleInst.h"
/*!< �����ꡢ�ṹ��ͷ�ļ� */
#include "IVSSDKNetKeyBoardDef.h"


class CNetKeyBoard
{
    DECLARE_SINGLE_INSTANCE(CNetKeyBoard)

public:
    CNetKeyBoard(void);
    virtual ~CNetKeyBoard(void);


    /**************************************************************************
    * name       : SetListenAddr
    * description: ���÷������˼���IP�Ͷ˿ں�
    * input      : iPort�������˼����˿ں�
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void SetNetKeyBoardListenAddr(unsigned short iPort);

    /**************************************************************************
    * name       : StartListenThread
    * description: ���������߳�
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : g_hThread
    * remark     : NA
    **************************************************************************/
    int StartNetKeyBoard(unsigned short ulNetKeyBoardVendor, unsigned short ulPort, LPCallBack lpCallBack);

    /**************************************************************************
    * name       : ReleaseThread
    * description: �ͷ��߳�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int ReleaseThread();

private:
    bool m_bRun;

};
#endif //NET_KEY_BOARD_H
