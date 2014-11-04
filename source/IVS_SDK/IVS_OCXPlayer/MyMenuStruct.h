/*****************************************************************
 filename    :    MyMenuStruct.h
 author      :    ykf75928
 created     :    2013/01/16
 description :    �˵��ṹ��
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/16 ��ʼ�汾
*****************************************************************/
#ifndef MENUGROUP_STRUCT
#define MENUGROUP_STRUCT

#include "IVSButton.h"
#include "IVSMenuHeader.h"

#define		MENU_CLASSNAME				_T("IVSMenuCtrl")
#define		FRAME_CLASSNAME				_T("IVSMenuFrameCtrl")
#define		HEADER_CLASSNAME			_T("IVSMenuHeader")
#define		ITEM_CLASSNAME				_T("IVSMenuItem")

#define		MENUTITLE_HEIGHT			48
#define		MARGIN						4
#define		MENUICON_WIDTH				32
#define		MENUICON_HEIGHT				32
#define		BUTTON_MAGIN				8
#define		BTNICON_WIDTH				16
#define		BTNICON_HEIGHT				16
#define		BUTTON_HEIGHT				24  //BTNICON_HEIGHT

//����Ŀ�ı���ɫ�������С
#define		BTN_OVER_COL				RGB(255,255,255)
#define		BTN_NORMAL_COL				RGB(204,204,204)
#define		BTN_FONT_SIZE				14

//��ϲ˵��ı���ɫ�������С
#define		MENU_OVER_COL				RGB(0,0,0)
#define		MENU_NORMAL_COL				RGB(164,164,164)
#define		MENU_FONT_SIZE				16

//��ϲ˵��򿪵�ʱ��߿���ɫ
#define		MENU_WIDGET_COL				RGB(225,221,98)

//��ϲ˵������ı����ⱳ��������ɫ��������
#define		MENUTITLE_LEFT_COL			RGB(220,215,61)
#define		MENUTITLE_RIGHT_COL			RGB(255,255,255)

//��ϲ˵���ʱ�ڲ�����������ɫ�����ϵ���
#define		MENU_TOP_COL					RGB(29, 37, 10)

//��Ͽ�ͷ���򿪹رմ���ID
#define	IDC_MENUHEADER				1111111
#define	IDC_MENU	 100
#define IDC_SUBITEM  132		
#define IDC_SUBITEMMAX 398
//�û����´���λ����Ϣ�����Բ�����
//#define		WM_UPDATEPOS				WM_USER+1

typedef struct STRU_BUTTONINFO
{
	CString			strTitle;				//GroupMenu������ı���
	UINT			nIcon;					//GroupMenu�������ͼ��
	bool			isCreatedResource;		// true is created by Resource; false is Created by filePath;
	IVS_INT32			nNorBitmap;				//GroupMenu�������λͼ
	IVS_INT32			nSelBitmap;				//GroupMenu�������λͼ
	IVS_INT32			nDisBitmap;				//GroupMenu�������λͼ
	std::string         strNorBitmap;
	std::string         strSelBitmap;
	std::string         strDisBitmap;
	UINT			nBtnID;					//GroupMenu�������ID
	BOOL			bSubMenu;
	CString			strTip;
	CWnd*			pChild;
	BOOL			bIsBordLine;				//
	BOOL			bFontBold;
	BOOL			bCheckBox;
	CIVSButton	btn;					//GroupMenu������
}MenuItemInfo;

typedef CTypedPtrList<CPtrList, MenuItemInfo*> TButtonList;

typedef struct STRU_GROUPMENU
{
	CString			strTitle;			//����
	UINT			nIcon;				//ͼ��
	UINT			nBitmap;			//λͼ
	BOOL			bStatus;			//
	CIVSHeader	    btnHeader;			//ͷ��
	TButtonList		BtnList;			//�����б�
}IVSMenu;

#endif
