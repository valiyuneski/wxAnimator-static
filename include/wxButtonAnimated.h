#pragma once

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
                  const wxString &name = wxASCII_STR(wxStaticTextNameStr));

    void doAnimations();

private:
    void animate(double fntStart, double fntStop, double clrStart, double clrStop);
    wxSize  getBestSize(const wxString &str = wxEmptyString) const;
    
private:
    double iFontSize_{18};
    int timerDuration_ {300};
    double minFontSize_{iFontSize_ / 2};
    double maxFontSize_{iFontSize_};

    Animator animator_;
    std::vector<AnimatedValue> animatedValues_;
};
