del /q \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\x86.debug\*
del /q \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\amd64.debug\*

copy D:\gitroot\IISIntegration\src\AspNetCore\bin\Debug\Win32\aspnetcore.dll \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\x86.debug
copy D:\gitroot\IISIntegration\src\AspNetCore\bin\Debug\Win32\aspnetcore.pdb \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\x86.debug
copy D:\gitroot\IISIntegration\src\RequestHandler\bin\Debug\Win32\aspnetcorerh.dll \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\x86.debug
copy D:\gitroot\IISIntegration\src\RequestHandler\bin\Debug\Win32\aspnetcorerh.pdb \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\x86.debug

copy D:\gitroot\IISIntegration\src\AspNetCore\bin\Debug\x64\aspnetcore.dll \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\amd64.debug
copy D:\gitroot\IISIntegration\src\AspNetCore\bin\Debug\x64\aspnetcore.pdb \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\amd64.debug
copy D:\gitroot\IISIntegration\src\RequestHandler\bin\Debug\x64\aspnetcorerh.dll \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\amd64.debug
copy D:\gitroot\IISIntegration\src\RequestHandler\bin\Debug\x64\aspnetcorerh.pdb \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\amd64.debug

kill w3wp.exe
copy /y \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\x86.debug\* c:\windows\syswow64\inetsrv
copy /y \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\amd64.debug\* c:\windows\system32\inetsrv

copy /y D:\gitroot\IISIntegration\src\RequestHandler \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\src\RequestHandler
copy /y D:\gitroot\IISIntegration\src\AspNetCore \\iisdist\privates\jhkim\IIS\Ancm\Inproc\private\src\AspNetCore