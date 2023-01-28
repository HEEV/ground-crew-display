#pragma once

#include <JuceHeader.h>

class MainComponent  : public juce::AnimatedAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void update() override;

private:

    // See https://docs.juce.com/master/classMouseListener.html for mouse events
    class MouseEvents : public MouseListener
    {
    public:
        void mouseDoubleClick(const MouseEvent& e) override;
    } _mouse;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};