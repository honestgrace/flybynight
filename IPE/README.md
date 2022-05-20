# Introduction 
TODO: Give a short introduction of your project. Let this section explain the objectives or the motivation behind this project. 
Updated from local branch

# Getting Started
TODO: Guide users through getting your code up and running on their own system. In this section you can talk about:
1.	Installation process
2.	Software dependencies
3.	Latest releases
4.	API references

# Build and Test
1. Build SimpleManagedExe
cd src\SimpleManagedExe
1.1. Build project with msbuild
    msbuild SimpleManagedEXE.csproj /verbosity:detailed

1.2. Manual build
    resgen /compile Properties\Resources.resx,ManagedExe.Properties.Resources.resources
    resgen Properties\ResourcesText.txt
    resgen Properties\ResourcesText.de_DE.txt
    csc /target:exe /out:test.exe Program.cs Properties\Resources.Designer.cs /res:ManagedExe.Properties.Resources.resources /res:Properties\ResourcesText.resources
    al -target:lib -embed:Properties\ResourcesText.de.resources -culture:de -out:test.dll
# Contribute
TODO: Explain how other users and developers can contribute to make your code better. 

If you want to learn more about creating good readme files then refer the following [guidelines](https://docs.microsoft.com/en-us/azure/devops/repos/git/create-a-readme?view=azure-devops). You can also seek inspiration from the below readme files:
- [ASP.NET Core](https://github.com/aspnet/Home)
- [Visual Studio Code](https://github.com/Microsoft/vscode)
- [Chakra Core](https://github.com/Microsoft/ChakraCore)