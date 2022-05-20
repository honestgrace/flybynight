@echo off
REM set path=%path%;c:\tools;d:\lsbuild
set root=C:\gitroot\TestLoc\src\Tutorial
cd %root%\localize
rd temp /s /q
echo Create Temp
md temp

echo Create temporary .resx
resgen %root%\strings.txt temp\strings.txt.resx

echo Parse source file and create a .lcg file in a temporary directory
lsbuild parse /o temp\strings.txt.resx.lcg temp\strings.txt.resx

if not exist extern\base\strings.txt.resx (
    echo First time hand off. Copy the temporary lcg to base directory
    copy /y temp\strings.txt.resx.lcg extern\base
)

echo Merge the .lcg in the temporay with the existing lcg file and create a new lcg in the merge directory
md merge
REM
REM Add various comments and check merging is done correctly
REM 
REM        <Cmts>
REM          <Cmt Name="Dev"><![CDATA[{MaxLength=16}]]></Cmt>
REM          <Cmt Name="LcxAdmin"><![CDATA[BTW, do not localize in Arabic {Locked=1025}]]></Cmt>
REM        </Cmts>
REM
REM /t => base
REM /z => temporary
lsbuild merge /o merge\strings.txt.resx.lcg /t extern\base\strings.txt.resx.lcg /z temp\strings.txt.resx.lcg
