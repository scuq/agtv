set QT_ROOT_PATH=C:\Qt\5.5\msvc2013
set VLC_QT_PATH=Z:\VLC-Qt
set SSL_DIR=C:\OpenSSL-Win32
set BUILD_DIR=Z:\build\agtv
set BUILD_DRIVE=Z:

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DQT_ROOT_PATH=%QT_ROOT_PATH% -DVLC_QT_PATH=%VLC_QT_PATH% -DSSL_DIR=%SSL_DIR% -B%BUILD_DIR%


%BUILD_DRIVE%
cd %BUILD_DIR%

nmake package

