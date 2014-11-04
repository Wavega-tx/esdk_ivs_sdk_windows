/********************************************************************
	filename	: 	download.h
	author		:	w00210470
	created		:	2013/02/06	
	description	:	����ҵ����;
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/02/06 ��ʼ�汾;
*********************************************************************/
#ifndef __DOWMLOAD_H__
#define __DOWMLOAD_H__

#include  "MediaBase.h"

class CDownload : public CMediaBase
{
public:
	CDownload(void);
	~CDownload(void);
public:
	void SetHandle(unsigned long /*ulHandle*/);
	unsigned long GetHandle();

    int StartPlatformDownLoad(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
		      const char* pCameraCode,
  			  const IVS_DOWNLOAD_PARAM* pDownloadParam
		    );
    
	int StopPlatformDownLoad();

    // ����
    int StartDisasterRecoveryDownload(const char* pCameraCode, const IVS_DOWNLOAD_PARAM *pDownloadParam);
    int StopDisasterRecoveryDownload();

    int DownLoadPause();
    int DownLoadResume();
	
	int SetDownLoadSpeed(float fSpeed);
    int GetDownLoadInfo(IVS_DOWNLOAD_INFO* pInfo);
	
    int StartPUDownLoad(
			const char* pCameraCode,
			const IVS_DOWNLOAD_PARAM* pDownloadParam,
		    const char *pFileName
			);
    
    int StopPUDownLoad();

    inline IVS_SERVICE_TYPE GetServiceType()
    {
        return m_enServiceType;
    }//lint !e1762

    inline void SetServiceType(IVS_SERVICE_TYPE enServiceType)
    {
        m_enServiceType = enServiceType;
    }

	bool IsPaused() const { return m_bPaused; }

private:
    int StartDownLoad(
		    const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
		    const IVS_DOWNLOAD_PARAM *pParam,
		    const IVS_MEDIA_PARA* pstMediaPara, 
            const IVS_CHAR* pCameraCode, 
            const TIME_SPAN_INTERNAL& stTimeSpanInter,
            const START_PLAY_PARAM& stParam,
            const char* pFileName
		   );

    int DoGetUrlDownLoad(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
			IVS_SERVICE_TYPE ServiceType,
            const TIME_SPAN_INTERNAL& stTimeSpanInter,
            const char *pFileName
		   );
private:
	TIME_SPAN_INTERNAL m_stTimeSpanInter;                // �������Ŀ�ʼ����ʱ��;
	float              m_fSpeed;                         // �����ٶ�;
    IVS_SERVICE_TYPE   m_enServiceType;

	unsigned int       m_uLastPercent;                   // �ϴλ�ȡ������Ϣʱ�İٷֱ�
	unsigned int       m_uLastLeftTime;                  // ��һ�μ���ʣ��ʱ��
	unsigned long long m_ullLastInfoTick;                // ��һ�λ�ȡ������Ϣʱ���
	unsigned long long m_ullPastTime;					 // �����Ѿ����ĵ�ʱ��;
	bool               m_bPaused;                        // ��ͣ״̬

};


#endif // end of __DOWMLOAD_H__
