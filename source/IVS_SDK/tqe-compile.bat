::�رջ���
@echo off

set	ProjectPath=%cd%

@echo ������������������������������ʼ����MP�⣭����������������������������
::call ThirdParty\getcomponent.bat 0	
@echo .
@echo ����������������������������ΪJava������������������������������������������
del "%ProjectPath%\IVS_SDK\Rename2Java.h"
call ThirdParty\Rename2Java.exe "%ProjectPath%\inc\IVS_SDK.h" "%ProjectPath%\IVS_SDK\Rename2Java.h"
@echo .
@echo ������������������������������ʼ����Release�������������������������������������
@echo .

"%VS100COMNTOOLS%\..\IDE\devenv.exe" "%ProjectPath%\IVS_SDK.sln" /rebuild "Release|Win32" /out tqeoutput.txt
