#include "../include/wxStaticTextAnimated.h"

wxStaticTextAnimated::wxStaticTextAnimated(wxWindow *parent,
                         wxWindowID id,
                         const wxString &label,
                         const wxPoint &pos,
                         const wxSize &sz,
                         long style,
                         const wxString &name)
                         : wxStaticText(parent, id, label, pos, sz, style, name) {
    SetTransparent(128);

    SetFont(wxFontInfo(iFontSize_).FaceName("Arial").Italic());

    Bind(wxEVT_SIZE, [this](wxSizeEvent& event) {
        const wxSize sz = getBestSize(GetLabelText());

        // Resize Window
        SetSize(sz);

        // Repaint
        Refresh();
    });

    Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent& event) {
        doAnimations();
    });

    animator_.Reset();
}

void wxStaticTextAnimated::doAnimations() {
    animate(maxFontSize_, minFontSize_, 100, 255);
    animator_.SetOnStop([this]() {
        animate(minFontSize_, maxFontSize_, 255, 100);
        animator_.SetOnStop([this]() {
        });

        animator_.Start(timerDuration_);
    });

    animator_.Start(timerDuration_);
}

void wxStaticTextAnimated::animate(double fntStart, double fntStop, double clrStart, double clrStop) {
    animatedValues_.clear();
    animatedValues_ = {
    {fntStart, fntStop, [this](AnimatedValue *sender, double tNorm, double value)
        {
            iFontSize_ = value;

            SetFont(wxFontInfo(iFontSize_).FaceName("Arial").Italic());

            sender->easingFunction(sender->startValue, sender->endValue, tNorm);
         },
        "Animate Font Size", AnimatedValue::EaseInOutCubic},
    {clrStart, clrStop, [this](AnimatedValue *sender, double tNorm, double value)
        {
            SetForegroundColour(wxColour(200, value, 100));

            sender->easingFunction(sender->startValue, sender->endValue, tNorm);
        },
        "Animate Color Value", AnimatedValue::EaseInOutQuad}
    };

    animator_.SetAnimatedValues(animatedValues_);
    animator_.SetslotIteration([this]() {
        Refresh();
    });
}

wxSize wxStaticTextAnimated::getBestSize(const wxString &str) const {
    wxScreenDC dc;
    dc.SetFont(wxFontInfo(iFontSize_).FaceName("Arial").Italic());
    wxCoord width, height;
    dc.GetTextExtent(str, &width, &height);
    return wxSize(width, height);
}
