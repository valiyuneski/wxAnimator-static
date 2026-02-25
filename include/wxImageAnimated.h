#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <cmath>
#include "animator.h"
#include "animatedvalue.h"
#include <memory>

class wxImageAnimated : public wxControl
{
public:
    wxImageAnimated(wxWindow *parent,
                    const wxImage& image = wxImage(),
                    wxWindowID wxID = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);

    void doAnimations();

private:
    void animate(double rotationStart, double rotationStop,
                 double scaleStart, double scaleStop,
                 double shearStart, double shearStop);
    wxSize DoGetBestSize() const;
    
private:
    wxImage image_;

    double dRotation_{0};
    double dScale_{1};
    double dShear_{0};
    int timerDuration_{300};
    
    Animator animator_;
    std::vector<AnimatedValue> animatedValues_;
};
