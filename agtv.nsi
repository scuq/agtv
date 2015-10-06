; agtv.nsi
!include "MUI.nsh"
; -------------------------------
; Start
 
  !define company "AbyleDotOrg"
  !define icon "agtv.ico"
  !define MUI_PRODUCT "agtv"
  !define MUI_FILE "agtv"
  !define MUI_VERSION "15.1002"
  !define MUI_BRANDINGTEXT "agtv"
  CRCCheck On
  



; The name of the installer
Name "agtv"

; The file to write
OutFile "agtv-15.1001-hexchat-vlcportable-x86-install.exe"

!define MUI_ICON "agtv.ico"
!define MUI_UNICON "agtv.ico"

; The default installation directory
InstallDir $PROGRAMFILES\agtv

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "install" ;No components page, name is not important



  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /r *.*

	;create desktop shortcut
	CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" ""
	
	
	 WriteUninstaller "$INSTDIR\Uninstall.exe"
	 
	 ;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
  
SectionEnd ; end the section

;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
 
;Delete Files 
  RMDir /r "$INSTDIR\*.*"    
 
;Remove the installation directory
  RMDir "$INSTDIR"
 
;Delete Start Menu Shortcuts
  Delete "$DESKTOP\agtv.lnk"

 
;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${MUI_PRODUCT}"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"  
 
SectionEnd
