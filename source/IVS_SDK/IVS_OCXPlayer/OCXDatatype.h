/*****************************************************************
 filename    :    OCXDatatype.h
 author      :    ykf75928
 created     :    2013/01/16
 description :    �ؼ���Ϣ�Լ��궨��
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/16 ��ʼ�汾
*****************************************************************/
#ifndef _OCX_DATATYPE__
#define _OCX_DATATYPE__

////�ؼ�������

#define WM_COMM_EVENT_CALLBACK (WM_USER + 600)  // �����ص��¼�

#define WM_EXCEPTION_CALLBACK  (WM_USER + 1)  //�쳣�ص���Ϣ
#define WM_ONCAPTURE_FINISHED  (WM_USER + 2) //����ץ�Ľ�����Ϣ
//#define WM_CLOSE_VOD_PLAY      (WM_USER + 3)  //�쳣������Ҫ��رյ㲥�ط�
#define WM_ALARM_REPORT_CALLBACK (WM_USER + 4) //�澯�ϱ���Ϣ
#define WM_ALARM_LINKAGE_CALLBACK (WM_USER + 5) //�澯������Ϣ
#define WM_DEVICE_REPORT_CALLBACK (WM_USER + 6) //�澯�ϱ���Ϣ
#define WM_STREA_EXCEPTION_CALLBACK (WM_USER + 7) //���쳣�ϱ���Ϣ
#define WM_DEVICE_ALARM_NOTIFY_CALLBACK (WM_USER + 8) // OMU�豸�澯�ϱ�
#define WM_MANUAL_REPORT_STATE_CALLBACK (WM_USER + 9) //�ֶ�¼������/ֹͣ�ϱ���Ϣ
#define WM_SYNC_PLAYBACK_MSG        (WM_USER + 10)
#define WM_WATER_ALARM_EXCEPTION_CALLBACK        (WM_USER + 11)
#define WM_BROADCAST_REPORT_CALLBACK        (WM_USER + 12)

#define WM_RESUME_REALPLAY_OK_MSG (WM_USER + 13) // ����OK
#define WM_TALKBACK_REPORT_CALLBACK           (WM_USER + 14)   // �Խ�

#define WM_ASYNC_REALPLAY_MSG   (WM_USER + 15)  // �������첽�¼�
#define WM_ASYNC_EVENT_MSG      (WM_USER + 16)  // �첽�¼��ϱ�

#define CAPTURE_DISK_WARNING_SPACE_MB      100  //ץ�ĸ澯���̿ռ�

#define WM_KEEP_ALIVE_CALLBACK  (WM_USER + 200)  

#define WM_RELOGIN_CALLBACK	(WM_USER + 300) //���µ�½�ɹ����ϱ��¼�
#define WM_EVENT_LOCAL_PLAYBACK_CALLBACK    (WM_USER + 400)  // ����¼��ط�
#define WM_EVENT_REMOTE_PLAYBACK_CALLBACK   (WM_USER + 401)  // ƽ̨�ط��¼�
#define WM_USER_OFFLINE_CALLBACK   (WM_USER + 402)  // �û������¼�

// �ֲ��Ŵ���Զ����¼�
#define WM_MAGNIFY_RECT_MOVE   (WM_USER + 500)    // �����ƶ��¼�
#define WM_MAGNIFY_RATE_CHANGE   (WM_USER + 501)  // �ٷֱȸı��¼�
#define WM_MAGNIFY_CLOSE   (WM_USER + 502)         // �ֲ��Ŵ����
#define WM_MAGNIFY_CLOSE_ALL (WM_USER + 503)    // �ر����еľֲ��Ŵ�
#define WM_TRANS_BTN_DOWN (WM_USER + 504)    
#define WM_TRANS_BTN_UP (WM_USER + 505)     
#define WM_TURNCRUISE_CLOSE (WM_USER + 506)     // �ر���ѵ��ť

//����ǽ
#define WM_TVWALL_CALLBACK (WM_USER + 550)     // ����ǽ�ص��¼�

//Hook��׽���˳�ȫ��ʱ��Ϣ
#define WM_RESET_FULL_SCREEN (WM_USER + 507)
//�������ò�����Ϣ
#define WM_RESET_WNDLAYOUT (WM_USER + 508)

//����ǽ
#define WM_TVWALL_CALLBACK (WM_USER + 550)     // ����ǽ�ص��¼�
#define WM_TVWALL_START_REALPLAY_TVWALL_END (WM_USER + 551)     // ��������ǽʵ������
#define WM_TVWALL_STOP_REALPLAY_TVWALL_END  (WM_USER + 552)     // ֹͣ����ǽʵ������
#define WM_TVWALL_NOTIFY_START_RES          (WM_USER + 553)     // �첽֪ͨ��ǽ���

#define TIMER_ID_WATER_MASK                   100   //ˮӡУ��Ļ�׼
#define TIMER_ID_WATER_MASK_START             101   //ˮӡУ��timer��ʼid
#define TIMER_ID_WATER_MASK_END               164   //ˮӡУ��timer����id

const int REALTIMEBAR_BUTTON_NUM_CS = 9;

//�¼��ϱ��ĺ궨��
#define EVENT_MOUSE_LEFT_BUTTON_CLICK          10050 //����������¼�
#define EVENT_MOUSE_RIGHT_BUTTON_CLICK         10051 //����Ҽ�����¼�
#define EVENT_EXCHANGE_WINDOW                  10052 //���񽻻��¼�
#define EVENT_MAGNIFY_WINDOW                   10053 //�Ŵ󴰸��¼�
#define EVENT_MINIFY_WINDOW                    10054 //��С�����¼�
#define EVENT_ACTIVE_WINDOW_CHANGE             10055 //�����ı��¼�
#define EVENT_VIDEO_INFO_BTN_CLICK             10056 //��Ƶ��Ϣ��ť����¼�
#define EVENT_VIDEO_STOP_BTN_CLICK             10057 //�ر���Ƶ��ť����¼�
#define EVENT_TOOLBAR_BTN_CLICK                10058 //��������ť����¼�
#define EVENT_ENTER_FULL_SCREEN                10059 //����ȫ���¼�
#define EVENT_EXIT_FULL_SCREEN                 10060 //�˳�ȫ���¼�
#define EVENT_BUTTON_SET_ALARM_WND             10061 //���ø澯�����¼�
#define EVENT_BUTTON_DEL_ALARM_WND             10062 //�˳��澯�����¼�
#define EVENT_BUTTON_SET_TURNCRUISE_PRE        10063 //������ѵ������ǰ��ť�¼�
#define EVENT_BUTTON_DEL_TURNCRUISE_NEXT       10064 //������ѵ�������ť�¼�
#define EVENT_LADDER_LAYOUT_WINDOW             10065 //����ʽ�����¼�
#define EVENT_LOCAL_CAPTURE_SUCCEED			   10066 // ����ץ�ĳɹ�
#define EVENT_LOCAL_CAPTURE_FAILED			   10067 // ����ץ��ʧ��
#define EVENT_LOCAL_CAPTURE_ALL					10068 // ����ץ�����д���
#define EVENT_INTELLIGENT_SELECT_GRAPHIC		10069 // ѡ��ͼ��
#define EVENT_INTELLIGENT_UNSELECT_GRAPHIC		10070 // ȥѡͼ��
#define EVENT_KEYBOARD_PRESS                   10071 //���̰��¼���Ӧ
#define EVENT_KEYBOARD_UP                      10072 //���̼�������Ӧ
#define EVENT_SOUND_STATUS					   10073 // ��·����״̬
#define EVENT_LOCAL_CAPTURE_NO_SPACE		   10074 // ����ץ�Ŀռ䲻��
#define EVENT_POINT_IS_OUTOF_OCX		       10075 // ���Drag�ƶ�����OCX֮��

typedef enum 
{
	TOOLBAR_BTN_PLAY = 1,
	TOOLBAR_BTN_RECORD = 2,
	TOOLBAR_BTN_CAPTURE = 3,
	TOOLBAR_BTN_LABEL = 4,
	TOOLBAR_BTN_SOUND = 5,
	TOOLBAR_BTN_TALKBACK = 6,
	TOOLBAR_BTN_MANIFY = 7,
	TOOLBAR_BTN_PTZ = 8
}TOOlBAR_BTN_TYPE;

#endif
