#ifndef WXCHECKBOXANIMATED_H_INCLUDED
#include <wx/wx.h>
#include <wx/graphics.h>
#include <cmath>

#include "animator.h"
#include "animatedvalue.h"

#include <memory>

class wxCheckBoxAnimated : public wxControl
{
public:
    wxCheckBoxAnimated(wxWindow *parent,
                       const wxImage& chkImage = wxImage(),
                       const wxImage& unchkImage = wxImage(),
                       wxWindowID wxID = wxID_ANY,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize)
                    : wxControl(parent, wxID, pos, size) {
        SetWindowStyle(wxBORDER_NONE);

        unchkImage_ = unchkImage;
        chkImage_ = chkImage;

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
                matrix.Translate(chkImage_.GetWidth()/2.0, chkImage_.GetHeight()/2.0);
                matrix.Rotate(dRotation * M_PI/180);
                matrix.Translate(-chkImage_.GetWidth()/2.0, -chkImage_.GetHeight()/2.0);
                dc.SetTransformMatrix(matrix);
            };
            //rotateMatrixByDegree(dc, dRotation_);

            [[maybe_unused]] auto shearMatrixBy = [&](wxGraphicsContext *gcPTR, double dShearX, double dShearY) {
                auto width = chkImage_.GetWidth();
                auto height = chkImage_.GetHeight();

                wxGraphicsMatrix A = gcPTR->CreateMatrix();
                wxGraphicsMatrix B = gcPTR->CreateMatrix(1, dShearX, dShearY, 1, 0, 0);
                wxGraphicsMatrix C = gcPTR->CreateMatrix();

                A.Translate(width/2.0, height/2.0);
                C.Translate(-width/2.0, -height/2.0);

                gcPTR->ConcatTransform(A);
                gcPTR->ConcatTransform(B);
                gcPTR->ConcatTransform(C);
            };

            [[maybe_unused]] auto scaleMatrixBy = [&](wxGraphicsContext *gcPTR, double dScaleX, double dScaleY) {
                auto width = chkImage_.GetWidth();
                auto height = chkImage_.GetHeight();

                wxGraphicsMatrix A = gcPTR->CreateMatrix();
                wxGraphicsMatrix B = gcPTR->CreateMatrix();

                A.Scale(dScaleX, dScaleY);

                A.Translate(width/2.0, height/2.0);
                B.Translate(-width/2.0, -height/2.0);

                gcPTR->ConcatTransform(A);
                gcPTR->ConcatTransform(B);
            };

            std::unique_ptr<wxGraphicsContext> gc{wxGraphicsContext::Create(dc)};
            //shearMatrixBy(gc.get(), dShear_, 0);
            scaleMatrixBy(gc.get(), dScale_, dScale_);
            gc.get()->DrawBitmap(bPressed_ ? unchkImage_ : chkImage_,
                                 0,
                                 0,
                                 chkImage_.GetWidth(),
                                 chkImage_.GetHeight());
        });

        Bind(wxEVT_SIZE, [this](wxSizeEvent& event) {
            const wxSize sz = DoGetBestSize();

            // Resize Window
            SetSize(sz);

            // Repaint
            Refresh();
        });

        Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent& event) {
             doForwardAnimation();
        });

        Bind(wxEVT_LEFT_UP, [this](wxMouseEvent& event) {
             doBackwardAnimation();
        });

        animator_.Reset();
    }

private:
    void doForwardAnimation() {
        animator_.Reset();
        animate(0, 90, 1, 0.3, -0.1, 0.9);
        animator_.SetOnStop([this]() {
        });

        bPressed_ = !bPressed_;
        animator_.Start(timerDuration_);
    }

    void doBackwardAnimation() {
        animator_.Reset();
        animate(90, 0, 0.3, 1, 0.9, -0.1);
        animator_.SetOnStop([this]() {
        });

        animator_.Start(timerDuration_);
    }

    void animate(double rotationStart, double rotationStop,
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

    wxSize DoGetBestSize() const {
       return chkImage_.GetSize();
    }

protected:
    wxImage chkImage_;
    wxImage unchkImage_;
    bool bPressed_ {false};

    double dRotation_{0};
    double dScale_{1};
    double dShear_{0};
    int timerDuration_{150};

    Animator animator_;
    std::vector<AnimatedValue> animatedValues_;
};

#endif // WXCHECKBOXANIMATED_H_INCLUDED
