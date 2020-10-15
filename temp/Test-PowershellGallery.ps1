powershell.exe -command "import-module iisadministration; (get-module iisadministration).Version"
powershell.exe -command "import-module iisadministration -version 1.0.0.0; (get-module iisadministration).Version"
powershell.exe -command "import-module iisadministration -version 1.0.0.7; (get-module iisadministration).Version"


powershell.exe -command "import-module iisadministration; (get-module iisadministration).Version"
powershell.exe -command "$env:PSModulePath='C:\Windows\system32\WindowsPowerShell\v1.0\Modules';import-module iisadministration -version 1.0.0.0; (get-module iisadministration).Version"
powershell.exe -command "$env:PSModulePath='C:\Program Files\WindowsPowerShell\Modules';import-module iisadministration -version 1.0.0.7; (get-module iisadministration).Version"