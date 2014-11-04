::�رջ���
@echo off

::��õ�ǰʱ�䣬��Ϊ���ɰ汾��Ŀ¼��
for /F "tokens=1-4 delims=-/ " %%i in ('date /t') do (
   set Year=%%i
   set Month=%%j
   set Day=%%k
   set DayOfWeek=%%l
)
for /F "tokens=1-2 delims=: " %%i in ('time /t') do (
   set Hour=%%i
   set Minute=%%j
)

::���ø�������
set   	DateTime=%Year%-%Month%-%Day%-%Hour%-%Minute%
set	WinRarRoot=C:\Program Files\WinRAR
set	ProjectPath=%cd%

@echo on
echo %DateTime%
@echo .
@echo ������������������������������ʼ����MP�⣭����������������������������
::call ThirdParty\getcomponent.bat 0	

@echo ����������������������������ɾ��ThirdParty�µ�MicroSoft��������������������������������
del /Q "%ProjectPath%\ThirdParty\cabarc.Exe"
del /Q "%ProjectPath%\ThirdParty\basicfiles\IEShims.dll"
del /Q "%ProjectPath%\ThirdParty\basicfiles\mfc90.dll"
del /Q "%ProjectPath%\ThirdParty\basicfiles\msvcp90.dll"
del /Q "%ProjectPath%\ThirdParty\basicfiles\msvcp100.dll"
del /Q "%ProjectPath%\ThirdParty\basicfiles\msvcr90.dll"
del /Q "%ProjectPath%\ThirdParty\basicfiles\msvcr100.dll"
del /Q "%ProjectPath%\ThirdParty\basicfiles\wer.dll"

@echo .
@echo ������������������������������ʼ����MicroSoft��ThirdParty������������������������������
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\cabarc.Exe"                 "ThirdParty\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\IEShims.dll"     "ThirdParty\basicfiles\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\mfc90.dll"       "ThirdParty\basicfiles\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\msvcp90.dll"     "ThirdParty\basicfiles\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\msvcp100.dll"    "ThirdParty\basicfiles\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\msvcr90.dll"     "ThirdParty\basicfiles\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\msvcr100.dll"    "ThirdParty\basicfiles\"
xcopy /y /i /r /s "..\..\..\platform\MicroSoft\basicfiles\wer.dll"         "ThirdParty\basicfiles\"

@echo .
@echo ������������������������������ʼ����Release�������������������������������������
@echo .

"%VS100COMNTOOLS%\..\IDE\devenv.exe" "%ProjectPath%\IVS_SDK.sln" /rebuild "Release|Win32" /out output.txt

@echo .
@echo �����������������������������������������ɣ�������������������������������	
@echo .
@echo ������������������������������ʼ����汾��������������������������������	
@echo .
mkdir tempversion
@echo .
@echo ������������������������������ʼ�����汾��������������������������������	
@echo .
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\ACE.dll"           "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\bp_base.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\bp_frame.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\bp_log.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\bp_net.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\ivs_base.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\ivs_compression.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_IADraw.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_SDKTVWall.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_SDKNetKeyBoard.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\ivs_security.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\ivs_sftp.dll"		"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\libeay32.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\libssh2.dll"		"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\nss.dll"	        "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\rtsp_client.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\ssleay32.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\zlib.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_SDK.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_OCXPlayer.ocx"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_NetSource.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_Player.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\mp4DLL.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\regsvrOCX.bat"    	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\mp_log.conf"		"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\cudart32_42_9.dll"	"tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\libmpg123.dll"	    "tempversion\bin\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\AnalyzeData.dll"	"tempversion\bin\"

@echo .
@echo ������������������������������ʼ����Lib�ļ���������������������������������	
@echo .
xcopy /E /y /i /r /s "build\windows\release\IVS_SDK\bin\lib\protocols"      "tempversion\bin\lib\protocols"
xcopy /E /y /i /r /s "build\windows\release\IVS_SDK\bin\lib\services"	    "tempversion\bin\lib\services"
xcopy /E /y /i /r /s "build\windows\release\IVS_SDK\bin\Skin"              "tempversion\bin\Skin"
xcopy /E /y /i /r /s "build\windows\release\IVS_SDK\bin\i18n"              "tempversion\bin\i18n"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\defaultConfig\LocalConfig.xml"              "tempversion\bin\defaultConfig\"

@echo .
@echo ������������������������������ʼ���������ļ���������������������������������	
@echo .
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\config\log.xml"        "tempversion\bin\config\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\config\net.xml"	    "tempversion\bin\config\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\config\service.xml"	"tempversion\bin\config\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\config\SDKConfig.xml"  "tempversion\bin\config\"

@echo .
@echo ������������������������������ʼ����SDK ͷ�ļ���������������������������������	
@echo .

::IVS_SDK.h��buildĿ¼�¿���������Ҫ�����������ͷ�ļ� 2014-7-8 zwx211831
xcopy /y /i /r /s "build\windows\include\IVS_SDK.h"                   	"tempversion\include\"
xcopy /y /i /r /s "inc\IVS_SDKTVWall.h"                 "tempversion\include\"
::xcopy /y /i /r /s "inc\IVS_SDKNetKeyBoard.h"            "tempversion\include\"
::xcopy /y /i /r /s "IVS_SDKNetKeyBoard\inc\IVSSDKNetKeyBoardDef.h"            "tempversion\include\"
xcopy /y /i /r /s "..\..\..\platform\IVS_COMMON\inc\hwsdk.h"	    	"tempversion\include\"
xcopy /y /i /r /s "..\..\..\platform\IVS_COMMON\inc\ivs_error.h"   	"tempversion\include\"



@echo .
@echo ������������������������������ʼ����SDK lib�ļ���������������������������������	
@echo .
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_SDK.lib"       "tempversion\sdklib\"

@echo .
@echo ������������������������������ʼ����pdb��������������������������������	
@echo .
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_NetSource.pdb"	"tempversion\pdb\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_OCXPlayer.pdb"	"tempversion\pdb\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_Player.pdb"	"tempversion\pdb\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_SDK.pdb"	    "tempversion\pdb\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\rtsp_client.pdb"	"tempversion\pdb\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\mp4DLL.pdb"	    "tempversion\pdb\"
xcopy /y /i /r /s "build\windows\release\IVS_SDK\bin\IVS_IADraw.pdb"	"tempversion\pdb\"

cd tempversion
"%WinRarRoot%\WinRAR.exe" a -r eSDK_IVS_API_V1.3.30_Windows.zip .\
move eSDK_IVS_API_V1.3.30_Windows.zip ..\
cd ..
rd /Q /S tempversion

mkdir version
move eSDK_IVS_API_V1.3.30_Windows.zip .\version\

@echo ������������������������������ʼ���CAB�汾��������������������������������	
@echo .

mkdir tempcabversion\IVS_SDK
@echo .
"%WinRarRoot%\WinRAR.exe" x -y "version\eSDK_IVS_API_V1.3.30_Windows.zip"  tempcabversion\IVS_SDK
::����֧�ſ⵽��ӦĿ¼��
::xcopy /E /y /i /r /s "tempcabversion\IVS_SDK\config"      		"tempcabversion\IVS_SDK\bin\config"
::xcopy /E /y /i /r /s "tempcabversion\IVS_SDK\lib"	        	"tempcabversion\IVS_SDK\bin\lib"

@echo ��������������������������������web�����ļ���������������������������������
::xcopy /y /i /r /s "ThirdParty\basicfiles\Alarm.wav"	                    	"tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\IEShims.dll"	                    "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\IVS_OCXPlayer.inf"	            	"tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\License_Information.docx"	        "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\mfc90.dll"	                    	"tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\Microsoft.VC90.CRT.manifest"	    "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\msvcp90.dll"	                    "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\msvcp100.dll"	                    "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\msvcr90.dll"	                    "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\msvcr100.dll"	                    "tempcabversion\IVS_SDK\bin\"
xcopy /y /i /r /s "ThirdParty\basicfiles\wer.dll"	                        "tempcabversion\IVS_SDK\bin\"

mkdir versioncab
cd tempcabversion\IVS_SDK\bin
%ProjectPath%\ThirdParty\cabarc -r -p N %ProjectPath%\versioncab\IVS_OCX.cab *

cd %ProjectPath%\versioncab\
"%WinRarRoot%\WinRAR.exe" a -r eSDK_IVS_OCX_V1.3.30_Windows.zip .\
del /Q .\IVS_OCX.cab

cd %ProjectPath%
rd /Q /S tempcabversion

@echo ��������������������������������汾��ɣ�������������������������������	
@echo off

@echo ��������������������������������������Ŀ¼��������������������������������
::������ڵ���3�㣬
if "%Hour%" geq "03" goto CheckifXiaoyudengyu7dian
goto CopytoDebugDir
::�ж��Ƿ�С�ڵ���7�㣬�����
:CheckifXiaoyudengyu7dian
if "%Hour%" leq "07" goto CopytoReleaseDir
goto CopytoDebugDir
:CopytoReleaseDir
::���ڵ���3��С�ڵ���7��ʱ����ڵİ��ŵ�����������+ת�⡱Ŀ¼��
xcopy /y /i /r /s  "%ProjectPath%\version\eSDK_IVS_API_V1.3.30_Windows.zip"  "%ProjectPath%\..\R3�汾Release\%Year%%Month%%Day%ת��\Windows\SDK\"
xcopy /y /i /r /s  "%ProjectPath%\versioncab\eSDK_IVS_OCX_V1.3.30_Windows.zip"  "%ProjectPath%\..\R3�汾Release\%Year%%Month%%Day%ת��\Windows\OCX\"
::���Ƶ��Զ�������ָ����Ŀ¼
copy /y "%ProjectPath%\version\eSDK_IVS_API_V1.3.30_Windows.zip"  "D:\ftpServer\IVS_Client\IVS_SDK.rar"
goto End
::С��3������7��ʱ����ڵİ��ŵ����������ڡ�Ŀ¼��
:CopytoDebugDir
xcopy /y /i /r /s  "%ProjectPath%\version\eSDK_IVS_API_V1.3.30_Windows.zip"  "%ProjectPath%\..\R3�汾Release\%Year%%Month%%Day%\Windows\SDK\"
xcopy /y /i /r /s  "%ProjectPath%\versioncab\eSDK_IVS_OCX_V1.3.30_Windows.zip"  "%ProjectPath%\..\R3�汾Release\%Year%%Month%%Day%\Windows\OCX\"
:End
@echo off
