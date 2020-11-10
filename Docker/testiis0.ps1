$global:g_passed = 0
$global:g_failed = 0
$global:g_scenario = $null

function Test-Assertion { 
    [CmdletBinding()] 
    Param( 
        #The value to assert. 
        [Parameter(Mandatory=$true, ValueFromPipeline=$true, Position=0)] 
        [AllowNull()] 
        [AllowEmptyCollection()] 
        [System.Object] 
        $InputObject 
    ) 
 
    Begin 
    { 
        $info = '{0}, file {1}, line {2}' -f @( 
            $MyInvocation.Line.Trim(), 
            $MyInvocation.ScriptName, 
            $MyInvocation.ScriptLineNumber 
        ) 
        $inputCount = 0 
        $inputFromPipeline = -not $PSBoundParameters.ContainsKey('InputObject') 
    } 
 
    Process 
    { 
        $inputCount++ 
        if ($inputCount -gt 1) { 
            $message = "Assertion failed (more than one object piped to Test-Assertion): $info" 
            Write-Debug -Message $message
            $global:g_failed++
            throw $message 
        } 
        if ($null -eq $InputObject) { 
            $message = "Assertion failed (`$InputObject is `$null): $info" 
            Write-Debug -Message $message
            $global:g_failed++
            throw  $message 
        } 
        if ($InputObject -isnot [System.Boolean]) { 
            $type = $InputObject.GetType().FullName 
            $value = if ($InputObject -is [System.String]) {"'$InputObject'"} else {"{$InputObject}"} 
            $message = "Assertion failed (`$InputObject is of type $type with value $value): $info" 
            Write-Debug -Message $message
            $global:g_failed++
            throw $message 
        } 
        if (-not $InputObject) { 
            $message = "Assertion failed (`$InputObject is `$false): $info" 
            Write-Debug -Message $message 
            $global:g_failed++
            throw $message 
        } 
        Write-Verbose -Message "Assertion passed: $info" 
    } 
 
    End 
    { 
        if ($inputFromPipeline -and $inputCount -lt 1) { 
            $message = "Assertion failed (no objects piped to Test-Assertion): $info" 
            Write-Debug -Message $message 
            $global:g_failed++
            throw $message 
        } 
    } 
} 

function SendRequest($item) {

    $result = invoke-restmethod $item.url
    Write-Host ("Url: " + $item.url + ", Expected: " + $item.expected)
    Test-Assertion ($result.Contains($item.expected)) -Verbose
}

function global:Invoke-TestIIS ($item, $testDescription) {
    $testTitle = "Test ==> " + $testDescription
    Write-Host $testTitle -ForegroundColor Yellow

    $testresult = $null
    $testresult = try
    { 
        SendRequest $item
        $global:g_passed++
    } 
    catch 
    { 
        ("Unexpected exception:" + $_.Exception)
        $global:g_failed++
    }
}

function Prepare-TestEnv() {
    try 
    { 
        $iisinstalled = Get-Command Get-IisSite
        if ($iisinstalled)
        {
            ####################
            # copy content files
            ####################
            copy C:\docker\wwwroot\* c:\inetpub\wwwroot -Recurse
            Test-Assertion ($null -ne (get-item c:\inetpub\wwwroot\test.htm)) -Verbose

            # Create selfsigned certificate and export and import to the root store
            $certificate = New-SelfSignedCertificate -DnsName "localhost" -CertStoreLocation "Cert:\LocalMachine\My"
            $mypwd = ConvertTo-SecureString -String "xxx" -Force -AsPlainText
            $certificatePath = ("cert:\localmachine\my\" + $certificate.Thumbprint)
            Export-PfxCertificate -FilePath "$env:systemdrive\docker\temp.pfx" -Cert $certificatePath -Password $mypwd
            Import-PfxCertificate -FilePath "$env:systemdrive\docker\temp.pfx" -CertStoreLocation "Cert:\LocalMachine\Root" -Password $mypwd

            # enable SSL
            $iisadministrationAvailable = Get-Command Get-IisSiteBinding
            if ( -not $iisadministrationAvailable ) {
                import-module webadministration
                if (1 -eq (get-webbinding -name "Default Web Site").count) {
                    New-WebBinding -Name "Default Web Site" -IP "*" -Port 443 -Protocol https
                    get-item $certificatePath | new-item -path IIS:\SslBindings\0.0.0.0!443
                }
            }
            else {
                if (1 -eq (Get-IisSiteBinding -Name "Default Web Site").count) {
                    New-IISSiteBinding -Name "Default Web Site" -Protocol https -CertificateThumbPrint $certificate.Thumbprint -BindingInformation "*:443:" -CertStoreLocation "My"
                }
            }
        }
    } 
    catch 
    { 
        ("Unexpected exception:" + $_.Exception)
    }
}

function Basic-HttpTest() {

    $item = @{
        "url"="http://localhost/test.htm";   
        "expected"="<html><body>test.htm</body><html>";
    }
    Invoke-TestIIS $item "test.htm" 

    $item = @{
        "url"="http://localhost/test.asp";   
        "expected"="<html><body>test.asp</body><html>";
    }
    Invoke-TestIIS $item "test.asp"

    $item = @{
        "url"="http://localhost/test.aspx";   
        "expected"="<html><body>test.aspx</body><html>";
    }
    Invoke-TestIIS $item "test.aspx"
}

function Basic-HttpsTest() {

    $item = @{
        "url"="https://localhost/test.htm";   
        "expected"="<html><body>test.htm</body><html>";
    }
    Invoke-TestIIS $item "test.htm" 

    $item = @{
        "url"="https://localhost/test.asp";   
        "expected"="<html><body>test.asp</body><html>";
    }
    Invoke-TestIIS $item "test.asp"

    $item = @{
        "url"="https://localhost/test.aspx";   
        "expected"="<html><body>test.aspx</body><html>";
    }
    Invoke-TestIIS $item "test.aspx"
}

###################
# Prepare Test Environment
###################
Prepare-TestEnv

###################
# Test
###################

Basic-HttpTest
Basic-HttpsTest

(1..100) | foreach {
    Basic-HttpTest
    Basic-HttpsTest
}

###################
# Show Test Result
###################
("")
Write-Host -ForegroundColor Yellow ("Test Summary:")
Write-Host -ForegroundColor Yellow ("    Passed: " + $global:g_passed)
Write-Host -ForegroundColor Yellow ("    Failed: " + $global:g_failed)
