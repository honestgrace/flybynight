rd "c:\tmp\foo" -Recurse -Force -Confirm:$false
rd "c:\tmp\bar" -Recurse -Force -Confirm:$false
if (Test-Path "c:\tmp\foo") {
    throw "Failed to delete foo folder"
}
if (Test-Path "c:\tmp\bar") {
    throw "Failed to delete bar folder"
}

function do-copyfile($source, $destination, $culture) {
    $parent = Split-Path $destination -Parent
    if (-not (test-path $parent)) {
        md $parent
    }
    (2..10) | ForEach-Object {
        $counter = $_
        if (Test-Path $destination) {
            $file = (Split-Path $destination -Leaf) + "." + $counter.ToString() 
            $destination = join-path $parent $file
        }
    }
    Copy-Item $source $destination
}

function do-file($file) {
    Test-Path $file
    $source = $file
    $destination = Split-Path $file -Leaf
    $culture = ""
    if ($file.StartsWith("E:\CTAS\work")) {
        $destination = Join-Path "c:\tmp\foo" $destination
        if ($destination.EndsWith(".resx")) {
            $culture = $file.Split("\")[8]
            $destination = $destination.Substring(0, $destination.Length - ".resx".Length)
            $destination += "." + $culture + ".resx"
        }
    } else {
        $destination = Join-Path "c:\tmp\bar" $destination
    }

    $languages   = "bg,ca,cs,da,de,el,es,et,eu,fi,fr,gl,hi,hr,hu,id,it,ja,kk,ko,lt,lv,ms,nb-NO,nl,pl,pt-BR,pt-PT,ro,ru,sk,sl,sr-Cyrl-RS,sr-Latn-RS,sv,th,tr,uk,vi,zh-HANS,zh-Hant"

    $languages.Split(",") | ForEach-Object {
        $culture = $_
        $lang_source = $source
        $lang_destination = $destination
        
        $lang_source = $lang_source.Replace("\ko\", "\" + $culture + "\")
        $lang_source = $lang_source.Replace(".ko.", "." + $culture + ".")

        if ($lang_destination.StartsWith("c:\tmp\foo")) {
            $lang_destination = $lang_destination.Replace("c:\tmp\foo", (join-path "c:\tmp\foo" $culture))
        }
        if ($lang_destination.StartsWith("c:\tmp\bar")) {
            $lang_destination = $lang_destination.Replace("c:\tmp\bar", (join-path "c:\tmp\bar" $culture))
        }
        $lang_destination = $lang_destination.Replace(".ko.", "." + $culture + ".")
        do-copyfile $lang_source $lang_destination
    }
}

do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\AlertHeading_daa32baa-86dc-4eef-a166-90c808e2cca8.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\ErrorHeading_12b46dc2-9c8d-4321-97b5-fc7e973b9dc9.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\FixFlowButtonText_8ce8f689-978f-4d40-8328-4ed96585a022.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\GoToMyFlowButtonText_931b9394-dbe8-458a-b6bf-9b2ca8c1a24d.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\RemediationHeading_b59cea22-bf14-4862-9812-349cbadbc268.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\FlowIssue\TroubleshootingTipsHeading_3d7b51d1-1357-40fe-9fe8-500cb94d0e4f.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\CompanyAddress_7567c904-0032-423d-9034-de12ca062a54.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_PrivacyLink_4b38b262-035b-43d3-bec1-95f1860898f1.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_PrivacyText_2eef69a6-0e6c-425a-8132-a246690bc456.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_UnsubscribeLink_72ddea0e-9a97-4056-ae9a-73cca473b1c2.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Footer_UnsubscribeText_fa5f6106-28d3-41bc-8864-d7655e4cc98a.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\Header_Text_f482f155-dcea-46fe-abd1-f3a91faa73e3.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\LearnMore_f0b2b5b4-1ab6-48dd-a997-141e0ef11c7d.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_LearnMoreLink_c10ed5eb-717e-4dfa-b94f-6cb46696f07b.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_LearnMoreText_6d77cb5d-ae9b-49e1-a9b8-d3eba4b8990d.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_Link_c10ed5eb-717e-4dfa-b94f-6cb46696f07b.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\MobileLinks_Text_aea14058-6893-4e35-934d-af5373f8f960.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\CommonComponents\Resources\ProductName_f9eb7814-29d0-4cc9-92de-55fcab241ce1.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\Closing_11e17480-6927-44a8-89f7-758d5a8fcc03.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\EmailDescription_ebdaed06-7a36-4307-a479-bfc0f5752da1.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\Greeting_053f3b4c-08c6-46f7-9267-d7c3a1ef2190.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\ListHeaderNotificationCount_bd01ef0a-dfea-41e3-b663-86f9ab7bc9a1.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\ListItemFailureCount_154341a4-af0d-459a-bc4f-c1414f958cc3.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\Signature_f4015ae0-12f5-4f25-86a3-1c20fdcd82c7.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\SupportPromptLink_310f5f90-b801-4368-8a6f-fbc4e376dc35.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\FailingFlows\Resources\SupportPromptText_13c16a32-3227-498d-a247-b54cecb55c44.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\InviteButtonFlowUsers\Resources\ButtonText_51715ace-c140-4a13-91a9-6828039697fe.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\InviteButtonFlowUsers\Resources\Greeting_821afac9-c045-4461-ae84-1d425fc97cbf.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\InviteButtonFlowUsers\Resources\Message_b5d086f1-b605-43d0-be72-92aba4457a1e.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\InviteOwners\Resources\ButtonText_9faabc76-8b0a-41e9-85e6-9b596caeec2a.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\InviteOwners\Resources\Greeting_5904b1c6-5536-4480-991f-9eee796cebb5.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\InviteOwners\Resources\Message_6706e9bd-88ad-43c8-84c8-1cd7a3e3be3d.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\RequestButtonAccess\Resources\ButtonText_60cb173d-a187-4fbf-9e49-9882433daa03.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\RequestButtonAccess\Resources\Message_e4f7e771-7217-46b3-9f12-dae838a96e19.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\AlertSubHeadingV2_880ad8f3-c14a-4922-83ea-d7bb87c6cc1e.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\AlertSubHeading_8b0c551e-e2b4-45f5-8555-6bb69950c850.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet1_2563bbac-348d-44a9-a1e9-c22c54ffee6a.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet2_fd59c007-4d0e-4a98-9c51-c4e0a0675aee.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet3_401bad18-cedb-4d52-be2c-22d346c287c6.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescriptionV2_036b679a-9c77-4088-8154-02261f5be557.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\ErrorDescription_b31ffbcc-6586-46a4-b1d5-c8770c6942d2.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\RemediationDetail_76c54fd1-7d47-45fa-8c9e-9a555f6a08d8.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\RemediationLinkActionLimits_8ec8ea4a-9ecf-45c9-a85c-8d1085c6713c.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\RemediationLinkComparePlans_e4f24451-b6f5-40ec-ad61-39da80652564.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetApplyToEach_7bb82d5d-7df5-47ff-a171-65f1cded3724.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetDoUntil_77770f2d-d077-4359-9de2-9712cd5317a8.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetFilterQuery_f949f5c6-4a03-4af6-947d-c72c24778c47.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SnippetTopCount_21c6437a-9972-4b83-a18d-8449db6a303c.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\SubjectV2_06037602-d86d-463b-b851-e7466525cdfd.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\Subject_e5f5d4fb-aec1-4815-bd59-eac2d5c182f0.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet1V2_3ab47331-f436-47d0-b587-9a44eb15309a.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet1_2fbe01b8-be67-4d68-a0ed-99e841ce01f6.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2V2Link_30862b0e-902c-45ce-938e-e369a3235367.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2V2_4f63cc77-e4a3-433f-bb8b-b201cd5148ac.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2_8620501d-12aa-40ee-a363-8993b8f489d2.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet3V2_8f5cb41a-4a16-454d-8bea-f6a011154e42.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet3_063d4027-eef2-4813-b75a-4f11083af3a8.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet4_81869eed-c9be-4b98-ba24-1ced756e1163.xml
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\ErrorResponses\ErrorResponseMessages.resx
do-file E:\CTAS\work\Runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\common\LbaWorking\default\common\lcg\Resources\CommonResources.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\AdaptiveCardsResources.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\ArmTemplateResources.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\FeedMessageTemplates.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\PushNotificationTemplates.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\SwaggerResources.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Resources\TemplateResources.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\Shadow\LogicApps\ErrorResponseMessages.resx
do-file E:\CTAS\work\runs\20210806-231344-xg1f76g2r1\work\PowerAutomate\PAuFlowRP\ko\processsimple.Roles.ProcessSimple.Data\LbaWorking\default\processsimple.Roles.ProcessSimple.Data\lcg\SystemTemplates\SystemTemplates.resx
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\AlertHeading_daa32baa-86dc-4eef-a166-90c808e2cca8.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\ErrorHeading_12b46dc2-9c8d-4321-97b5-fc7e973b9dc9.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\FixFlowButtonText_8ce8f689-978f-4d40-8328-4ed96585a022.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\GoToMyFlowButtonText_931b9394-dbe8-458a-b6bf-9b2ca8c1a24d.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\RemediationHeading_b59cea22-bf14-4862-9812-349cbadbc268.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\FlowIssue\TroubleshootingTipsHeading_3d7b51d1-1357-40fe-9fe8-500cb94d0e4f.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\CompanyAddress_7567c904-0032-423d-9034-de12ca062a54.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\Footer_PrivacyLink_4b38b262-035b-43d3-bec1-95f1860898f1.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\Footer_PrivacyText_2eef69a6-0e6c-425a-8132-a246690bc456.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\Footer_UnsubscribeLink_72ddea0e-9a97-4056-ae9a-73cca473b1c2.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\Footer_UnsubscribeText_fa5f6106-28d3-41bc-8864-d7655e4cc98a.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\Header_Text_f482f155-dcea-46fe-abd1-f3a91faa73e3.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\LearnMore_f0b2b5b4-1ab6-48dd-a997-141e0ef11c7d.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\MobileLinks_LearnMoreLink_c10ed5eb-717e-4dfa-b94f-6cb46696f07b.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\MobileLinks_LearnMoreText_6d77cb5d-ae9b-49e1-a9b8-d3eba4b8990d.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\MobileLinks_Link_c10ed5eb-717e-4dfa-b94f-6cb46696f07b.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\MobileLinks_Text_aea14058-6893-4e35-934d-af5373f8f960.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\CommonComponents\Resources\ProductName_f9eb7814-29d0-4cc9-92de-55fcab241ce1.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\Closing_11e17480-6927-44a8-89f7-758d5a8fcc03.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\EmailDescription_ebdaed06-7a36-4307-a479-bfc0f5752da1.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\Greeting_053f3b4c-08c6-46f7-9267-d7c3a1ef2190.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\ListHeaderNotificationCount_bd01ef0a-dfea-41e3-b663-86f9ab7bc9a1.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\ListItemFailureCount_154341a4-af0d-459a-bc4f-c1414f958cc3.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\Signature_f4015ae0-12f5-4f25-86a3-1c20fdcd82c7.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\SupportPromptLink_310f5f90-b801-4368-8a6f-fbc4e376dc35.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\FailingFlows\Resources\SupportPromptText_13c16a32-3227-498d-a247-b54cecb55c44.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\InviteButtonFlowUsers\Resources\ButtonText_51715ace-c140-4a13-91a9-6828039697fe.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\InviteButtonFlowUsers\Resources\Greeting_821afac9-c045-4461-ae84-1d425fc97cbf.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\InviteButtonFlowUsers\Resources\Message_b5d086f1-b605-43d0-be72-92aba4457a1e.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\InviteOwners\Resources\ButtonText_9faabc76-8b0a-41e9-85e6-9b596caeec2a.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\InviteOwners\Resources\Greeting_5904b1c6-5536-4480-991f-9eee796cebb5.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\InviteOwners\Resources\Message_6706e9bd-88ad-43c8-84c8-1cd7a3e3be3d.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\RequestButtonAccess\Resources\ButtonText_60cb173d-a187-4fbf-9e49-9882433daa03.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\RequestButtonAccess\Resources\Message_e4f7e771-7217-46b3-9f12-dae838a96e19.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\AlertSubHeadingV2_880ad8f3-c14a-4922-83ea-d7bb87c6cc1e.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\AlertSubHeading_8b0c551e-e2b4-45f5-8555-6bb69950c850.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet1_2563bbac-348d-44a9-a1e9-c22c54ffee6a.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet2_fd59c007-4d0e-4a98-9c51-c4e0a0675aee.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\ErrorDescriptionBullet3_401bad18-cedb-4d52-be2c-22d346c287c6.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\ErrorDescriptionV2_036b679a-9c77-4088-8154-02261f5be557.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\ErrorDescription_b31ffbcc-6586-46a4-b1d5-c8770c6942d2.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\RemediationDetail_76c54fd1-7d47-45fa-8c9e-9a555f6a08d8.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\RemediationLinkActionLimits_8ec8ea4a-9ecf-45c9-a85c-8d1085c6713c.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\RemediationLinkComparePlans_e4f24451-b6f5-40ec-ad61-39da80652564.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\SnippetApplyToEach_7bb82d5d-7df5-47ff-a171-65f1cded3724.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\SnippetDoUntil_77770f2d-d077-4359-9de2-9712cd5317a8.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\SnippetFilterQuery_f949f5c6-4a03-4af6-947d-c72c24778c47.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\SnippetTopCount_21c6437a-9972-4b83-a18d-8449db6a303c.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\SubjectV2_06037602-d86d-463b-b851-e7466525cdfd.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\Subject_e5f5d4fb-aec1-4815-bd59-eac2d5c182f0.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet1V2_3ab47331-f436-47d0-b587-9a44eb15309a.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet1_2fbe01b8-be67-4d68-a0ed-99e841ce01f6.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2V2Link_30862b0e-902c-45ce-938e-e369a3235367.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2V2_4f63cc77-e4a3-433f-bb8b-b201cd5148ac.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet2_8620501d-12aa-40ee-a363-8993b8f489d2.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet3V2_8f5cb41a-4a16-454d-8bea-f6a011154e42.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet3_063d4027-eef2-4813-b75a-4f11083af3a8.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\EmailLoc\ThrottlingAlert\TroubleshootingTipsBullet4_81869eed-c9be-4b98-ba24-1ced756e1163.xml
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\ErrorResponses\ErrorResponseMessages.ko.resx
do-file C:\tmp\test-flow-rp\builddrop\common\lba.wksp_lbaonly\lba\ko\common\locstudio.target\Resources\CommonResources.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\AdaptiveCardsResources.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\ArmTemplateResources.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\FeedMessageTemplates.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\PushNotificationTemplates.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\SwaggerResources.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Resources\TemplateResources.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\Shadow\LogicApps\ErrorResponseMessages.ko.resx
do-file  C:\tmp\test-flow-rp\builddrop\processsimple.Roles.ProcessSimple.Data\lba.wksp_lbaonly\lba\ko\processsimple.Roles.ProcessSimple.Data\locstudio.target\SystemTemplates\SystemTemplates.ko.resx
