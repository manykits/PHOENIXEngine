-- homepage.lua

function rc_FrameHomePage()
    local frameAll = UIFrame:New()
    frameAll:SetAnchorHor(0.0, 1.0)
    frameAll:SetAnchorVer(0.0, 1.0)
    frameAll:SetAnchorParamHor(0.0, 0.0)
    frameAll:SetAnchorParamVer(0.0, 0.0)

    return frameAll
end