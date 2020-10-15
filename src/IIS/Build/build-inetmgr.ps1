function backupfile_x64($fileName)
{
    $isInGAC = $false
    $result = Get-ChildItem ("$env:windir\system32\inetsrv\$fileName") -Recurse 2> out-null
    if ($result -eq $null)
    {
        $result = Get-ChildItem ("$env:windir\assembly\$fileName") -Recurse 2> out-null
        if ($result -ne $null)
        {
            $isInGAC = $true
        }
    }
    if ($result -eq $null)
    {
        throw ("Can't find $filename")
    }
    
    $backupDirectory = "$env:systemdrive\private\org\x64"
    if (-not (Test-Path $backupDirectory))
    {
        md $backupDirectory | out-null
    }
    if (Test-Path ("$backupDirectory\$fileName"))
    {
        ("$backupDirectory\$fileName already exists")
    }
    else
    {
        ("$backupDirectory\$fileName copied")
        copy $result $backupDirectory | out-null
        $backupFilePath = join-path $backupDirectory $fileName
        compareFile $result $backupFilePath
    }
    $result
    $isInGAC
    return
}

function backupfile_x86($fileName)
{
    $result = Get-ChildItem ("$env:windir\syswow64\$fileName") -Recurse 2> out-null    
    if ($result -eq $null)
    {
        return
    }

    $backupDirectory = "$env:systemdrive\private\org\x86"
    if (-not (Test-Path $backupDirectory))
    {
        md $backupDirectory | out-null
    }
    if (Test-Path ("$backupDirectory\$fileName"))
    {
        ("$backupDirectory\$fileName already exists")
    }
    else
    {
        ("$backupDirectory\$fileName copied")
        copy $result $backupDirectory | out-null
    }
}

function compareFile($fromFile, $toFile)
{
    $from = get-item $fromFile
    $to = get-item $toFile
    if ($from.Length -ne $to.Length)
    {
       throw "File not matched $fromFile $toFile"
    }
}

function installPrivate($filePath)
{
    $fileName = (get-item $filePath).Name
    $result = backupfile_x64 $fileName
    $isInGAC = $result | Select-Object -Last 1
    $originalFilePath = $result | Select-Object -Last 2 | Select-Object -First 1
    if (-not (Test-Path $originalFilePath))
    {
       throw "Wrong original file path is returned"
    }
    $originalFilePath = (Get-Item $originalFilePath).FullName

    backupfile_x86 $fileName

    $privateDirectory = "$env:systemdrive\private"
    $privateFilePath = "$env:systemdrive\private\$fileName"
    $privateSymbolFilePath = $privateFilePath.Replace(".exe", "").Replace(".dll", "") + ".pdb"
    if (-not (Test-Path $privateDirectory))
    {
        md $privateDirectory
    }    
    copy $filePath $privateDirectory | out-null
    compareFile $filePath $privateFilePath

    $symbolFilePath = $filePath.Replace(".exe", "").Replace(".dll", "") + ".pdb"
    if (-not (Test-Path $symbolFilePath))
    {
        if (-not $symbolFilePath.contains("resources"))
        {
            throw "Symbol file not found $symbolFilePath"
        }
    }
    else
    {
        copy $symbolFilePath $privateDirectory | out-null
        compareFile $symbolFilePath $privateSymbolFilePath
    }
    if ($isInGAC)
    {
        $assemblyName = $fileName.Replace(".dll", "")
        gacutil.exe /uf $assemblyName
        del $originalFilePath 2> out-null
        if (test-path $originalFilePath)
        {
            throw "Failed to clean up $originalFilePath"
        }
        gacutil /if $privateFilePath
    }
    else
    {
        sfpcopy $privateFilePath $originalFilePath
    }
    compareFile $privateFilePath $originalFilePath
}

function VerifyProject ($project_file_path)
{
    $directoryPath = (get-item $project_file_path).DirectoryName
    $sourcesFilePath = Join-Path $directoryPath sources
    
    $projectContent = Get-Content $project_file_path
    $srccontent = Get-Content $sourcesFilePath

    $srcContent | foreach {
        $line = $psitem.ToString().trim()        
        if (-not $line.StartsWith("#") -and $line.Contains(".cs"))
        {
            $tokens = $line.Split("\")
            $csFile = $tokens | Where-Object { $psitem.trim().EndsWith(".cs") }
            if ($csFile -eq $null)
            {
                continue
            }
            $csFile = $csFile.Trim()
            $find = $projectContent | Where-Object { $psitem.Contains($csFile) }
            if ($find -eq $null)
            {
                ("Not Found: $csFile on $project_file_path") 
            }
        }
    }

    $projectContent | foreach {
        $line = $psitem.ToString().trim()
        if ($line.Contains(".cs"))
        {
            $tokens = $line.Split('\"><')
            $csFile = $tokens | Where-Object { $psitem.trim().EndsWith(".cs") }
            if ($csFile -eq $null)
            {
                # ignore
            }
            else
            {
                $csFile = $csFile.Trim()
                   
                $find = $srcContent | Where-Object { $psitem.Contains($csFile) }
                if ($find -eq $null)
                {
                    ("Not Found: $csFile on $sourcesFilePath") 
                }
            }
        }
    }
}

########################################################
# Inetmgr.exe
# bcz c:\src\inetsrv\iis\Managed\wm
# bcz c:\src\inetsrv\iis\admin\snapin
# bcz c:\src\inetsrv\iis\iisrearc\ftp\server\managed
########################################################
#
<# project files
VerifyProject \src\inetsrv\iis\iisrearc\ftp\server\managed\wm\Ftp\Ftp.csproj
VerifyProject \src\inetsrv\iis\iisrearc\ftp\server\managed\wm\FtpClient\FtpClient.csproj
VerifyProject \src\inetsrv\iis\iisrearc\ftp\server\managed\wm\Ftp\Ftp.csproj
VerifyProject \src\inetsrv\iis\iisrearc\ftp\server\managed\wm\FtpClient\FtpClient.csproj
VerifyProject \src\inetsrv\iis\Managed\wm\Aspnet\Aspnet.csproj
VerifyProject \src\inetsrv\iis\Managed\wm\AspnetClient\AspnetClient.csproj
VerifyProject \src\inetsrv\iis\Managed\wm\Framework\Framework.csproj
VerifyProject \src\inetsrv\iis\Managed\wm\Iis\Iis.csproj
VerifyProject \src\inetsrv\iis\Managed\wm\IisClient\IisClient.csproj
#>


net stop was /y 2> out-null
stop-process -name inetmgr -Force 2> out-null
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\aspnet\objchk\amd64\Microsoft.Web.Management.Aspnet.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\aspnet\objchk\amd64\Microsoft.Web.Management.Aspnet.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\aspnetclient\objchk\amd64\Microsoft.Web.Management.AspnetClient.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\aspnetclient\objchk\amd64\Microsoft.Web.Management.AspnetClient.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\framework\objchk\amd64\Microsoft.Web.Management.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\framework\objchk\amd64\Microsoft.Web.Management.resources.dll
#installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\frameworkext\objchk\amd64\Microsoft.Web.Management.Extensions.dll
#installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\frameworkext\objchk\amd64\Microsoft.Web.Management.Extensions.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\iis\objchk\amd64\Microsoft.Web.Management.Iis.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\iis\objchk\amd64\Microsoft.Web.Management.Iis.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\iisclient\objchk\amd64\Microsoft.Web.Management.IisClient.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\iisclient\objchk\amd64\Microsoft.Web.Management.IisClient.resources.dll
#installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\remoting\objchk\amd64\Microsoft.Web.Management.Remoting.dll
#installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\remoting\objchk\amd64\Microsoft.Web.Management.Remoting.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\native\service\objchk\amd64\WMSvc.exe
#installPrivate c:\src.obj.amd64chk\inetsrv\iis\iisrearc\ftp\server\managed\extensibility\objchk\amd64\Microsoft.Web.FtpServer.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\iisrearc\ftp\server\managed\wm\ftp\objchk\amd64\Microsoft.Web.Management.Ftp.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\iisrearc\ftp\server\managed\wm\ftp\objchk\amd64\Microsoft.Web.Management.Ftp.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\iisrearc\ftp\server\managed\wm\ftpclient\objchk\amd64\Microsoft.Web.Management.FtpClient.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\iisrearc\ftp\server\managed\wm\ftpclient\objchk\amd64\Microsoft.Web.Management.FtpClient.resources.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\admin\snapin\objchk\amd64\inetmgr.dll
installPrivate c:\src.obj.amd64chk\inetsrv\iis\managed\wm\native\shell\objchk\amd64\InetMgr.exe
net start w3svc