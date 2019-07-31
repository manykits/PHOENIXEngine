-- acceleratorpage.lua

rc_SliderVer = nil
rc_SliderHor = nil

function rc_AcceleratorPage ()
    local uiFrame = UIFrame:New()

	uiFrame:SetAnchorHor(0.0, 1.0)
    uiFrame:SetAnchorVer(0.0, 1.0)
    
    local sliderVer = UISlider:New()
    uiFrame:AttachChild(sliderVer)
    rc_SliderVer = sliderVer
    sliderVer:GetButSlider():SetStateColorDefaultWhite()
    sliderVer:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    sliderVer:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(8,30,8,30)        
    sliderVer:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/RobotControl/images/directctrl/slider0.png")
    sliderVer:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/directctrl/slider1.png")
    sliderVer.LocalTransform:SetTranslateY(-2.0)
    sliderVer:SetAnchorParamVer(80.0, 80.0)
    sliderVer:SetSliderLength(60)    
    sliderVer:SetHeight(60.0)
    sliderVer:SetWidth(500.0)
    local text = sliderVer:CreateAddText(""..PX2_LM_APP:GetValue("Ver"))
    text:GetText():SetFontColor(Float3.WHITE)
    text:SetAnchorParamVer(-50.0, -50.0)
    sliderVer:GetButSlider():SetSize(60,60)

    local sliderHor = UISlider:New()
    uiFrame:AttachChild(sliderHor)
    rc_SliderHor = sliderHor
    sliderHor:GetButSlider():SetStateColorDefaultWhite()
    sliderHor:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    sliderHor:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(8,30,8,30)   
    sliderHor:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/RobotControl/images/directctrl/slider0.png")
    sliderHor:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/directctrl/slider1.png")
    sliderHor.LocalTransform:SetTranslateY(-2.0)
    sliderHor:SetAnchorParamVer(-80.0, -80.0)
    sliderHor:SetSliderLength(60)    
    sliderHor:SetHeight(60.0)
    sliderHor:SetWidth(500.0)
    local text = sliderHor:CreateAddText(""..PX2_LM_APP:GetValue("Hor"))
    text:GetText():SetFontColor(Float3.WHITE)
    text:SetAnchorParamVer(-50.0, -50.0)
    sliderHor:GetButSlider():SetSize(60,60)

    return uiFrame
end

rc_AcceUpdateTiming = 0.0
function rc_AcceleratorUpdate(appseconds, elapsedseconds)
    rc_AcceUpdateTiming = rc_AcceUpdateTiming + elapsedseconds

    if rc_AcceUpdateTiming > 0.1 then
        if  rc_ThePageType == PageType.PT_AXIS then
            local xPos = PX2_PFSDK:GetAcceleratorX()
            local yPos = PX2_PFSDK:GetAcceleratorY()
            local zPos = PX2_PFSDK:GetAcceleratorZ()

            local percVer = 0.5 + (xPos*0.1)*0.5
            local percHor = 0.5 + (yPos*0.1)*0.5

            if percVer < 0.0 then
                percVer = 0.0
            end
            if percVer > 1.0 then
                percVer = 1.0
            end

            if percHor < 0.0 then
                percHor = 0.0
            end
            if percHor > 1.0 then
                percHor = 1.0
            end

            rc_SliderVer:SetPercent(percVer, false)
            rc_SliderHor:SetPercent(percHor, false)

            local distVer = math.abs(percVer - 0.5)
            local distHor = math.abs(percHor - 0.5)

            local simpleMoveType = Arduino.SDT_NONE
            local moveSpeed = 255

            if distVer > 0.05 then
                if percVer <= 0.5 then
                    simpleMoveType = Arduino.SDT_FORWARD
                    moveSpeed = (0.5-percVer) * 500
                else
                    simpleMoveType = Arduino.SDT_BACKWARD
                    moveSpeed = (percVer-0.5) * 500
                end
            end

            if distHor > 0.05 then
                if percHor <= 0.4 then
                    simpleMoveType = Arduino.SDT_LEFT
                    moveSpeed = (0.5-percHor) * 200
                elseif percHor >= 0.6 then
                    simpleMoveType = Arduino.SDT_RIGHT
                    moveSpeed = (percHor-0.6) * 200
                end
            end
            
            rc_Arduino:Run(simpleMoveType, moveSpeed)
        end

        rc_AcceUpdateTiming = 0
    end
end