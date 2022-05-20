#
# LcxConvertForFabric.ps1
# Use LCX OM to adjust settings in lcl files for Fabric.
#

param (
    [parameter(mandatory=$true)]
    [string]$inputFile,
    [string]$LSBuildExePath
)

<#
param (
    [string]$inputFile="F:\Automation\BAGIE\power-platform-loc\HelloWorld\HelloWorld\pt-BR\TestLoc\src\HelloWorld\src\HelloWorld\Properties\Resources.resx.lcl",
    [string]$LSBuildExePath="F:\automation\BAGIE\D365_Tools\Tools\LSBuild\LSBuild.Corext.6.12.4929.5"
)
F:\Automation\BAGIE\power-platform-loc\HelloWorld\Tools\LcxConvertForFabric.ps1 -inputFile "F:\Automation\BAGIE\power-platform-loc\HelloWorld\HelloWorld\pt-BR\TestLoc\src\HelloWorld\src\HelloWorld\Properties\Resources.resx.lcl" -LSBuildExePath "F:\automation\BAGIE\D365_Tools\Tools\LSBuild\LSBuild.Corext.6.12.4929.5"
#>

$dlldir = $LSBuildExePath
$dllpath = Join-path $dlldir  "Microsoft.Localization.dll"
$null=[system.reflection.assembly]::LoadFrom($dllPath)

Function LcxConvertForFabric ($file)
{
    $reader = New-Object Microsoft.Localization.Lcx.LcxReaderWriter $file, Open, ReadWrite
    $reader.MinimumSchemaVersion = New-Object Version(1, 0);
    $doc = $reader.Load()
    if ($null -ne $doc)
    {
        $doc.BeginEdit()
        # Convert settingsPath
        if (($null -eq $doc.SettingsFile) -or (-not ($doc.SettingsFile.Name.StartsWith("@SettingsPath")))) {
            $doc.Name = (split-path -Path $($doc.Name) -Leaf)
            $doc.SettingsFile = New-Object Microsoft.Localization.LocFileRef('@SettingsPath@\default.lss',[Microsoft.Localization.LocFileRef]::Lss)
        } 

        # Convert culture
        switch ($doc.TargetCulture.Name.ToLower()) {
            "ar" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ar-SA'); break }
            "bg" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('bg-BG'); break }
            "ca" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ca-ES'); break }
            "cs" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('cs-CZ'); break }
            "da" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('da-DK'); break }
            "de" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('de-DE'); break }
            "el" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('el-GR'); break }
            "en" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('en-US'); break }
            "es" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('es-ES'); break }
            "et" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('et-EE'); break }
            "eu" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('eu-ES'); break }
            "fi" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('fi-FI'); break }
            "fr" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('fr-FR'); break }
            "gl" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('gl-ES'); break }
            "he" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('he-IL'); break }
            "hi" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('hi-IN'); break }
            "hr" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('hr-HR'); break }
            "hu" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('hu-HU'); break }
            "id" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('id-ID'); break }
            "it" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('it-IT'); break }
            "ja" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ja-JP'); break }
            "kk" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('kk-KZ'); break }
            "ko" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ko-KR'); break }
            "lt" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('lt-LT'); break }
            "lv" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('lv-LV'); break }
            "ms" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ms-MY'); break }
            "no" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('nb-NO'); break }
            "nb" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('nb-NO'); break }
            "nl" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('nl-NL'); break }
            "pl" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('pl-PL'); break }
            "pt-pt" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('pt-PT'); break }
            "ro" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ro-RO'); break }
            "ru" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('ru-RU'); break }
            "sk" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('sk-SK'); break }
            "sl" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('sl-SI'); break }
            "sr-cyrl-rs" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('sr-Cyrl-CS'); break }
            "sr-latn-rs" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('sr-Latn-CS'); break }
            "sv" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('sv-SE'); break }
            "th" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('th-TH'); break }
            "tr" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('tr-TR'); break }
            "uk" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('uk-UA'); break }
            "vi" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('vi-VN'); break }
            "zh-hans" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('zh-CN'); break }
            "zh-hant" { $doc.TargetCulture = [Microsoft.Localization.LocCulture]::GetCulture('zh-TW'); break }
        }
        $doc.EndEdit()
    } else {
        Write-Output "Cannot load Specified lcl file - $file."
    }
    $reader.Save($doc)
    $reader.Close()
    $doc.Close
}

Function main {
    $file = Get-Item $inputFile
    
    #echo $file.FullName
    LcxConvertForFabric $file.FullName
}

main