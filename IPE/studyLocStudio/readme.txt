How to test JHKIM_Sandbox_HO_D.arg

1. Goto F:\Automation\BAGIE\power-platform-loc\PowerAutomate
2. Git checkout users/jhkim/sandbox
3. (Optional) Rebase from master if required
4. Start RunCtas and open JHKIM_Sandbox_HO_D.arg
5. Disable tasks below the Git task
6. The "Lsbuild Merge" task is supposed to be failed if the staging repo does not have the the master(.lcg) file.
   The "Stage Master" task is also supposed to be failed because the target master is created by the "LsBuild Merge" task
   As a workaround, you can run "Lsbuild parse" task and copy the master(.lcg) to the corresponding directory.
7. Run and see the files are created under F:\Automation\BAGIE\power-platform-loc\PowerAutomate
8. Roll back the change from F:\Automation\BAGIE\power-platform-loc\PowerAutomate
