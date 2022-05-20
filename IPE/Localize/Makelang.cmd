@echo off
REM ----------------------------------------------------------------------
REM Microsoft CRM International Build Kit for HelloWorld
REM MakeLang.cmd
REM ----------------------------------------------------------------------

echo.
echo Microsoft (R) CRM International Build Kit for HelloWorld
echo Copyright (C) Microsoft Corporation 2002-2020. All rights reserved.
echo.


REM Check command-line parameters
REM ----------------------------------------------------------------------

if "%1"=="/?" goto :DisplayUsage
if "%1"=="" goto :DisplayUsage


REM ----------------------------------------------------------------------

call :ParseArguments %*

REM init
set LOC_PNAME=HelloWorld
set LOC_LCID=
set LOC_CULTURE=
set LOC_TRG_FILE=
set LOC_BASEDONE=
set LOC_TMPPATH=%~dp0Temp
if exist "%LOC_TMPPATH%" rd /S /Q "%LOC_TMPPATH%

REM Set language options
REM if Arabic
REM - LOC_CULTURE=ar

set LOC_LCID=%1

for /f "usebackq  eol=! tokens=1 delims= " %%i in (%~dp0language.txt) do (
   if /i "%%i"=="%LOC_LCID%" (
       set LOC_CULTURE=%%i
    )
)


REM ----------------------------------------------------------------------
:CheckArguments

if /i NOT "All"=="%LOC_LCID%" (
   if NOT DEFINED LOC_CULTURE (
       echo.
       echo Error: Specified LCID is not valid.
       echo        Please specify all or supported languages in language.txt.
       echo.
       goto :DisplayUsage
   )
)


REM Initialize the localization kit
REM ----------------------------------------------------------------------
:Init

set LSver=6.12.6202
set LSBuild=CRM.LSBuild
set inetroot=%~dp0
set inetroot=%inetroot:\Localize\=%
if not exist "%~dp0packages\%LSBuild%.%LSver%" (
    call nuget install -OutputDirectory "%~dp0packages"
)
set PKG_LSBUILD=%~dp0packages\%LSBuild%.%LSver%

if not exist "%PKG_LSBUILD%" (
       echo.
       echo Error: No LSBuild available.
       echo.
       goto :DisplayUsage
)

set LOC_LOCPATH=%~dp0Extern
if not exist "%LOC_TMPPATH%" (
    md "%LOC_TMPPATH%"
) else ( 
    if exist "%LOC_TMPPATH%\target.txt" del /f /q "%LOC_TMPPATH%\target.txt"
)
REM Check 
if NOT DEFINED LOC_SRCPATH (
    echo.
    echo Use default path - %inetroot%
    echo.
    set LOC_SRCPATH=%inetroot%
)

REM Get relative Path 
call :Get_CharCount %inetroot%

setlocal ENABLEDELAYEDEXPANSION
for /f "usebackq  eol=! tokens=1 delims=," %%i in (%~dp0filelist.txt) do (
    call :Get_RelativePath "%LOC_SRCPATH%\%%i" %LOC_charcount%
)
endlocal

if /i "All"=="%LOC_LCID%" (
    for /f "usebackq  eol=! tokens=1 delims= " %%i in (%~dp0language.txt) do (
        set LOC_CULTURE=%%i
        call :RunlsBuild
        set LOC_BASEDONE=true
    )
    call :lsBuildGenerate lsbuild.response
    call robocopy "%LOC_LOCPATH%\Base\%LOC_PNAME%" "%LOC_TMPPATH%\Base\%LOC_PNAME%" *.lcg /s
    call :RunCmd copytarget.cmd
) else (
    for /f "usebackq  eol=! tokens=1 delims= " %%i in (%~dp0language.txt) do (
        if /i "%%i" == "%LOC_LCID%" (
            set LOC_CULTURE=%%i
            call :RunlsBuild
        )
    )
    call :lsBuildGenerate lsbuild.response
    call robocopy "%LOC_LOCPATH%\Base\%LOC_PNAME%" "%LOC_TMPPATH%\Base\%LOC_PNAME%" *.lcg /s
    call :RunCmd copytarget.cmd
)
exit /b

REM 
REM Process all the files under target language
REM 
:Runlsbuild
echo.
echo -------------------------------------
echo      Build Info
echo.
echo Language = %LOC_CULTURE%%LOC_PL_FULL%
echo  SRCPath = %LOC_SRCPATH%
echo -------------------------------------
echo.

setlocal ENABLEDELAYEDEXPANSION
echo.
echo Searching target files...
echo.

if exist "%LOC_TMPPATH%\%LOC_CULTURE%" rd /S /Q "%LOC_TMPPATH%\%LOC_CULTURE%"

REM 
REM Generate target.txt
REM 

for /f "tokens=1,2 delims=," %%o in (%LOC_TMPPATH%\target.txt) do (
    set LOC_PATH=%%p
    set LOC_FILE=%%~no%%~xo
    set LOC_SOURCE=%%o
    set LOC_TARGET=%%o
    set LOC_COMPNAME=%%p

    if DEFINED LOC_EXTRACT (
        if NOT DEFINED LOC_BASEDONE (
            REM ExtractOnly - Copy the source file to Master folder
            echo "Extracting source"
            call robocopy "%%~do%%~po\" "%LOC_TMPPATH%\Master\%LOC_PNAME%\!LOC_COMPNAME!\" !LOC_FILE!
        )
    )
    REM Set LSBuild Parser and Target file name
    REM Default to RESX Parser
    set LOC_PARSER=211
    if /i "%%~xo" == ".json" (
        set LOC_PARSER=306
    )

    if not "!LOC_PATH:\en\=!" == "!LOC_PATH!" (
        set LOC_TARGETFILE=!LOC_FILE:\en\=\%LOC_CULTURE%\!
        set LOC_PATH=!LOC_PATH:\en\=\%LOC_CULTURE%\!
        set LOC_TARGET=!LOC_TARGET:\en\=\%LOC_CULTURE%\!
    ) else (
        set LOC_TARGETFILE=%%~no.%LOC_CULTURE%%%~xo
        set LOC_TARGET=!LOC_TARGET:%%~no%%~xo=%%~no.%LOC_CULTURE%%%~xo!
    )

    if exist "%LOC_LOCPATH%\Base\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcg" (
        if DEFINED LOC_PL_FULL (
            if NOT DEFINED LOC_BASEDONE (
                echo parse /p !LOC_PARSER! /s "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS\lss.lss" /o "%LOC_TMPPATH%\Intermidiate\Parse\!LOC_COMPNAME!\!LOC_FILE!.lcl" !LOC_SOURCE!>>%LOC_TMPPATH%\lsbuild.response
                echo merge /s "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS\lss.lss" /o "%LOC_TMPPATH%\Intermidiate\Merged\!LOC_COMPNAME!\!LOC_FILE!.lcl" /z "%LOC_TMPPATH%\Intermidiate\Parse\!LOC_COMPNAME!\!LOC_FILE!.lcl" /t "%LOC_LOCPATH%\Base\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcg">>%LOC_TMPPATH%\lsbuild.response
                echo update /s "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS\lss.lss" /i "%LOC_TMPPATH%\Intermidiate\Merged\!LOC_COMPNAME!\!LOC_FILE!.lcl" "%LOC_LOCPATH%\Base\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcg">>%LOC_TMPPATH%\lsbuild.response
            )
        )
        if exist "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcl" (
            if DEFINED LOC_PL_FULL (
                echo update /s "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\LSS\lss.lss" /i "%LOC_TMPPATH%\Intermidiate\Merged\!LOC_COMPNAME!\!LOC_FILE!.lcl" "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcl">>%LOC_TMPPATH%\lsbuild.response
            )
            if NOT DEFINED LOC_EXTRACT (
                echo generate %LOC_PL_FULL% /novalidate /iu /ol "%LOC_TMPPATH%\%LOC_CULTURE%\!LOC_COMPNAME!\target\!LOC_TARGETFILE!.lcl" /d %LOC_CULTURE% /s "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\LSS\lss.lss" /o "%LOC_TMPPATH%\%LOC_CULTURE%\!LOC_PATH!\!LOC_FILE!" /t "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcl" !LOC_SOURCE!>>%LOC_TMPPATH%\lsbuild.response
                REM Generate copy command
                if NOT EXIST "%LOC_SRCPATH%!LOC_PATH!" md "%LOC_SRCPATH%!LOC_PATH!"
                echo call copy "%LOC_TMPPATH%\%LOC_CULTURE%!LOC_PATH!!LOC_FILE!" !LOC_TARGET! /Y>>%LOC_TMPPATH%\copytarget.cmd
            )
        ) else (
            REM New language added - generate lcl files for new languages
            if DEFINED LOC_EXTRACT (
                if NOT EXIST %LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\LSS md %LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\LSS
                echo call robocopy "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS" "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\LSS" *.lss /s>>%LOC_TMPPATH%\copytarget.cmd
                echo generate /novalidate /iu /ol "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcl" /d %LOC_CULTURE% /s "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS\lss.lss" /o "%LOC_TMPPATH%\%LOC_CULTURE%\!LOC_PATH!\!LOC_FILE!" /t "%LOC_LOCPATH%\Base\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcg" !LOC_SOURCE!>>%LOC_TMPPATH%\lsbuild.response
            )
        )
    ) else (
        REM New component added - generate lcl files for Base and languages
        if DEFINED LOC_EXTRACT (
            if NOT DEFINED LOC_BASEDONE (
                echo parse /p !LOC_PARSER! /s "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS\lss.lss" /o "%LOC_LOCPATH%\Base\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcg" !LOC_SOURCE!>>%LOC_TMPPATH%\lsbuild.response
            )
            echo generate /novalidate /iu /ol "%LOC_LOCPATH%\%LOC_CULTURE%\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcl" /d %LOC_CULTURE% /s "%LOC_LOCPATH%\Base\%LOC_PNAME%\LSS\lss.lss" /o "%LOC_TMPPATH%\%LOC_CULTURE%\!LOC_PATH!\!LOC_FILE!" /t "%LOC_LOCPATH%\Base\%LOC_PNAME%\!LOC_COMPNAME!\!LOC_FILE!.lcg" !LOC_SOURCE!>>%LOC_TMPPATH%\lsbuild.response
        )
    )
)

endlocal
goto :eof

:lsBuildGenerate
if EXIST %LOC_TMPPATH%\%1 (
    call %PKG_LSBUILD%\lsbuild response %LOC_TMPPATH%\%1
) else (
    if NOT DEFINED LOC_EXTRACT (
        echo Error - Solution Name does not exist
    )
    exit /b
)
goto :eof


REM ----------------------------------------------------------------------
REM RunCmd
:RunCmd

REM Copy to final destination
if exist %LOC_TMPPATH%\%1 call %LOC_TMPPATH%\%1

echo.
echo Process completed.
echo.

goto :eof


REM ----------------------------------------------------------------------
REM Parse command-line options
:ParseArguments

set LOC_PL_FULL=
set LOC_FILE=
set LOC_SRCPATH=
set LOC_EXTRACT=

shift
:ArgumentsLoopBegin
if "%~1"=="" goto :ArgumentsLoopEnd

if /i "%~1" EQU "/FullPL" (
    set LOC_PL_FULL=/Pseudo
) else (
    if /i "%~1" EQU "/handoff" (
        set LOC_EXTRACT=1
        set LOC_PL_FULL=/Pseudo
    ) else (
        if /i "%~1" EQU "/path" (
            if "%~2"=="" goto :DisplayUsage
            set LOC_SRCPATH=%~2
        )
    )
)
shift
goto :ArgumentsLoopBegin

:ArgumentsLoopEnd
shift /0
goto :eof


:Get_CharCount
set s=%1
set LOC_charcount=0
:LOOP_GCC
if defined s (
    set s=%s:~1%
    set /a LOC_charcount+=1
    goto :LOOP_GCC
)
exit /b



:Get_RelativePath
set LOC_R_PATH=%~p1
set /a ch=%2
:LOOP_GRP
if not 2==%ch% (
    set LOC_R_PATH=%LOC_R_PATH:~1%
    set /a ch-=1
    goto :LOOP_GRP
)
    if not exist "%LOC_TMPPATH%" md "%LOC_TMPPATH%" 
    echo>>"%LOC_TMPPATH%\target.txt" %1,%LOC_R_PATH%
exit /b


REM ----------------------------------------------------------------------
:DisplayUsage
echo ----------------------------------------------------------------------
echo Usage: LOC_MakeLang LCID [options]
echo.
echo        LCID            is a language code such as de or fr. specify all to process 
echo                        all language. (Required)
echo                        Specify All to process all languages
echo.
echo Options:
echo        /FullPL         indicates that all strings should be pseudo-localized.
echo                        With this option, the language-specific LCLs are not used. 
echo                        The build kit uses generic PL LCLs in conjunction with
echo                        language-specific LSS settings.
echo.
echo        /handoff        For IPE use only.
echo                        With this option, Base file and Master files will be copied to 
echo                        Temp folder and can be used for localization hand off.
echo.
echo        /path folder    indicates root folder which contains target files. (Optional)
echo                        If omitted, default path (%inetroot%\src) will be used and process
echo                        all the files exposed to localization.  
echo.
echo.
echo Example:
echo        LOC_MakeLang fr /FullPL
echo          - Result = Generate French Pseudo for all the files exposed to localization.
echo.
echo        LOC_MakeLang All /path c:\foo\ver
echo          - Result = Genererate all the target file under c:\foo\ver for all languages.
echo.
exit /b 1