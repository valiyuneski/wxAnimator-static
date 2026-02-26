#ifndef WXDLGANIMATED_H
#define WXDLGANIMATED_H

#include <wx/sizer.h>
#include <wx/minifram.h>
#include <wx/radiobox.h>

#include "animator.h"
#include "animatedvalue.h"

class wxDlgAnimated : public wxMiniFrame
{
    public:
        wxDlgAnimated(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString& title = _T("wxProjectAxesDlg"))
            : wxMiniFrame(parent, id, title, wxDefaultPosition, wxSize(800, 300), wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxSTAY_ON_TOP) {
            SetTransparent(wxByte(500));

/*            static const wxString mode[] = { "sinus", "image", "wave", "button", "ellipse", "circular", "tachometer" };
            const auto radioPickerMode = new wxRadioBox(this, wxID_ANY, "Mode", wxDefaultPosition, wxDefaultSize, WXSIZEOF(mode), mode);

            static const wxString graph[] = { "volume", "pressure", "flow", "pressurevolume", "volumeflow" };
            const auto radioPickerGraph = new wxRadioBox(this, wxID_ANY, "Graph", wxDefaultPosition, wxDefaultSize, WXSIZEOF(graph), graph);

            auto createLayout = [&]() {
                auto mainSizer = new wxBoxSizer(wxVERTICAL);
                mainSizer->Add(radioPickerMode, wxSizerFlags().Expand().Border(wxALL, 1));
                mainSizer->Add(radioPickerGraph, wxSizerFlags().Expand().Border(wxALL, 1));

                SetSizer(mainSizer);
            };
            createLayout();*/

            doAnimations();

            animator_.Reset();
        }

    private:
        void doAnimations() {
            animate(10, 300);
            animator_.SetOnStop([this]() {
                //animate(300, 10);
                //animator_.SetOnStop([this]() {
                    // Post Resize Event To OurSelves
                    //wxCommandEvent event(wxEVT_SIZE);
                    //wxPostEvent(this, event);
                //});

                //animator_.Start(timerDuration_);

                static const wxString mode[] = { "sinus", "image", "wave", "button", "ellipse", "circular", "tachometer" };
                const auto radioPickerMode = new wxRadioBox(this, wxID_ANY, "Mode", wxDefaultPosition, wxDefaultSize, WXSIZEOF(mode), mode);

                static const wxString graph[] = { "volume", "pressure", "flow", "pressurevolume", "volumeflow" };
                const auto radioPickerGraph = new wxRadioBox(this, wxID_ANY, "Graph", wxDefaultPosition, wxDefaultSize, WXSIZEOF(graph), graph);

                auto createLayout = [&]() {
                    auto mainSizer = new wxBoxSizer(wxVERTICAL);
                    mainSizer->Add(radioPickerMode, wxSizerFlags().Expand().Border(wxALL, 1));
                    mainSizer->Add(radioPickerGraph, wxSizerFlags().Expand().Border(wxALL, 1));

                    SetSizer(mainSizer);

                    // Post Resize Event To OurSelves
                    wxCommandEvent event(wxEVT_SIZE);
                    wxPostEvent(this, event);
                };
                createLayout();
            });

            animator_.Start(timerDuration_);
        }

        void animate(double szStart, double szStop) {
            animatedValues_.clear();
            animatedValues_ = {
            {szStart, szStop, [this](AnimatedValue *sender, double tNorm, double value)
                {
                    iSize_ = value;

                    SetSize(wxSize(800, iSize_));

                    sender->easingFunction(sender->startValue, sender->endValue, tNorm);
                 },
                "Animate Y Size", AnimatedValue::EaseInOutCubic}
            };

            animator_.SetAnimatedValues(animatedValues_);
            animator_.SetslotIteration([this]() {
                Refresh();
            });
        }

    private:
        double iSize_{10};
        int timerDuration_ {500};

        Animator animator_;
        std::vector<AnimatedValue> animatedValues_;
};

#endif // WXSDLGANIMATED_H
