#include "../include/wxImageAnimated.h"

wxImageAnimated::wxImageAnimated(wxWindow *parent,
                    const wxImage& image,
                    wxWindowID wxID,
                    const wxPoint& pos,
                    const wxSize& size)
                    : wxControl(parent, wxID, pos, size) {
    SetWindowStyle(wxBORDER_NONE);
    SetTransparent(wxByte(220));

    image_ = image;

    Bind(wxEVT_PAINT, [&](wxPaintEvent& event) {
        wxPaintDC dc(this);

        [[maybe_unused]] auto scaleMatrix = [&](wxPaintDC &dc, double dScale) {
            wxAffineMatrix2D matrix;
            matrix.Scale(dScale, 1);
            dc.SetTransformMatrix(matrix);
        };
        //scaleMatrix(dc, dScale_);

        [[maybe_unused]] auto rotateMatrixByDegree = [&](wxPaintDC &dc, double dRotation) {
            wxAffineMatrix2D matrix;
            matrix.Translate(image_.GetWidth()/2, image_.GetHeight()/2);
            matrix.Rotate(dRotation * M_PI/180);
            matrix.Translate(-image_.GetWidth()/2, -image_.GetHeight()/2);
            dc.SetTransformMatrix(matrix);
        };
        rotateMatrixByDegree(dc, dRotation_);

        [[maybe_unused]] auto mirrorMatrix = [&](wxPaintDC &dc) {
            wxAffineMatrix2D matrix;
            matrix.Translate(image_.GetWidth()/2, image_.GetHeight()/2);
            matrix.Mirror();
            matrix.Translate(-image_.GetWidth()/2, -image_.GetHeight()/2);
            dc.SetTransformMatrix(matrix);
        };
        //mirrorMatrix(dc);

        [[maybe_unused]] auto shearMatrix = [&](wxPaintDC &dc, double dShear) {
            // Simplified shear - just scale for now
            wxAffineMatrix2D matrix;
            matrix.Translate(image_.GetWidth()/2, image_.GetHeight()/2);
            matrix.Scale(1.0 + dShear * 0.1, 1.0);
            matrix.Translate(-image_.GetWidth()/2, -image_.GetHeight()/2);
            dc.SetTransformMatrix(matrix);
        };
        shearMatrix(dc, dShear_);

        dc.DrawBitmap(image_, 0, 0, true);
    });

    Bind(wxEVT_LEFT_DOWN, [&](wxMouseEvent& event) {
        doAnimations();
    });

    animator_.Reset();
}

void wxImageAnimated::doAnimations() {
    animate(0, 360, 1, 1.5, 0, 0.5);
    animator_.SetOnStop([this]() {
        animate(360, 0, 1.5, 1, 0.5, 0);
        animator_.SetOnStop([this]() {
        });

        animator_.Start(timerDuration_);
    });

    animator_.Start(timerDuration_);
}

void wxImageAnimated::animate(double rotationStart, double rotationStop,
                 double scaleStart, double scaleStop,
                 double shearStart, double shearStop) {
    animatedValues_.clear();
    animatedValues_ = {
    {rotationStart, rotationStop, [this](AnimatedValue *sender, double tNorm, double value)
        {
            dRotation_ = value;
            sender->easingFunction(sender->startValue, sender->endValue, tNorm);
         },
        "Animate Rotation", AnimatedValue::EaseInOutCubic},
    {scaleStart, scaleStop, [this](AnimatedValue *sender, double tNorm, double value)
        {
            dScale_ = value;
            sender->easingFunction(sender->startValue, sender->endValue, tNorm);
         },
        "Animate Scale", AnimatedValue::EaseInOutQuad},
    {shearStart, shearStop, [this](AnimatedValue *sender, double tNorm, double value)
        {
            dShear_ = value;
            sender->easingFunction(sender->startValue, sender->endValue, tNorm);
         },
        "Animate Shear", AnimatedValue::EaseInOutQuad}
    };

    animator_.SetAnimatedValues(animatedValues_);
    animator_.SetslotIteration([this]() {
        Refresh();
    });
}

wxSize wxImageAnimated::DoGetBestSize() const {
   return image_.GetSize();
}
