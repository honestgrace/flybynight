function Test-Assertion 
{ 
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
            throw $message 
        } 
        if ($null -eq $InputObject) { 
            $message = "Assertion failed (`$InputObject is `$null): $info" 
            Write-Debug -Message $message 
            throw  $message 
        } 
        if ($InputObject -isnot [System.Boolean]) { 
            $type = $InputObject.GetType().FullName 
            $value = if ($InputObject -is [System.String]) {"'$InputObject'"} else {"{$InputObject}"} 
            $message = "Assertion failed (`$InputObject is of type $type with value $value): $info" 
            Write-Debug -Message $message 
            throw $message 
        } 
        if (-not $InputObject) { 
            $message = "Assertion failed (`$InputObject is `$false): $info" 
            Write-Debug -Message $message 
            throw $message 
        } 
        Write-Verbose -Message "Assertion passed: $info" 
    } 
 
    End 
    { 
        if ($inputFromPipeline -and $inputCount -lt 1) { 
            $message = "Assertion failed (no objects piped to Test-Assertion): $info" 
            Write-Debug -Message $message 
            throw $message 
        } 
    } 
} 

function Test-CORS($failUnlistedOrigins, $orgin, $expectedStatusCode, $expectedAccessControlAllowOrigin)
{ 
    $configFilePath = "C:\inetpub\wwwroot\web.config"
    $configFile = `
    '<?xml version="1.0" encoding="UTF-8"?>
    <configuration>
        <system.webServer>
            <cors enabled="true" failUnlistedOrigins="' + $failUnlistedOrigins + '">
	           <add origin="' + $orgin + '" allowed="true" />               
	        </cors>
        </system.webServer>
    </configuration>'

    $uri = "http://foo.com"
    $respose = $null

    stop-process -name w3wp -confirm:$false -force
    Start-WebAppPool DefaultAppPool

    remove-item $configFilePath -Force -Confirm:$false
    new-item -ItemType file -Value $configFile $configFilePath -Force

    $respose = Invoke-WebRequest $uri -Headers @{"Origin"="www.foo.com"}
    Test-Assertion ($respose.StatusCode -eq $expectedStatusCode) -Verbose
    Test-Assertion ($respose.Headers.'Access-Control-Allow-Origin' -eq $expectedAccessControlAllowOrigin) -Verbose
}


Test-CORS -failUnlistedOrigins "false" -orgin "www.foo.com" -expectedStatusCode 200 -expectedAccessControlAllowOrigin "www.foo.com"