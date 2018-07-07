;�����װ��ִ���ļ����ļ�����,
#define MyAppName "VerifyServer"
;����汾��
;xx.xx.xx--date, year + mouth + day
;.xx--release version to use version, if test version out to 9, then + 1; 
;.xx--test version to out version
#define MyAppVersion "16.05.19.03.4"
#define MyAppPublisher "person, Inc."
;������ɰ�װ���Ŀ�ִ���ļ���
#define MyAppExeName "sop.exe"
;�����Ҫ��������Ļ������ַ���һ��
#define MyProgramsMutexName "VerifyServer-7AB674DA-1001-4B74-8B2C-5FDA50123TE"
#define InstallExeName  "VerifyServer.exe"
#define UninstallExeName  "VerifyServer.exe"
#define ExecName      "verify_launch.exe"

[Setup]
AppMutex={#MyProgramsMutexName} 
;ID��������ͬ��ĿҪ��һ��
AppId={{7AB674DA-1001-4B74-8B2C-5FDA50123TE}
AppName={#MyAppName}
;�汾�ţ�����汾��Ϊ v���Ϻ�#define MyAppVersion "201411251"��ֵ����v201411251
AppVersion={#MyAppVersion}
;��˾��
AppPublisher={#MyAppPublisher}
;Ĭ�ϰ�װ·��, {#MyAppName}ָ��Ϊ�궨��#define MyAppName "SopServer"��ֵ����SopServer
;DefaultDirName=D:\Program Files\{#MyAppName}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
;�û����Э���ļ���·��+�ļ����������û����Э���ļ���ű���ͬһ�ļ����С�����LicenseFile=D:\license.txt
;LicenseFile=license.txt
;��װ�����·����������˼�ǽ���װ���������ű�ͬһĿ¼�µ�sop_setup�ļ�����
OutputDir=verify_setup
;��װ�����ƣ����������verify_setup.exe
OutputBaseFilename=verify_setup
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin


[Languages]
;��Ӷ�������ʱ��Ҫ�ǵ���Ӧ�����[CustomMessages]�µĶ���������ʾ
Name: "chs"; MessagesFile: "compiler:Languages\ChinaeseSimp.isl"
;Name: "en"; MessagesFile: "compiler:Default.isl"

[CustomMessages]
chs.AlreadySetup=��⵽������Ѿ���װ���Ƿ�����ж��?%n�������ǡ�����ж��%n��������˳���װ 
;en.AlreadySetup=this softwares has setup, do Uninstall now?%n click "yes" to Uninstall%nclick "no" to cancel setup
 
chs.DeleteUserdata=�Ƿ�ɾ���������ݣ�
;en.DeleteUserdata=Do you want to delete local data?
[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce; OnlyBelowVersion: 0,6.1

[Files]
;������ļ��������ǽ��ű������ļ����е�bin�ļ���������ļ����
Source: "..\bin\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#ExecName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Dirs]
;����Ӧ�ó����û�����Ŀ¼,  uninsneveruninstallָ��ж�س���ʱ��ɾ��
Name: "{userappdata}\{#MyAppName}"; Flags: uninsneveruninstall

[Registry]
;��UninstallStringд��ע������ڼ���Ƿ��Ѱ�װ��������ͬ��ĿҪ��һ��
Root:HKLM;Subkey:SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FileEncryExpert;ValueType: string; ValueName:VerifyServer_UninstallString;ValueData:{app}\unins000.exe; Flags: uninsdeletevalue
;Root:HKCU;Subkey:SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers;ValueType: string; ValueName:{app}\FtpServer\bin\ftpd.exe;ValueData:RUNASADMIN; Flags: uninsdeletevalue

[Run]
Filename: "{app}\{#InstallExeName}"; Parameters : "--install";  Description: "install verify server";  Flags: runhidden
Filename: "{app}\{#InstallExeName}"; Parameters : "--install";  Flags: nowait postinstall skipifsilent unchecked;
; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
;Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent unchecked

[UninstallRun]
Filename: "{app}\{#UninstallExeName}"; Parameters : "--unstall"; Flags: runhidden;

[Code]
//ж��ʱ����ʾ�Ƿ�ɾ�������������
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  WarnName: String;
begin
    if  CurUninstallStep = usPostUninstall then
            begin
                WarnName :=  CustomMessage('DeleteUserdata');
                if Msgbox(WarnName, mbConfirmation, MB_YESNO) = IDYES then 
                  begin
                    Log(ExpandConstant('{userappdata}\{#MyAppName}'));
                    DelTree(ExpandConstant('{userappdata}\{#MyAppName}'), True, True, True);
                  end; 
             end; 
end;
//��װʱ������Ƿ��Ѱ�װ. 
function InitializeSetup(): Boolean;
var  
  WarnName: String;
  ResultStr: String;  
  ResultCode: Integer;  
begin
  if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\FileEncryExpert', 'UninstallString', ResultStr) then  
  begin  
    WarnName := CustomMessage('AlreadySetup');
    if(Msgbox(WarnName, mbConfirmation, MB_YESNO) = IDYES) then
    begin
        ResultStr := RemoveQuotes(ResultStr);  
        Exec(ResultStr, '/silent', '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        result :=true;  
    end
    else 
    begin
        result := false;
    end; 
  end
  else
  begin
      result := true;
  end;     
end;

//����Ƿ��������������ݣ��������򿽱�Ĭ��
function ShouldSkipPage(PageID: Integer): Boolean;
var
    UserdataDir:String;
    ExistingFileDev:String;
    NewFileDev:String;
    ExistingFileCng:String;
    NewFileCng:String; 

begin
  Log('ShouldSkipPage(' + IntToStr(PageID) + ') called');
  { Skip wpInfoBefore page; show all others }

  case PageID of
    wpFinished:
    begin
      UserdataDir :=  ExpandConstant('{userappdata}\{#MyAppName}');
      if(DirExists(UserdataDir)=false) then begin
          CreateDir(UserdataDir);  
      end; 
      
      NewFileDev :=  ExpandConstant('{userappdata}\{#MyAppName}\sopdev.dat');
      if(FileExists(NewFileDev)= false ) then begin
        ExistingFileDev := ExpandConstant('{app}\sopdev.dat');
        FileCopy(ExistingFileDev, NewFileDev, true);
      end;
          
      NewFileCng := ExpandConstant('{userappdata}\{#MyAppName}\sopcng.dat');   
      if(FileExists(NewFileCng)= false ) then begin
        ExistingFileCng := ExpandConstant('{app}\sopcng.dat');               
        FileCopy(ExistingFileCng, NewFileCng, true);
      end;   
    end;
  end;

  Result := False;  
 end;

