# You need to disable SN
# set Path=%path%;c:\Program Files (x86)\Microsoft SDKs\Windows\v8.1A\bin\NETFX 4.5.1 Tools\x64
# FYI, you can also disable SN for specific dll files with running the following commands:
# sn -Vr Microsoft.IIS.PowerShell.Commands.resources.dll
# sn -Vr Microsoft.IIS.PowerShell.Commands.dll
# \\iisdist\privates\nitashav\snreg.bat

<# 
# backup original files
md d:\private\org
copy c:\Windows\Microsoft.NET\assembly\GAC_MSIL\Microsoft.IIS.PowerShell.Commands\v4.0_10.0.0.0__31bf3856ad364e35\Microsoft.IIS.PowerShell.Commands.dll %systemdrive%\private\org
copy c:\Windows\Microsoft.NET\assembly\GAC_MSIL\Microsoft.IIS.PowerShell.Commands.Resources\v4.0_10.0.0.0_en_31bf3856ad364e35\Microsoft.IIS.PowerShell.Commands.resources.dll %systemdrive%\private\org

# setup (including clean-up test-env) NOTE: This should be done only one time with commandline window
md %systemdrive%\private
del %systemdrive%\private\Microsoft.IIS.PowerShell.Commands.*
copy /y C:\src.obj.amd64chk\servercommon\inetsrv\iis\admin\powershell2\fullclr\objchk\amd64\*.dll \\iisdist\privates\jhkim\iis\iisadministration\fullclr
copy /y C:\src.obj.amd64chk\servercommon\inetsrv\iis\admin\powershell2\fullclr\objchk\amd64\*.pdb \\iisdist\privates\jhkim\iis\iisadministration\fullclr
copy /y C:\src.obj.amd64chk\servercommon\inetsrv\iis\admin\powershell2\coreclr\objchk\amd64\*.dll \\iisdist\privates\jhkim\iis\iisadministration\coreclr
copy /y C:\src.obj.amd64chk\servercommon\inetsrv\iis\admin\powershell2\coreclr\objchk\amd64\*.pdb \\iisdist\privates\jhkim\iis\iisadministration\coreclr
copy /y \\iisdist\privates\jhkim\iis\iisadministration\fullclr\* %systemdrive%\private

net stop was /y
gacutil /u Microsoft.IIS.PowerShell.Commands
gacutil /u Microsoft.IIS.PowerShell.Commands.resources
del c:\Windows\Microsoft.NET\assembly\GAC_MSIL\Microsoft.IIS.PowerShell.Commands\v4.0_10.0.0.0__31bf3856ad364e35\Microsoft.IIS.PowerShell.Commands.dll
del c:\Windows\Microsoft.NET\assembly\GAC_MSIL\Microsoft.IIS.PowerShell.Commands.Resources\v4.0_10.0.0.0_en_31bf3856ad364e35\Microsoft.IIS.PowerShell.Commands.resources.dll

gacutil /if "$env:systemdrive\private\Microsoft.IIS.PowerShell.Commands.resources.dll"
gacutil /if "$env:systemdrive\private\Microsoft.IIS.PowerShell.Commands.dll"
net start w3svc
#>

# Test this should be done on powershell window (running as administrator)
# clean up
dir Cert:\LocalMachine\my | Where-Object {$_.Subject -eq "CN=foo.com" } | remove-item
dir Cert:\LocalMachine\root | Where-Object {$_.Subject -eq "CN=foo.com" } | remove-item

# create a new certificate
$storeLocation = "Cert:\LocalMachine\My"
$certificate = New-SelfSignedCertificate -DnsName foo.com -CertStoreLocation $storeLocation
$certificatePath = ("cert:\localmachine\my\" + $certificate.Thumbprint)

# Verify certificate with using Test-Certificate, you will get "False" value because all the certificate chain is trusted 
$testResult = ($certificate | Test-Certificate -Policy SSL -DNSName "foo.com" 2> out-null)
if (-not $testResult)
{
   Write-host "The intermediate certificate chain is not trusted, which is expected for a newly created selfsigned certificate"
} else {
   Write-Error "Expected: Not trusted"
}

# Make the certificate trusted with exporting it to "Root" cert store
if (test-path $certificatePath) {
    Write-Host "Certificate creation succeed" -Foreground Green
    $mypwd = ConvertTo-SecureString -String "xxx" -Force -AsPlainText
    Export-PfxCertificate -FilePath "$env:systemdrive\private\temp.pfx" -Cert $certificatePath -Password $mypwd
    Import-PfxCertificate -FilePath "$env:systemdrive\private\temp.pfx" -CertStoreLocation "Cert:\LocalMachine\Root" -Password $mypwd
} else {
    Write-Error "Expected: Certificate is created"
}

$certificatePath = ("cert:\localmachine\root\" + $certificate.Thumbprint)
if (test-path $certificatePath) {
    Write-Host "Exporting succeed" -Foreground Green
} else {
    Write-Error "Expected: Certificate is exported"
}

# Verify certificate with using Test-Certificate again and you should get "True" here
$testResult = ($certificate | Test-Certificate -Policy SSL -DNSName "foo.com")
if ($testResult) {
    Write-host "Certificate is verified" -Foreground Green
} else {
    Write-Error "Expected: Test-Certificate is passed"
}

$isModuleAvailable = get-module iisadministration
if ($isModuleAvailable -eq $null) {
    Write-host "iisadministration module is available, trying to load" -Foreground Green
    import-module iisadministration
    #import-module C:\src\servercommon\inetsrv\iis\admin\powershell2\bin\Debug\Microsoft.IIS.Powershell.Commands.dll
} 

$isModuleAvailable = get-module iisadministration
if ($isModuleAvailable -ne $null) {
    Write-host "iisadministration module is loaded" -Foreground Green
} else {
    Write-Error "Expected: iisadministration is loaded"
}

# Enable CCS
$temp = Get-IISCentralCertProvider
if ($temp[0].Contains("Enabled") -and $temp[0].Contains("False")) {
    $PrivateKeyPassword = "xxx"
    $user = "foo"
    $passwordSecure = convertto-securestring iis6!dfu -asplaintext -force
    $PrivateKeyPasswordSecure = convertto-securestring $PrivateKeyPassword -asplaintext -force 
    $temp = Get-LocalUser -Name $user
    if ($temp -eq $null) {
        New-LocalUser -Name $user -Password $passwordSecure
    }
    Enable-IISCentralCertProvider -CertStoreLocation c:\private -UserName $user -Password $passwordSecure -PrivateKeyPassword $PrivateKeyPasswordSecure
}
$temp = Get-IISCentralCertProvider
if ($temp[0].Contains("Enabled") -and $temp[0].Contains("True")) {
    Write-host "Centralized Certificate Store is enabled" -Foreground Green
} else {
    Write-Error "Expected: Centralized Certificate Store is enabled"
}

Remove-IISSiteBinding "Default Web Site" "*:443:" -confirm:$false
New-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:443:" -CertificateThumbPrint $certificate.Thumbprint -CertStoreLocation $storeLocation -bindingprotocol https -Force
Get-IISSiteBinding -Name "Default Web Site" -BindingInformation "*:443:"
Remove-IISSiteBinding "Default Web Site" "*:443:" -confirm:$false

$Sni = [Microsoft.Web.Administration.SslFlags]::Sni
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint $Sni $storeLocation
Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false

$Sni = [Microsoft.Web.Administration.SslFlags]::Sni
Remove-IISSiteBinding "Default Web Site" "*:443:" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:" $certificate.Thumbprint $Sni $storeLocation
$result = Get-IISSiteBinding "Default Web Site" "*:443:"
if ($result -eq $null) {
    Write-host "Binding is not created with empty hostheader" -Foreground Green
} else {
    Write-Error "Expected: Binding is not created with empty hostheader"
}

Remove-IISSiteBinding "Default Web Site" "*:443:" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:" $certificate.Thumbprint Sni $storeLocation
$result = Get-IISSiteBinding "Default Web Site" "*:443:"
if ($result -eq $null) {
    Write-host "Binding is not created with empty hostheader" -Foreground Green
} else {
    Write-Error "Expected: Binding is not created with empty hostheader"
}

Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint Sni $storeLocation
$result = Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
if ($result -ne $null) {
    Write-host "Binding is created with non-empty hostheader" -Foreground Green
} else {
    Write-Error "Expected: Binding is created with non-empty hostheader"
}

Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false

$Sni_CCS = [Microsoft.Web.Administration.SslFlags]::Sni + [Microsoft.Web.Administration.SslFlags]::CentralCertStore
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint $Sni_CCS $storeLocation -Verbose
Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false

$Sni_CCS = "Sni, CentralCertStore"
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint "Sni, CentralCertStore" $storeLocation -Verbose
Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
$Sni_CCS = (Get-IISSiteBinding "Default Web Site" "*:443:foo.com").sslFlags
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false

Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint $Sni_CCS $storeLocation -Verbose
Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false

# Disable CCS
$temp = Get-IISCentralCertProvider
if ($temp[0].Contains("Enabled") -and $temp[0].Contains("True")) {
    Disable-IISCentralCertProvider
}
$temp = Get-IISCentralCertProvider
if ($temp[0].Contains("Enabled") -and $temp[0].Contains("False")) {
    Write-host "Centralized Certificate Store is disabled" -Foreground Green
} else {
    Write-Error "Expected: Centralized Certificate Store is disabled"
}

Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint $Sni_CCS $storeLocation -Verbose
$result = Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
if ($result -eq $null) {
    Write-host "Centralized Certificate Store feature is required" -Foreground Green
} else {
    Write-Error "Expected: Binding creation should be failed because Centralized Certificate Store is disabled"
}

# Enable CCS
$temp = Get-IISCentralCertProvider
if ($temp[0].Contains("Enabled") -and $temp[0].Contains("False")) {
    $PrivateKeyPassword = "xxx"
    $user = "foo"
    $passwordSecure = convertto-securestring iis6!dfu -asplaintext -force
    $PrivateKeyPasswordSecure = convertto-securestring $PrivateKeyPassword -asplaintext -force 
    $temp = Get-LocalUser -Name $user
    if ($temp -eq $null) {
        New-LocalUser -Name $user -Password $passwordSecure
    }
    Enable-IISCentralCertProvider -CertStoreLocation c:\private -UserName $user -Password $passwordSecure -PrivateKeyPassword $PrivateKeyPasswordSecure
}
$temp = Get-IISCentralCertProvider
if ($temp[0].Contains("Enabled") -and $temp[0].Contains("True")) {
    Write-host "Centralized Certificate Store is enabled" -Foreground Green
} else {
    Write-Error "Expected: Centralized Certificate Store is enabled"
}

New-IISSiteBinding "Default Web Site" https "*:443:foo.com" $certificate.Thumbprint $Sni_CCS $storeLocation -Verbose
$result = Get-IISSiteBinding "Default Web Site" "*:443:foo.com"
if ($result -ne $null) {
    Write-host "Binding is created after enabling CCS" -Foreground Green
} else {
    Write-Error "Expected: Binding creation should be succeeded because CCS is enabled"
}
get-iissite | Get-IISSiteBinding
get-iissite -name "Default Web Site" | Get-IISSiteBinding
get-iissite -name "Default Web Site" | Get-IISSiteBinding -BindingInformation "*:443:foo.com"
Remove-IISSiteBinding "Default Web Site" "*:443:foo.com" -confirm:$false
#>

$testCount = 0
$passedTests = 0

foreach ($testcase in $testcases) 
{
    $testCount++
    $output = . $testcase["Test"]
    if ($output -eq $testcase.Expected)
    {
        $passedTests++
    }
    else
    {
        Write-Host ("Failed '{0}' yielded '{1}' instead of '{2}'" -f $testcase["Test"], $output, $testcase["Expected"]) -ForegroundColor Red
    }
}

if ($passedTests -gt 0)
{
    Write-Host "Passed $passedTests of $testCount" -ForegroundColor Green
}
else
{
    Write-Host "FAILED ALL TESTS" -ForegroundColor Red
}