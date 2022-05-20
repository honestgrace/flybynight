/****** Script for SelectTopNRows command from SSMS  ******/
SELECT TOP (1000) *
  FROM [OfficeFabric_BAG].[dbo].[DataSource]
  join Project p on p.ProjectID=dbo.DataSource.ProjectID
  where Subtenant = 'PowerAutomate' and p.Name = 'PAuFlowPeopleBot'

SELECT count(*) from DataSource where FileName like '%Robin.ExtractFromWeb.Design%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.MacroRecorder.Application%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.MacroRecorder.Driver%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.MacroRecorder.UI%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.ActiveDirectory.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Ancora.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.AWS.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Azure.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Capturefast.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Clipboard.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Cmd.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Cognitive.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.CompressionOpenSource.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Console.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Cryptography.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.CyberArk.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Database.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.DateTime.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Display.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Email.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Excel.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Exchange.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.File.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Folder.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.FTP.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.MouseAndKeyboard.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.OCR.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.OCR.Utilities%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Outlook.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.PDF.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Services.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.System.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.TerminalEmulation.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Text.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.UIAutomation.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Variables.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.Web.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.WebAutomation.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.Modules.XML.Actions%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%Robin.UIAutomation.UI.Common%' and ProjectID = 1092
SELECT count(*) from DataSource where FileName like '%WinAutomation.Localization%' and ProjectID = 1092