#pragma once

// stdafx.h : include file for standard system include files,or project 
// specific include files that are used frequently,but are changed infrequently

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

//#include <winsock2.h>

#define _CONVERSION_DONT_USE_THREAD_LOCALE 
#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _ATL_NO_AUTOMATIC_NAMESPACE             // avoid class name conflicts
#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Comon Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>          // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

//#include <afxwin.h>

#include <atlconv.h>

#include <WinUser.h>

// _GDI+_SUPPORT
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

//STL �㷨֧��
#include <algorithm>
#include <bitset>
#include <array>
#include <vector>
#include <map>
#include <list>

//�������ݽṹ
#include "hwsdk.h"
#include "ivs_error.h" //������ͷ�ļ�
#include "constant.h"  //��������ͷ�ļ�
#include "ToolsHelp.h"

#include "version.h"                    //�汾�ļ������ļ�
#include "IVS_SDK.h"
#include "SDKDef.h"
#include "IVS_SDKTVWall.h"
#include "IVS_SDKNetKeyBoard.h"
#include "vos.h"
#include "ENUM_DEF.h"
#include "OCXDatatype.h"

// BP ��־ 
#include "bp_def.h"
#include "bp_log.h"
#include "bp_log_conf.h"
#include "bp_environments.h"

#include "TranslucentDialog.h"

#include "SkinMgr.h"   // Ƥ��������
#include "Utility.h"   //
#include "LayoutMgr.h" // ���ֹ�����
#include "I18N.h" // ���ʻ�

#include "IVSCommon.h"
#include <afxcontrolbars.h>

// CDialogEX
#include <afxdialogex.h>

#include <afx.h>
#include <afxwin.h>
//#include <afxwin.h>
#endif // _WIN64
//#define CMemDC X_CMemDC


//#include "Skin.h"

