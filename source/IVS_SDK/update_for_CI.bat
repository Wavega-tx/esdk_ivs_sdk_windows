::���ø�������
set	SVNROOT=%svn_path%
cd "D:\R2_File\eSpace_IVS_V100R002C01\IVS_SDK"

@echo on
@echo . /noprogressui /nodlg /noui 
@echo ������������������������������ʼ����Ŀ¼������������������������������
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:cleanup /path:"D:\R2_File\eSpace_IVS_V100R002C01\IVS_SDK" /noui /noprogressui /nodlg 
@echo ����������������������������������ɣ�����������������������������
@echo . 
@echo ������������������������������ʼ���£�����������������������������
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:update /path:"D:\R2_File\eSpace_IVS_V100R002C01\IVS_SDK" /closeonend:2  
@echo ����������������������������������ɣ�����������������������������