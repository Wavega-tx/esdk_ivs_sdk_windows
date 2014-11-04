/********************************************************************
	filename	: 	IVS_OCXPlayerRes.h
	author		:	z00201790
	created		:	2013/01/24	
	description	:	������������Դ�������;
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2013/01/24 ��ʼ�汾;
*********************************************************************/
#ifndef __IVS_I18N_H__
#define __IVS_I18N_H__
#include "vos.h"
#include "SDKDef.h"
typedef std::map<std::string,std::string> MAP_RESOURCE_TO_LANGUAGE;
typedef MAP_RESOURCE_TO_LANGUAGE::iterator MAP_RESOURCE_TO_LANGUAGE_ITER;

class CI18N
{
public:
	CI18N(void);
	~CI18N(void);

	static CI18N& GetInstance()
	{
		static CI18N inst;
		return inst;
	}
public:
	//����XML����Դ·����XML���ص�Map��
	bool SetLanguage(const std::string& strLanguageXMLPath);
	bool GetResourceLanguage(const std::string& strResource,std::string& strLanguage);
	void Clear();

private:
	VOS_Mutex* m_pMutexLock;

	MAP_RESOURCE_TO_LANGUAGE m_mapResource2Language;
};

#endif
