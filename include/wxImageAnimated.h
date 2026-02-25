#ifndef WXIMAGEANIMATED_H_INCLUDED
#define WXIMAGEANIMATED_H_INCLUDED

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
                    const wxSize& size = wxDefaultSize)
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

            [[maybe_unused]] auto shearMatrixBy = [&](wxGraphicsContext *gcPTR, double dShearX, double dShearY) {
                auto width = image_.GetWidth();
                auto height = image_.GetHeight();

                wxGraphicsMatrix A = gcPTR->CreateMatrix();
                wxGraphicsMatrix B = gcPTR->CreateMatrix(1, dShearX, dShearY, 1, 0, 0);
                wxGraphicsMatrix C = gcPTR->CreateMatrix();

                A.Translate(width/2, height/2);
                C.Translate(-width/2, -height/2);

                gcPTR->ConcatTransform(A);
                gcPTR->ConcatTransform(B);
                gcPTR->ConcatTransform(C);
            };

            [[maybe_unused]] auto scaleMatrixBy = [&](wxGraphicsContext *gcPTR, double dScaleX, double dScaleY) {
                auto width = image_.GetWidth();
                auto height = image_.GetHeight();

                wxGraphicsMatrix A = gcPTR->CreateMatrix();
                wxGraphicsMatrix B = gcPTR->CreateMatrix();

                A.Scale(dScaleX, dScaleY);

                A.Translate(width/2, height/2);
                B.Translate(-width/2, -height/2);

                gcPTR->ConcatTransform(A);
                gcPTR->ConcatTransform(B);
            };

            [[maybe_unused]] auto setProjectionMatrix = [&](const float &angleOfView, const float &near, const float &far, wxGraphicsMatrix &M) {
                // set the basic projection matrix
                float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
                //M[0][0] = scale;  // scale the x coordinates of the projected point
                //M[1][1] = scale;  // scale the y coordinates of the projected point
                //M[2][2] = -far / (far - near);  // used to remap z to [0,1]
                //M[3][2] = -far * near / (far - near);  // used to remap z [0,1]
                //M[2][3] = -1;  // set w = -z
                //M[3][3] = 0;

                M.Set(scale, scale, -far / (far - near), -far * near / (far - near), -1.0, 0.0);
            };

            [[maybe_unused]] auto projectMatrixBy = [&](wxGraphicsContext *gcPTR) {
                auto width = image_.GetWidth();
                auto height = image_.GetHeight();

                wxGraphicsMatrix A = gcPTR->CreateMatrix();
                wxGraphicsMatrix B = gcPTR->CreateMatrix();

                float angleOfView = 90;
                float near = 0.1;
                float far = 100;
                setProjectionMatrix(angleOfView, near, far, A);

                A.Translate(width/2, height/2);
                B.Translate(-width/2, -height/2);

                gcPTR->ConcatTransform(A);
                gcPTR->ConcatTransform(B);
            };

            std::unique_ptr<wxGraphicsContext> gc{wxGraphicsContext::Create(dc)};
            //shearMatrixBy(gc.get(), dShear_, dShear_);
            scaleMatrixBy(gc.get(), dScale_, dScale_);
            //projectMatrixBy(gc.get());

            gc.get()->DrawBitmap(image_, 0, 0, image_.GetWidth(), image_.GetHeight());
        });

        Bind(wxEVT_SIZE, [this](wxSizeEvent& event) {
            const wxSize sz = DoGetBestSize();

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

    void doAnimations() {
        animate(0, 90, 1, 0.5, 0.0, 1.0);
        animator_.SetOnStop([this]() {
            animate(90, 0, 0.5, 1, 1.0, 0.0);
            animator_.SetOnStop([this]() {
            });

            animator_.Start(timerDuration_);
        });

        animator_.Start(timerDuration_);
    }
private:
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
       return image_.GetSize();
    }

protected:
    wxImage image_;

    double dRotation_{0};
    double dScale_{1};
    double dShear_{0};
    int timerDuration_{300};

    Animator animator_;
    std::vector<AnimatedValue> animatedValues_;
};

#endif // WXIMAGEANIMATED_H_INCLUDED
