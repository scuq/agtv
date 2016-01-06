; agtv.nsi
!include "MUI2.nsh"

; -------------------------------
; Start
!define /file VERSION "version.txt"
!define /file ARCH "arch.txt"
!define company "AbyleDotOrg"
!define icon "agtv.ico"
!define MUI_PRODUCT "agtv"
!define MUI_FILE "agtv"
!define MUI_VERSION ${VERSION}
!define MUI_BRANDINGTEXT "agtv"

CRCCheck On

; The name of the installer
Name "agtv"

; The file to write
OutFile "agtv-${VERSION}-core-${ARCH}-installer.exe"

!define MUI_ICON "agtv.ico"
!define MUI_UNICON "agtv.ico"

; The default installation directory
InstallDir $PROGRAMFILES\agtv

!insertmacro MUI_LANGUAGE "English"
;--------------------------------


; Pages
Page directory
Page components
Page instfiles


!include "Sections.nsh"


Function .onInit

Execwait '"$SYSDIR\taskkill.exe" /F /IM agtv.exe /T'

IfFileExists $PROGRAMFILES\VideoLan\VLC\vlc.exe uncheckVlc checkVlc


        checkVlc:
                SectionSetFlags 1 1
                goto vlcend

        uncheckVlc:
                SectionSetFlags 1 0


        IfFileExists $PROGRAMFILES32\VideoLan\VLC\vlc.exe uncheckVlc2 checkVlc2


        checkVlc2:
                SectionSetFlags 1 1
                goto vlcend

        uncheckVlc2:
                SectionSetFlags 1 0
                goto vlcend

        vlcend:


IfFileExists $PROGRAMFILES\Hexchat\hexchat.exe uncheckHexchat checkHexchat


        checkHexchat:
                SectionSetFlags 2 1
                goto hexchatend

        uncheckHexchat:
                SectionSetFlags 2 0


        IfFileExists $PROGRAMFILES32\VideoLan\VLC\vlc.exe uncheckHexchat2 checkHexchat2


        checkHexchat2:
                SectionSetFlags 2 1
                goto hexchatend

        uncheckHexchat2:
                SectionSetFlags 2 0
                goto hexchatend

        hexchatend:

FunctionEnd



; The stuff to install
Section "agtv" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  RMDir /r "$INSTDIR\3rdparty-addons"

  ; Put file there
   File /r /x *.txt /x *.bat *.*

        ;create desktop shortcut
        CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" ""


         WriteUninstaller "$INSTDIR\Uninstall.exe"

         ;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"

SectionEnd ; end the section


Section /o "vlc (open in browser)" vlc ;No components page, name is not important
execshell open "http://www.videolan.org"

SectionEnd ; end the section

Section /o "hexchat (open in browser)" hexchat ;No components page, name is not important
        execshell open "https://hexchat.github.io/"
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
