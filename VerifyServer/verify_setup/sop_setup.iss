;软件安装可执行文件根文件夹名,
#define MyAppName "VerifyServer"
;软件版本号
;xx.xx.xx--date, year + mouth + day
;.xx--release version to use version, if test version out to 9, then + 1; 
;.xx--test version to out version
#define MyAppVersion "16.05.19.03.4"
#define MyAppPublisher "person, Inc."
;被打包成安装包的可执行文件名
#define MyAppExeName "sop.exe"
;这个需要与程序代码的互斥量字符串一致
#define MyProgramsMutexName "VerifyServer-7AB674DA-1001-4B74-8B2C-5FDA50123TE"
#define InstallExeName  "VerifyServer.exe"
#define UninstallExeName  "VerifyServer.exe"
#define ExecName      "verify_launch.exe"

[Setup]
AppMutex={#MyProgramsMutexName} 
;ID，其他不同项目要不一样
AppId={{7AB674DA-1001-4B74-8B2C-5FDA50123TE}
AppName={#MyAppName}
;版本号，这里版本号为 v加上宏#define MyAppVersion "201411251"的值，即v201411251
AppVersion={#MyAppVersion}
;公司名
AppPublisher={#MyAppPublisher}
;默认安装路径, {#MyAppName}指代为宏定义#define MyAppName "SopServer"的值，即SopServer
;DefaultDirName=D:\Program Files\{#MyAppName}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
;用户许可协议文件，路径+文件名，这里用户许可协议文件与脚本在同一文件夹中。例：LicenseFile=D:\license.txt
;LicenseFile=license.txt
;安装包输出路径，这里意思是将安装包输出到与脚本同一目录下的sop_setup文件夹中
OutputDir=verify_setup
;安装包名称，这里输出名verify_setup.exe
OutputBaseFilename=verify_setup
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin


[Languages]
;添加多种语言时，要记得相应的添加[CustomMessages]下的多种语言提示
Name: "chs"; MessagesFile: "compiler:Languages\ChinaeseSimp.isl"
;Name: "en"; MessagesFile: "compiler:Default.isl"

[CustomMessages]
chs.AlreadySetup=检测到本软件已经安装，是否现在卸载?%n单击“是”现在卸载%n点击“否”退出安装 
;en.AlreadySetup=this softwares has setup, do Uninstall now?%n click "yes" to Uninstall%nclick "no" to cancel setup
 
chs.DeleteUserdata=是否删除本地数据？
;en.DeleteUserdata=Do you want to delete local data?
[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce; OnlyBelowVersion: 0,6.1

[Files]
;被打包文件，这里是将脚本所在文件夹中的bin文件夹里面的文件打包
Source: "..\bin\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#ExecName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Dirs]
;创建应用程序用户数据目录,  uninsneveruninstall指定卸载程序时不删除
Name: "{userappdata}\{#MyAppName}"; Flags: uninsneveruninstall

[Registry]
;将UninstallString写入注册表，用于检测是否已安装，其他不同项目要不一样
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
//卸载时，提示是否删除软件本地数据
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
//安装时，检测是否已安装. 
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

//检测是否存在软件本地数据，不存在则拷贝默认
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

