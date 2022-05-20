$workdir = "C:\tmp\work0718"
if (-not (Test-Path $workdir)) {
    new-item $workdir -ItemType Directory
}

function Do-CopyFile($source, $destination) {
    $parentPath = Split-Path -parent $destination
    if (-not (Test-Path $parentPath)) {
        md $parentPath
    }
    Copy-Item $source $destination    
}

function Do-MoveFile($source, $destination) {
    $parentPath = Split-Path -parent $destination
    if (-not (Test-Path $parentPath)) {
        md $parentPath
    }
    Move-Item $source $destination    
}

$lookupTable = @{
    "src\Robin.ExtractFromWeb.Design\Properties\Resources.resx" = "src\UIAutomation.ExtractFromWeb\Properties\Resources.resx"
    "src\Robin.MacroRecorder.Application\Properties\Resources.resx" = "src\UIAutomation.DesktopRecorder.Application\Properties\Resources.resx"
    "src\Robin.MacroRecorder.Driver\Properties\Resources.resx" = "src\UIAutomation.DesktopRecorder.Driver\Properties\Resources.resx"
    "src\Robin.MacroRecorder.UI\Properties\Resources.resx" = "src\UIAutomation.DesktopRecorder.UI\Properties\Resources.resx"
    "src\Robin.Modules.ActiveDirectory.Actions\Properties\Resources.resx" = "src\Modules.ActiveDirectory.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Ancora.Actions\Properties\Resources.resx" = "src\Modules.Ancora.Actions\Properties\Resources.resx"
    "src\Robin.Modules.AWS.Actions\Properties\Resources.resx" = "src\Modules.AWS.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Azure.Actions\Properties\Resources.resx" = "src\Modules.Azure.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Capturefast.Actions\Properties\Resources.resx" = "src\Modules.CaptureFast.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Clipboard.Actions\Properties\Resources.resx" = "src\Modules.Clipboard.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Cmd.Actions\Properties\Resources.resx" = "src\Modules.Cmd.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Cognitive.Actions\Properties\Resources.resx" = "src\Modules.Cognitive.Actions\Properties\Resources.resx"
    "src\Robin.Modules.CompressionOpenSource.Actions\Properties\Resources.resx" = "src\Modules.Compression.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Console.Actions\Properties\Resources.resx" = "src\Modules.Console.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Cryptography.Actions\Properties\Resources.resx" = "src\Modules.Cryptography.Actions\Properties\Resources.resx"
    "src\Robin.Modules.CyberArk.Actions\Properties\Resources.resx" = "src\Modules.Cyberark.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Database.Actions\Properties\Resources.resx" = "src\Modules.Database.Actions\Properties\Resources.resx"
    "src\Robin.Modules.DateTime.Actions\Properties\Resources.resx" = "src\Modules.DateTime.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Display.Actions\Properties\Resources.resx" = "src\Modules.Display.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Email.Actions\Properties\Resources.resx" = "src\Modules.Email.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Excel.Actions\Properties\Resources.resx" = "src\Modules.Excel.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Exchange.Actions\Properties\Resources.resx" = "src\Modules.Exchange.Actions\Properties\Resources.resx"
    "src\Robin.Modules.File.Actions\Properties\Resources.resx" = "src\Modules.File.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Folder.Actions\Properties\Resources.resx" = "src\Modules.Folder.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Ftp.Actions\Properties\Resources.resx" = "src\Modules.FTP.Actions\Properties\Resources.resx"
    "src\Robin.Modules.MouseAndKeyboard.Actions\Properties\Resources.resx" = "src\Modules.MouseAndKeyboard.Actions\Properties\Resources.resx"
    "src\Robin.Modules.OCR.Actions\Properties\Resources.resx" = "src\Modules.OCR.Actions\Properties\Resources.resx"
    "src\Robin.Modules.OCR.Utilities\Properties\Resources.resx" = "src\Modules.OCR.Utilities\Properties\Resources.resx"
    "src\Robin.Modules.Outlook.Actions\Properties\Resources.resx" = "src\Modules.Outlook.Actions\Properties\Resources.resx"
    "src\Robin.Modules.PDF.Actions\Properties\Resources.resx" = "src\Modules.Pdf.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Services.Actions\Properties\Resources.resx" = "src\Modules.Services.Actions\Properties\Resources.resx"
    "src\Robin.Modules.System.Actions\Properties\Resources.resx" = "src\Modules.System.Actions\Properties\Resources.resx"
    "src\Robin.Modules.TerminalEmulation.Actions\Properties\Resources.resx" = "src\Modules.TerminalEmulation.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Text.Actions\Properties\Resources.resx" = "src\Modules.Text.Actions\Properties\Resources.resx"
    "src\Robin.Modules.UIAutomation.Actions\Properties\Resources.resx" = "src\Modules.UIAutomation.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Variables.Actions\Properties\Resources.resx" = "src\Modules.Variables.Actions\Properties\Resources.resx"
    "src\Robin.Modules.Web.Actions\Properties\Resources.resx" = "src\Modules.Web.Actions\Properties\Resources.resx"
    "src\Robin.Modules.WebAutomation.Actions\Properties\Resources.resx" = "src\Modules.WebAutomation.Actions\Properties\Resources.resx"
    "src\Robin.Modules.XML.Actions\Properties\Resources.resx" = "src\Modules.XML.Actions\Properties\Resources.resx"
    "src\Robin.UIAutomation.UI.Common\Properties\Resources.resx" = "src\UIAutomation.Recorder.UI\Properties\Resources.resx"
    "src\WinAutomation.Localization\Properties\Resources.resx" = "src\Shared.Localization\Properties\Resources.resx"
}

function Do-MigratePAD($from, $to) {
    $parentFrom = "F:\Src\CAP\Flow-PeopleBot"
    Write-Debug "FROM: $from"
    
    Write-Host "source: $source"
    $source = join-path $parentFrom $from
    $sourceWork = Join-Path "$workdir\masterbranch" $from
    $tempSourceWork = Join-Path "$workdir\tempSourceWork" $from
    $renamedWork = Join-Path "$workdir\renamed" $to

    $destination = join-path $parentFrom $to
    $destinationWork = Join-Path "$workdir\featurebranch" $to

    <# 
    1. Copy master file to WORKDir

    cd F:\Src\CAP\Flow-PeopleBot
    git checkout master

    if (Test-Path $source) {
        Write-Host "Source: $source"
        Do-copyfile $source $sourceWork    
    }
    else {
        Write-Host "source not found: $source"
    }
    #>

    <#
    2. Copy topic branch file to WORKDir

    cd F:\Src\CAP\Flow-PeopleBot
    git checkout feature/repoReName3

    if (Test-Path $destination) {
        Write-Host "Destination: $destination"
        Do-copyfile $destination $destinationWork    
    }
    else {
        Write-Host "Destination not found: $destination"
    }
    #>

    <# 
    3. Copy source files to temporary file and move the temporary file to the new location

    if ((Test-Path $sourceWork) -and (Test-Path $destinationWork)) {
        Do-CopyFile $sourceWork $tempSourceWork
        Do-MoveFile  $tempSourceWork $renamedWork
    }
    else {
        Write-Host "source not found: $source"
    }

    #>
}

$lookupTable.GetEnumerator() | ForEach-Object {
    Do-MigratePAD $_.Key $_.Value
}

<# 
# Validate and the number of files is 41 in total
c:\tools\windiff C:\tmp\work0718\featurebranch C:\tmp\work0718\renamed
    
# Update EOL
NOTE: Referential PR: https://dev.azure.com/bagie/Production/_git/power-platform-loc/pullrequest/5919

# Validate Updated EOL
open C:\tmp\work0718\EOL.eol and save as UTF-8 with BOM
c:\tools\windiff  C:\tmp\work0718\EOL.eol F:\Automation\BAGIE\power-platform-loc\PowerAutomate\EOL.eol
#>

$original_file = "F:\Automation\BAGIE\power-platform-loc\PowerAutomate\EOL.eol"
$destination_file = Join-Path $workdir "EOL.eol"
$targetValueFound = ""
Get-Content -Path $original_file | ForEach-Object {
    $line = $_
    $found = $false
    $lookupTable.GetEnumerator() | ForEach-Object {
        $targetValue = $_.Key.TrimEnd("Resources.resx").TrimEnd("\")
        $newValue = $_.Value.TrimEnd("Resources.resx").TrimEnd("\")

        if ($line.Contains($targetValue))
        {
            $line.Replace($targetValue, $newValue)
            $found = $true
            $targetValueFound += $_.Key;
            $break
        }
    }
    if (-not $found) {
        $line
    }
} | Set-Content -Path $destination_file -Encoding UTF8

$count = 0
$lookupTable.GetEnumerator() | ForEach-Object {
    if ($targetValueFound.Contains($_.Key)) {
        $count++;
    }
    else {
        ("Not found " + $_.Key)
    }
}
("Found : $count")

<#
# Update CFG file

# Validate CFG file

# Update lcg and lcl files
NOTE: because there is no change in file name, we don't need to change the content of lcg and lcl files
#>

