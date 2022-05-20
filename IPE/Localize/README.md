# HelloWorld Localization Scripts

This folder contains scripts and files used for handing off and handing back localization to the BAGIE localization team.

## How to initialization this Localize folder for a brand-new project
 - Create Localize folder
 - Copy all the files of Localize from other repo to the newly created Localize folder
 - Open makelang.cmd and update the feature name with the new feature name
 - Create LSS folder for BASE and each target languages
   Ex. 
      Extern\Base\HelloWorld\LSS
	  Extern\de-DE\HelloWorld\LSS
	  ...
 - Copy LSS files from other repo to all the newly created LSS folders

## How to generate lcl files
 MakeLang all /handoff

## How to generate resx files
 MakeLang all

## How to generate pseudo localized resx files
 MakeLang All /FullPL

## Descriptions:
 filelist.txt
 - contains relative path to localization target files

 language.txt
 - contains target language list

 Makelang.cmd
 - language files generation script

 Extern\[language]\...\LSS
 - Folder contains lss files  (Please do not touch)

 Extern\[language]\src\...\[filename].lcl
 - lcl file which contains localization per file  (Please do not touch)

## Locver example:
In resx file, you can prvide <comment>

When you want to keep entire string same as English source:
		<value>Azure VirtualMachine Info</value>
		<comment>{Locked}</comment>
Result: "Azure VirtualMachine Info" will remain as same as English.

When you want to keep part of the string  same as English source
		<value>Azure VirtualMachine Info</value>
		<comment>{Locked="Azure VirtualMachine"}</comment>
Result: "Azure VirtualMachine" will remain as same as English (and "Info" will be localized)

When you want to use Placeholder:
		<value>Failed to install required components: {0}</value>
		<comment>{Placeholder = "{0}"} Placeholder 0 = Cause of failure (Error number)</comment>
Result: You provide context info of placeholder so that translator can localize properly

Note:
Please use curly bracket "{" and "}" in comment only for locver rule.
