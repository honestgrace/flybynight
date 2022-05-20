output

Source:
\\reddog\Builds\branches\git_flow_rp_master\1.0.2733.1\retail-amd64\localization

Test
NOTE: The reason why we choosed the CFG.arg is because it has the superset of languages\
Open PowerAutomate\CFG.arg and select flow_rp manifes.
Choose "SimpleHandBack_PPUX_Core" scenario. Disable all the git check-in related tasks.

Test result:
E:\CTAS\work\Runs\20210806-195854-0zs9vx3537
E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1

Compare:
                                                                        C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\AlertHeading_daa32baa-86dc-4eef-a166-90c808e2cca8.xml
E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\AlertHeading_daa32baa-86dc-4eef-a166-90c808e2cca8.xml

                                                                       C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\TemplateResources.ko.resx
E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\TemplateResources.resx


LocalizedBinary was added with copying PPUX stuff.
      <PathTemplate type="Binary" copy="yes"
                  common="Master\[group]\en-US\[file]"
                  source="[project]\[Component]\[common]"
                    work="[Unit]\[Component]\[common]"
               published="[project]\[Component]\[common]" />

      <PathTemplate type="LocalizedBinary" copy="yes"
                  common="[group]\[targetCulture]\[file]"
                  source="[project]\[Component]\[common]"
                    work="[Unit]\[Component]\[common]"
               published="[Repo]\[common]" />


f:\src\CAP\Flow-RP
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\CommonComponents\FlowIssue\AlertHeading_daa32baa-86dc-4eef-a166-90c808e2cca8.xml
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\CommonComponents\Resources\CompanyAddress_7567c904-0032-423d-9034-de12ca062a54.xml
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\FailingFlows\Resources\Closing_11e17480-6927-44a8-89f7-758d5a8fcc03.xml
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\InviteButtonFlowUsers\Resources\ButtonText_51715ace-c140-4a13-91a9-6828039697fe.xml
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\InviteOwners\Resources\ButtonText_9faabc76-8b0a-41e9-85e6-9b596caeec2a.xml
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\RequestButtonAccess\Resources\ButtonText_60cb173d-a187-4fbf-9e49-9882433daa03.xml
src\processsimple\Roles\ProcessSimple.Common\EmailLoc\ThrottlingAlert\AlertSubHeadingV2_880ad8f3-c14a-4922-83ea-d7bb87c6cc1e.xml
src\processsimple\Roles\ProcessSimple.Common\ErrorResponses\ErrorResponseMessages.resx   *** two instance
src\processsimple\Roles\ProcessSimple.Common\Resources\CommonResources.resx
src\processsimple\Roles\ProcessSimple.Data\Resources\AdaptiveCardsResources.resx
src\processsimple\Roles\ProcessSimple.Data\Shadow\LogicApps\ErrorResponseMessages.resx  *** two instance
src\processsimple\Roles\ProcessSimple.Data\SystemTemplates\SystemTemplates.resx

          <Var name="MasterPath">common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue<\Var >
          <Var name="TargetPath">loc\processsimple\Roles\ProcessSimple.Common.locbld\lba\[targetCulture]\common\lcl\EmailLoc\CommonComponents\FlowIssue<\Var >

          <Var name="BinaryPath">src\processsimple\Roles\ProcessSimple.Common\EmailLoc\CommonComponents\FlowIssue<\Var >


HO_D
Use this for flowRP hand off

    <Profile name="EvalDevRepo" tasks="Sync D365_Tools Repo,Sync PPLOC Repo,ListRepoAndMakelang,Sync Production Repo,Copy Binary,Lsbuild Parse,Lsbuild Merge,Stage Master,Flat Lcg File Name,Stop workflow when no file to hand off,Generate Build Version file for PAD,Stage BuildVerFile,Git Check In,Stop workflow when lcg file is not commited,Call GenerateCFG,NotifyRun"\>

EOL:
e:\CTAS\work\runs\20210804-201343-vj1857nm44\work\EOL.eol

E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue
                                               F:\Automation\BAGIE\power-platform-loc\PowerAutomate\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue

      <PathTemplate type="Binary" copy="yes"
                  common="Master\[group]\en-US\[file]"
                  source="[project]\[Component]\[common]"
                    work="[Unit]\[Component]\[common]"
               published="[project]\[Component]\[common]" />


c:\tools\windiff  F:\Automation\BAGIE\power-platform-loc\PowerAutomate\PAuFlowRP E:\CTAS\work\runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP

E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\AlertHeading_daa32baa-86dc-4eef-a166-90c808e2cca8.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\ErrorHeading_12b46dc2-9c8d-4321-97b5-fc7e973b9dc9.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\FixFlowButtonText_8ce8f689-978f-4d40-8328-4ed96585a022.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\GoToMyFlowButtonText_931b9394-dbe8-458a-b6bf-9b2ca8c1a24d.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\RemediationHeading_b59cea22-bf14-4862-9812-349cbadbc268.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\TroubleshootingTipsHeading_3d7b51d1-1357-40fe-9fe8-500cb94d0e4f.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\CompanyAddress_7567c904-0032-423d-9034-de12ca062a54.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_PrivacyLink_4b38b262-035b-43d3-bec1-95f1860898f1.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_PrivacyText_2eef69a6-0e6c-425a-8132-a246690bc456.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_UnsubscribeLink_72ddea0e-9a97-4056-ae9a-73cca473b1c2.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_UnsubscribeText_fa5f6106-28d3-41bc-8864-d7655e4cc98a.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Header_Text_f482f155-dcea-46fe-abd1-f3a91faa73e3.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\LearnMore_f0b2b5b4-1ab6-48dd-a997-141e0ef11c7d.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_LearnMoreLink_c10ed5eb-717e-4dfa-b94f-6cb46696f07b.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_LearnMoreText_6d77cb5d-ae9b-49e1-a9b8-d3eba4b8990d.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_Link_c10ed5eb-717e-4dfa-b94f-6cb46696f07b.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_Text_aea14058-6893-4e35-934d-af5373f8f960.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\ProductName_f9eb7814-29d0-4cc9-92de-55fcab241ce1.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\Closing_11e17480-6927-44a8-89f7-758d5a8fcc03.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\EmailDescription_ebdaed06-7a36-4307-a479-bfc0f5752da1.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\Greeting_053f3b4c-08c6-46f7-9267-d7c3a1ef2190.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\ListHeaderNotificationCount_bd01ef0a-dfea-41e3-b663-86f9ab7bc9a1.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\ListItemFailureCount_154341a4-af0d-459a-bc4f-c1414f958cc3.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\Signature_f4015ae0-12f5-4f25-86a3-1c20fdcd82c7.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\SupportPromptLink_310f5f90-b801-4368-8a6f-fbc4e376dc35.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\SupportPromptText_13c16a32-3227-498d-a247-b54cecb55c44.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\InviteButtonFlowUsers\Resources\ButtonText_51715ace-c140-4a13-91a9-6828039697fe.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\InviteButtonFlowUsers\Resources\Greeting_821afac9-c045-4461-ae84-1d425fc97cbf.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\InviteButtonFlowUsers\Resources\Message_b5d086f1-b605-43d0-be72-92aba4457a1e.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\InviteOwners\Resources\ButtonText_9faabc76-8b0a-41e9-85e6-9b596caeec2a.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\InviteOwners\Resources\Greeting_5904b1c6-5536-4480-991f-9eee796cebb5.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\InviteOwners\Resources\Message_6706e9bd-88ad-43c8-84c8-1cd7a3e3be3d.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\RequestButtonAccess\Resources\ButtonText_60cb173d-a187-4fbf-9e49-9882433daa03.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\RequestButtonAccess\Resources\Message_e4f7e771-7217-46b3-9f12-dae838a96e19.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\AlertSubHeadingV2_880ad8f3-c14a-4922-83ea-d7bb87c6cc1e.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\AlertSubHeading_8b0c551e-e2b4-45f5-8555-6bb69950c850.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet1_2563bbac-348d-44a9-a1e9-c22c54ffee6a.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet2_fd59c007-4d0e-4a98-9c51-c4e0a0675aee.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet3_401bad18-cedb-4d52-be2c-22d346c287c6.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionV2_036b679a-9c77-4088-8154-02261f5be557.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescription_b31ffbcc-6586-46a4-b1d5-c8770c6942d2.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\RemediationDetail_76c54fd1-7d47-45fa-8c9e-9a555f6a08d8.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\RemediationLinkActionLimits_8ec8ea4a-9ecf-45c9-a85c-8d1085c6713c.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\RemediationLinkComparePlans_e4f24451-b6f5-40ec-ad61-39da80652564.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetApplyToEach_7bb82d5d-7df5-47ff-a171-65f1cded3724.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetDoUntil_77770f2d-d077-4359-9de2-9712cd5317a8.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetFilterQuery_f949f5c6-4a03-4af6-947d-c72c24778c47.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetTopCount_21c6437a-9972-4b83-a18d-8449db6a303c.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SubjectV2_06037602-d86d-463b-b851-e7466525cdfd.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\Subject_e5f5d4fb-aec1-4815-bd59-eac2d5c182f0.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet1V2_3ab47331-f436-47d0-b587-9a44eb15309a.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet1_2fbe01b8-be67-4d68-a0ed-99e841ce01f6.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2V2Link_30862b0e-902c-45ce-938e-e369a3235367.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2V2_4f63cc77-e4a3-433f-bb8b-b201cd5148ac.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2_8620501d-12aa-40ee-a363-8993b8f489d2.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet3V2_8f5cb41a-4a16-454d-8bea-f6a011154e42.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet3_063d4027-eef2-4813-b75a-4f11083af3a8.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet4_81869eed-c9be-4b98-ba24-1ced756e1163.xml.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\ErrorResponses\ErrorResponseMessages.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\common\LbaWorking\default\common\lcg\Resources\CommonResources.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\AdaptiveCardsResources.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\ArmTemplateResources.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\FeedMessageTemplates.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\PushNotificationTemplates.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\SwaggerResources.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\TemplateResources.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Shadow\LogicApps\ErrorResponseMessages.resx.lcg
E:\CTAS\work\Runs\20210804-202111-y4kp0swzq2\work\PowerAutomate\Generated\PAuFlowRP\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\SystemTemplates\SystemTemplates.resx.lcg
