$lists = @'
Robin.ExtractFromWeb.Design	UIAutomation.ExtractFromWeb
Robin.MacroRecorder.Application	UIAutomation.DesktopRecorder.Application
Robin.MacroRecorder.Driver	UIAutomation.DesktopRecorder.Driver
Robin.MacroRecorder.UI	UIAutomation.DesktopRecorder.UI
Robin.Modules.ActiveDirectory.Actions	Modules.ActiveDirectory.Actions
Robin.Modules.Ancora.Actions	Modules.Ancora.Actions
Robin.Modules.AWS.Actions	Modules.AWS.Actions
Robin.Modules.Azure.Actions	Modules.Azure.Actions
Robin.Modules.Capturefast.Actions	Modules.CaptureFast.Actions
Robin.Modules.Clipboard.Actions	Modules.Clipboard.Actions
Robin.Modules.Cmd.Actions	Modules.Cmd.Actions
Robin.Modules.Cognitive.Actions	Modules.Cognitive.Actions
Robin.Modules.CompressionOpenSource.Actions	Modules.Compression.Actions
Robin.Modules.Console.Actions	Modules.Console.Actions
Robin.Modules.Cryptography.Actions	Modules.Cryptography.Actions
Robin.Modules.CyberArk.Actions	Modules.Cyberark.Actions
Robin.Modules.Database.Actions	Modules.Database.Actions
Robin.Modules.DateTime.Actions	Modules.DateTime.Actions
Robin.Modules.Display.Actions	Modules.Display.Actions
Robin.Modules.Email.Actions	Modules.Email.Actions
Robin.Modules.Excel.Actions	Modules.Excel.Actions
Robin.Modules.Exchange.Actions	Modules.Exchange.Actions
Robin.Modules.File.Actions	Modules.File.Actions
Robin.Modules.Folder.Actions	Modules.Folder.Actions
Robin.Modules.Ftp.Actions	Modules.FTP.Actions
Robin.Modules.MouseAndKeyboard.Actions	Modules.MouseAndKeyboard.Actions
Robin.Modules.OCR.Actions	Modules.OCR.Actions
Robin.Modules.OCR.Utilities	Modules.OCR.Utilities
Robin.Modules.Outlook.Actions	Modules.Outlook.Actions
Robin.Modules.PDF.Actions	Modules.Pdf.Actions
Robin.Modules.Services.Actions	Modules.Services.Actions
Robin.Modules.System.Actions	Modules.System.Actions
Robin.Modules.TerminalEmulation.Actions	Modules.TerminalEmulation.Actions
Robin.Modules.Text.Actions	Modules.Text.Actions
Robin.Modules.UIAutomation.Actions	Modules.UIAutomation.Actions
Robin.Modules.Variables.Actions	Modules.Variables.Actions
Robin.Modules.Web.Actions	Modules.Web.Actions
Robin.Modules.WebAutomation.Actions	Modules.WebAutomation.Actions
Robin.Modules.XML.Actions	Modules.XML.Actions
Robin.UIAutomation.UI.Common	UIAutomation.Recorder.UI
WinAutomation.Localization	Shared.Localization
'@.split("`r`n")


$files = $lists.split("`t")

$old=@()
$new=@()
$sqllines = @()

$sql_update = "Update DataSource Set FileName = REPLACE(filename,'{0}','{1}'), RelativePath = REPLACE(RelativePath,'{0}','{1}'), ChangeID = @ChangeID where FileName like '%{0}%' and ProjectID = 1092"

$lists | % {

    $line=$_.Split("`t")
    if ($line.Count -eq 2) {
        $sqllines += $sql_update -f $line[0],$line[1]
    }
}


$sqllines | clip.exe
