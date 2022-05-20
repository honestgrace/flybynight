$workdir = "C:\tmp\work0724"
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
    "src\Robin.Core\Properties\Resources.resx" = "src\Robin.SDK\Properties\Resources.resx"
    "src\Robin.Runtime.Engine\Properties\Resources.resx" = "src\Robin.Engine\Properties\Resources.resx"
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

$path = "F:\Automation\BAGIE\power-platform-loc\PowerAutomate\PAuFlowPeopleBot"
#$path = "F:\Automation\BAGIE\D365_CE\Preview\PowerAutomate\PAuFlowPeopleBot"
#$path = "F:\Automation\BAGIE\D365_CE\Adaptation\PowerAutomate\PAuFlowPeopleBot"
$files = get-childitem $path -Recurse -File

$fromFiles = ""
$toFiles = ""
$targetfiles = $lookupTable.GetEnumerator() | ForEach-Object {
    $count = 0
    $from = $_.Key
    $to = $_.Value
    $files | ForEach {
        $item = $_
        if ($item.FullName.ToLower().Contains($from.ToLower())) {
            #Validate the case matches as well
            if (-not ($item.FullName.Contains($from))) {
                #Write-Error ("Case mismatch: " + $item.FullName)
            } 
            $count++
            $fromFiles += $item.FullName + "`n"
            $temp = $item.FullName.Replace($from, $to,  [System.StringComparison]::InvariantCultureIgnoreCase)
            if (-not ($temp.Contains($to))) {
                Write-Error ("Failed to convert: " + $item.FullName + ", " + $temp)
            }
            $toFiles += $temp + "`n"

            ('Do-Work ' + '"' + $item.FullName + '" "' + $temp + '"')
        }
    }
    Write-Host("$from : $count")
}

$tempFilePath = join-path $workdir targetFiles.txt
remove-item $tempFilePath
new-item $tempFilePath
set-content $tempFilePath $targetfiles

$tempFilePath = join-path $workdir fromFiles.txt
remove-item $tempFilePath
new-item $tempFilePath
set-content $tempFilePath $fromFiles

$tempFilePath = join-path $workdir toFiles.txt
remove-item $tempFilePath
new-item $tempFilePath
set-content $tempFilePath $toFiles