REM References
REM 1. MSDN
REM    https://docs.microsoft.com/en-us/dotnet/framework/resources/retrieving-resources-in-desktop-apps
REM 2. Microsoft LsBuild
REM    https://microsoft.sharepoint.com/teams/celoc4dev/SitePages/About%20Managed%20Code%20Localization.aspx

REM Adjust path environment variable
REM set path=%path%;c:\tools;d:\lsbuild

REM ##############################
REM Build program
REM ##############################
resgen strings.txt MyResource.resources
csc GetString.cs -resource:MyResource.resources
getstring.exe

REM ##############################
REM Build satellite assemblies with the localized txt files
REM ##############################
resgen strings.en-US.txt MyResource.en-US.resources
md en-US  
al.exe -embed:MyResource.en-US.resources -culture:en-US -out:en-US\GetString.resources.dll  
resgen strings.fr-FR.txt MyResource.fr-FR.resources
md fr-FR  
al.exe -embed:MyResource.fr-FR.resources -culture:fr-FR -out:fr-FR\GetString.resources.dll  
resgen strings.ru-RU.txt MyResource.ru-RU.resources
md ru-RU  
al.exe -embed:MyResource.ru-RU.resources -culture:ru-RU -out:ru-RU\GetString.resources.dll  
resgen strings.ko-KR.txt MyResource.ko-KR.resources 
md ko-KR
al.exe -embed:MyResource.ko-KR.resources -culture:ko-KR -out:ko-KR\GetString.resources.dll  
getstring.exe

REM ##############################
REM Convert .txt to .resx and use al.exe to generate a satellite assembly with a resx file which is localized
REM ##############################
del ko-KR\GetString.resources.dll
md temp
resgen strings.txt temp\exported.resx
REM Open temp\exported.resx and replace with new value
REM Or, use the backup file to do that
fc exported.resx.backup temp\exported.resx 
copy /y exported.resx.backup temp\exported.resx 
resgen temp\exported.resx temp\MyResource.ko-KR.resources
al.exe -embed:temp\MyResource.ko-KR.resources -culture:ko-KR -out:ko-KR\GetString.resources.dll  
getstring.exe

REM ##############################
REM Pre-compilation
REM Generate .resx from a .txt file.
REM Use lsbuild.exe to generate a .lcl file from a .resx file
REm Use lsbuild.exe to generate localized .resx file with the .lcl file that is handed back 
REm Use the resgen.exe to generate a .resources file with a .resx file which is localized.
REm Use the al.exe to generate a satellite assembly with the .resources file
REM ##############################
del ko-KR\GetString.resources.dll
resgen strings.txt exported.resx
lsbuild parse /o exported.resx.lcl exported.resx
REM Open exported.resx.lcl and add new value
REM          <Tgt Cat="Text" Stat="Loc" Orig="New" AutoAppr="N/A">
REM            <Val><![CDATA[New data here]]></Val>
REM          </Tgt>
REM Or, use the backup file to do that
fc exported.resx.lcl.backup exported.resx.lcl
copy /y exported.resx.lcl.backup exported.resx.lcl
lsbuild generate /w 0 /iu /novalidate /d 1042 /o temp\exported.resx /t exported.resx.lcl exported.resx
fc temp\exported.ko-KR.resx exported.resx
resgen temp\exported.ko-KR.resx MyResource.ko-KR.resources
al -embed:MyResource.ko-KR.resources -culture:ko-KR -out:ko-KR\GetString.resources.dll
getstring.exe

REM ##############################
REM Pre-compilation
REM Use lsbuild.exe to generate a .lcl file from a .resources file
REm Use lsbuild.exe to generate localized .resources file with the .lcl file that is handed back 
REm Use the al.exe to generate a satellite assembly with a .resources file which is localized.
REM ##############################
del ko-KR\GetString.resources.dll 
del MyResource.ko-KR.resources
lsbuild parse /o MyResource.resources.ko-KR.lcl MyResource.resources
REM Open MyResource.resources.lcl and add new value
REM          <Tgt Cat="Text" Stat="Loc" Orig="New" AutoAppr="N/A">
REM            <Val><![CDATA[New data here]]></Val>
REM          </Tgt>
REM Or, use the backup file to do that
fc MyResource.resources.ko-KR.lcl.backup MyResource.resources.ko-KR.lcl
copy /y MyResource.resources.ko-KR.lcl.backup MyResource.resources.ko-KR.lcl
lsbuild generate /w 0 /iu /novalidate /d 1042 /o MyResource.ko-KR.resources /t MyResource.resources.ko-KR.lcl MyResource.resources
dir MyResource.ko-KR.resources
al -embed:MyResource.ko-KR.resources -culture:ko-KR -out:ko-KR\GetString.resources.dll
getstring.exe

REM ##############################
REM Compare /ol switch
REM ##############################
lsbuild generate /w 0 /iu /novalidate /d 1042 /o temp\exported.resx /ol temp\exported.resx.lct /t exported.resx.lcl exported.resx
lsbuild generate /w 0 /iu /novalidate /d 1042 /o temp\exported.resx /ol temp\exported.resx.lcg /t exported.resx.lcl exported.resx
fc exported.resx.lcl temp\exported.resx.lct
fc exported.resx.lcl temp\exported.resx.lcg
REM windiff exported.resx.lcl temp\exported.resx.lcx

REM ##############################
REM Pre-compilation
REM Use lsbuild.exe to generate a .lcl file from a .exe file
REM Post-compilation
REm Use lsbuild.exe to generate a satellite assembly with the .lcl file that is handed back 
REM ##############################
del ko-KR\GetString.resources.dll
lsbuild parse /p 211 /o getstring.exe.lcl getstring.exe
REM Open exported.resx.lcl and add new value
REM          <Tgt Cat="Text" Stat="Loc" Orig="New" AutoAppr="N/A">
REM            <Val><![CDATA[New data here]]></Val>
REM          </Tgt>
REM Or, use the backup file to do that
fc getstring.exe.lcl.backup getstring.exe.lcl
copy /y getstring.exe.lcl.backup getstring.exe.lcl
REM NOTE we can't generate EXE file because we used 211 parsing. The output file name is automatically switched to dll file even though we set .exe file name as the following:
REM      lsbuild generate /w 0 /iu /novalidate /d 1042 /o ko-KR\getstring.exe /ol temp\GetString.resources.dll.lcx /t getstring.exe.lcl getstring.exe
lsbuild generate /w 0 /iu /novalidate /d 1042 /o ko-KR\GetString.resources.dll /ol temp\GetString.resources.dll.lcx /t getstring.exe.lcl getstring.exe
getstring.exe

REM ##############################
REM We can't generate satellite assembly with only the post compilatoin. So, you won't see the translated text when you run getstring.exe.
REM ##############################
del ko-KR\GetString.resources.dll 
lsbuild generate /w 0 /iu /novalidate /d 1042 /o ko-KR\GetString.resources.dll /ol temp\GetString.resources.dll.lcx /t exported.resx.lcl GetString.exe
dir ko-KR\GetString.resources.dll 
getstring.exe