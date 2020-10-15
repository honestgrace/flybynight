Function Get-Something {
    [cmdletbinding()]
    Param (
        [parameter(ValueFromPipeline=$True)]
        [string[]]$Name,
        [parameter(ValueFromPipeline=$True)]
        [string[]]$SiteName
    )
    Begin {
        Write-Verbose "Initialize stuff in Begin block"
    }

    Process {
        Write-Verbose "Process block"
        Write-Host "Name: $Name"
        Write-Host "Directory: $Directory"
    }

    End {
        Write-Verbose "Final work in End block"
        $Report
    }
}

Function List-BindingInformation {
    [cmdletbinding()]
    Param (
        [parameter(ValueFromPipeline=$True)]
        [string[]]$SiteName
    )
    Begin {
        Write-Verbose "Initialize stuff in Begin block"
    }

    Process {
        Write-Verbose "Stuff in Process block to perform"
        $Computername
    }

    End {
        Write-Verbose "Final work in End block"
    }
}

gci -file C:\gitroot\tools\src\Powershell | Get-Something -Verbose



function export-sourcefile ($filePath, $postfix = "backup")
{
    ("Start " + $filePath)
    if (-not (test-path $filePath))
    {
        throw ("File not found" + $filePath)
    }

    $item = get-item $filePath
    $itemDirectoryName = $item.DirectoryName.ToLower()
    if (-not ($itemDirectoryName.Contains("c:\src")))
    {
        throw ("file path does not contain c:\src")
    }
    $exportDirectory = $itemDirectoryName.Replace("c:\src", "c:\src." + $postfix)
    if (-not (test-path $exportDirectory))
    {
        md $exportDirectory | Out-Null
    }        
    $exportfile = Join-Path $exportDirectory $item.Name
    if (test-path $exportfile)
    {
        remove-item $exportfile -Force -Confirm:$false
    }
    if (test-path $exportfile)
    {
        throw ("Can't delete file")
    }
    copy $item $exportDirectory  | Out-Null
    if (-not (test-path $exportfile))
    {
        throw ("Can't export file")
    }     
    $exportitem = get-item($exportfile) 
    if ($item.Length -ne $exportitem.Length)
    {
        throw ("Copy failed")
    }
    ("Success " + $filePath)
}


export-sourcefile c:\src\servercommon\inetsrv\iis\admin\PowerShell2\Constants.cs
export-sourcefile c:\src\servercommon\inetsrv\iis\admin\PowerShell2\microsoft.iis.powershell.commands.csproj
export-sourcefile c:\src\servercommon\inetsrv\iis\admin\PowerShell2\microsoft.iis.powershell.commands.sln
export-sourcefile c:\src\servercommon\inetsrv\iis\admin\PowerShell2\SiteBindingCommands.cs
export-sourcefile c:\src\servercommon\inetsrv\iis\admin\PowerShell2\SiteCommands.cs
export-sourcefile C:\src\servercommon\inetsrv\iis\managed\wm\commonext\User.cs
export-sourcefile C:\src\servercommon\inetsrv\iis\Managed\wm\CommonExt\CertificatesShared.cs


#Test 

get-iissite | Get-IISSiteBinding
get-iissite -name "Default Web Site" | Get-IISSiteBinding
get-iissite -name "Default Web Site" | Get-IISSiteBinding -BindingInformation "*:80:"

Remove-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:80:"
Remove-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:443:"
New-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:80:" 

Reset-IISServerManager -Confirm:$false
dir Cert:\LocalMachine\My\2055BAE809319B77E0C0E8465ABE8196CCE2E637
New-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:443:" -CertificateThumbPrint 2055BAE809319B77E0C0E8465ABE8196CCE2E637 -bindingprotocol https -Force

Reset-IISServerManager -Confirm:$false
dir Cert:\LocalMachine\My\23E91C095235ADC40630BD1449FE1C557260407C
New-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:44300:" -CertificateThumbPrint 23E91C095235ADC40630BD1449FE1C557260407C -bindingprotocol https -CertificateStoreName "My"



Microsoft.Web.Management.dll!Microsoft.Web.Management.Server.ManagementContentNavigator.GetChildren() Line 216

Add-Type -AssemblyName "Microsoft.Web.Configuration.AppHostFileProvider, Version=10.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35, processorArchitecture=MSIL"

[Reflection.Assembly]::LoadFile("C:\temp\Microsoft.Web.Configuration.AppHostFileProvider.dll")



