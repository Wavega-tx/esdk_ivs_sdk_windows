::�رջ���
@echo off
::���ø�������
set	ProjectPath=%cd%
@echo on
@echo .
@echo ����������������������������ΪJava������������������������������������������
del "%ProjectPath%\IVS_SDK\Rename2Java.h"
call ThirdParty\Rename2Java.exe "%ProjectPath%\inc\IVS_SDK.h" "%ProjectPath%\IVS_SDK\Rename2Java.h"
@echo .
@echo off
