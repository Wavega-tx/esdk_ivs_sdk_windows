#include "I18N.h"
#include "ToolsHelp.h"

CI18N::CI18N(void)
	:m_pMutexLock(nullptr)
{
	m_pMutexLock = VOS_CreateMutex();
}

CI18N::~CI18N(void)
{
	try
	{
		Clear();
		VOS_DestroyMutex(m_pMutexLock);
		m_pMutexLock = NULL;
	}
	catch (...)
	{
	}
}

void CI18N::Clear()
{
	try
	{
		m_mapResource2Language.clear();
	}catch (...)
	{}
}

bool CI18N::SetLanguage(const std::string& strLanguageXMLPath)
{
    BP_RUN_LOG_INF("load language xml failed","strLanguageXMLPath %s", strLanguageXMLPath.c_str());
	CXml xml;
	if (!xml.Load(strLanguageXMLPath.c_str(), TIXML_ENCODING_UTF8))
	{
		//��ʾ������ԴXML���ɹ�
		BP_RUN_LOG_ERR(IVS_FAIL,"load language xml failed","NA");
		return false;
	}

	//������سɹ��ˣ��ͱ���xml�����е���Դ�����԰󶨲�����map��
	if (!xml.FindElemEx("Content"))
	{
		//û�ҵ���һ�����
		return false;
	}

	//�Ƚ������
	Clear();

	do 
	{
		// ��ȡ��ǰ���
		const char* ResourceNode = xml.GetElem();
		// ��ȡ��ǰ����ֵ
		const char* ResourceNodeValue = xml.GetElemValue();
		// ת�����룬��ֹ����
		char* xml2ANSI = CToolsHelp::UTF8ToANSI(ResourceNodeValue);
		//�ѽ���ֵ�󶨽��в���
		(void)VOS_MutexLock(m_pMutexLock);
		(void)m_mapResource2Language.insert(std::make_pair(ResourceNode, xml2ANSI));
		(void)VOS_MutexUnlock(m_pMutexLock);
		free(xml2ANSI);
	} while (xml.NextElem());

	return true;
}

//������ԴID��map����ȥ������ȡ����Դ��Ӧ������
bool CI18N::GetResourceLanguage(const std::string& strResource,std::string& strLanguage)
{
	bool bRet = false;
	MAP_RESOURCE_TO_LANGUAGE_ITER IpIter;
	(void)VOS_MutexLock(m_pMutexLock);

	IpIter = m_mapResource2Language.find(strResource);
	if (IpIter != m_mapResource2Language.end())
	{
		//�ҵ���
		strLanguage = static_cast<std::string>(IpIter->second);
		bRet = true;
	}
	else
	{
		//û�ҵ�
		strLanguage = "";
	}
	(void)VOS_MutexUnlock(m_pMutexLock);
	return bRet;
}
