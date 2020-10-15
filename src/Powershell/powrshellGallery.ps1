"System.Web.Handlers.ScriptModule, System.Web.Extensions, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35"
System.Web.Extensions, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35

New-WebManagedModule -name "test" -Type "System.Web.Handlers.ScriptModule, System.Web.Extensions, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" -Precondition "managedHandler,runtimeVersionv4.0"


if ($null -eq (get-module Gac2)) {
    find-module Gac | Install-Module
}
Import-Module Gac
$moduleType = (Get-GacAssembly -Name System.Web.Handlers.ScriptModule, System.Web.Extensions -Version 4.0.0.0).FullyQualifiedName
New-WebManagedModule -name "test" -Type $moduleType -Precondition "managedHandler,runtimeVersionv4.0"


Publish-Module -Name "iisadministration" -NuGetApiKey "0476ad39-8e9d-44a4-8351-444f616d31a3" -LicenseUri "hhttps://github.com/LTruijens/powershell-gac/blob/master/LICENSE.md" -Tag "IIS" -ReleaseNote "IISAdministration module"