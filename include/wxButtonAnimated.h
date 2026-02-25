#ifndef WXBTNANIMATED_H_INCLUDED
#define WXBTNANIMATED_H_INCLUDED

#include <wx/button.h>

#include "animator.h"
#include "animatedvalue.h"

wxDEFINE_EVENT(wxEVT_STATIC_BTN_LEFT_DOWN, wxCommandEvent);

class wxButtonAnimated : public wxButton {
public:
    wxButtonAnimated(wxWindow *parent,
                  wxWindowID id,
                  const wxString &label,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &sz = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString &name = wxASCII_STR(wxStaticTextNameStr))
                  : wxButton(parent, id, label, pos, sz, style, validator, name) {
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

            wxPostEvent(this, wxCommandEvent(wxEVT_STATIC_BTN_LEFT_DOWN));
        });

        animator_.Reset();
    }

    void doAnimations() {
        animate(maxFontSize_, minFontSize_, 100, 255);
        animator_.SetOnStop([this]() {
            animate(minFontSize_, maxFontSize_, 255, 100);
            animator_.SetOnStop([this]() {
            });

            animator_.Start(timerDuration_);
        });

        animator_.Start(timerDuration_);
    }

private:
    void animate(double fntStart, double fntStop, double clrStart, double clrStop) {
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

    wxSize  getBestSize(const wxString &str = wxEmptyString) const {
        wxScreenDC dc;
        dc.SetFont(wxFontInfo(iFontSize_).FaceName("Arial").Italic());
        wxCoord width, height;
        dc.GetTextExtent(str, &width, &height);
        return wxSize(width, height);
    }
private:
    double iFontSize_{18};
    int timerDuration_ {300};
    double minFontSize_{iFontSize_ / 2};
    double maxFontSize_{iFontSize_};

    Animator animator_;
    std::vector<AnimatedValue> animatedValues_;
};

#endif // WXBTNANIMATED_H_INCLUDED
