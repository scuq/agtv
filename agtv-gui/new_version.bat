@echo off
setlocal enabledelayedexpansion
for /f "tokens=2 delims==" %%G in ('wmic os get localdatetime /value') do set datetime=%%G

set shortyear=%datetime:~2,2%
set month=%datetime:~4,2%
set day=%datetime:~6,2%
set hour=%datetime:~8,2%
set minute=%datetime:~10,2%



echo %shortyear%    > MAJOR.ver
echo %month% 	    > MINOR.ver
echo %day%          > PATCH.ver
echo %hour%%minute% > BUILD.ver



@echo on
echo %shortyear%.%month%.%day%.%hour%%minute%

