/*****************************************************************
 filename    :    IVS_OCXPlayerPropPage.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ������ʾ�Զ���ؼ���������ͼ�ν����
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once

// IVS_OCXPlayerPropPage.h : Declaration of the CIVS_OCXPlayerPropPage property page class.
// CIVS_OCXPlayerPropPage : See IVS_OCXPlayerPropPage.cpp for implementation.

class CIVS_OCXPlayerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CIVS_OCXPlayerPropPage)
	DECLARE_OLECREATE_EX(CIVS_OCXPlayerPropPage)	//lint !e1768  MFC�Զ����ɵĵĺ���

// Constructor
public:
	CIVS_OCXPlayerPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_IVS_OCXPLAYER };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
}; //lint !e1712  MFC�Զ����ɵĵĺ���

