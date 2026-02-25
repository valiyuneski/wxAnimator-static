#pragma once
#include <wx/wx.h>

#include <vector>

#include <chrono>
#include <stdexcept>
#include <functional>

#include "animatedvalue.h"

class Animator : public wxEvtHandler {
public:
    Animator() {
        timer.SetOwner(this);
        Bind(wxEVT_TIMER, &Animator::OnTimer, this);
    }

    void SetAnimatedValues(const std::vector<AnimatedValue> &values) {
        animatedValues = values;
    }

    const std::vector<AnimatedValue> &GetAnimatedValues() const {
        return animatedValues;
    }

    void SetslotIteration(const std::function<void()> &slotIter) {
        this->slotIter = slotIter;
    }

    void SetOnStop(const std::function<void()> &onStop) {
        this->onStop = onStop;
    }

    void Start(double durationMs) {
        if (animatedValues.empty())
            throw std::runtime_error("No animated values");

        if (durationMs <= 0)
            throw std::runtime_error("Duration must be positive");

        animationDurationMs = durationMs;
        startTime = std::chrono::steady_clock::now();

        timer.Start(10);
    }

    void Stop() {
        timer.Stop();
        onStop();
    }

    void Reset() {
        for (auto &value : animatedValues)
            value.slotValueChanged(&value, 0.0, value.startValue);
    }

    bool IsRunning() const{
        return timer.IsRunning();
    }

private:
    wxTimer timer;

    std::vector<AnimatedValue> animatedValues;

    std::function<void()> slotIter;
    std::function<void()> onStop;

    std::chrono::steady_clock::time_point startTime;

    double animationDurationMs;

    void OnTimer(wxTimerEvent &event) {
        auto now = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        if (elapsedMs >= animationDurationMs) {
            Stop();
            return;
        }

        double tNorm = elapsedMs / animationDurationMs;

        for (auto &value : animatedValues) {
            double callbackValue = value.easingFunction(value.startValue, value.endValue, tNorm);
            value.slotValueChanged(&value, tNorm, callbackValue);
        }

        slotIter();
    }
};
