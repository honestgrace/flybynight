To reproduce:
1. In IIS 8 create an application pool
	1.1 In Advanced Settings set CPU Limit Action to Throttle (or anything other than NoAction or KillW3wp)
2. In Program.cs change computerName to be the hostname of the computer running IIS
	2.1 I have been testing this with a remote machine on which I have administrator access
3. Compile & run ConsoleApplication1.exe
4. Marvel in the COMException thrown
5. Change CPU Limit Action to NoAction or KillW3wp
6. Run ConsoleApplication1.exe
7. No COMException