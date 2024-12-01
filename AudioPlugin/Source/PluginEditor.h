#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using Rectangle = juce::Rectangle<float>;

//==============================================================================
/**
*/
class AudioScopeAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       public juce::Timer
{
public:
    AudioScopeAudioProcessorEditor(AudioScopeAudioProcessor&);
    ~AudioScopeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    AudioScopeAudioProcessor& audioProcessor;

    int const REFRESH_RATE{ 10 };

    juce::Slider verticalScale;
    juce::Slider verticalPosition;
    juce::Slider horizontalScale;
    juce::Slider horizontalPosition;
    juce::ToggleButton acMode;
    juce::ToggleButton dcMode;
    juce::ToggleButton range1Toggle;
    juce::ToggleButton range2Toggle;
    juce::ToggleButton range3Toggle;
    juce::ToggleButton range4Toggle;

    Rectangle rectVerticalScale;
    Rectangle rectVerticalPosition;
    Rectangle rectHorizontalScale;
    Rectangle rectHorizontalPosition;
    Rectangle rectScope;
    juce::Rectangle<int> rectModeGroup;
    juce::Rectangle<int> rectRangeGroup;

    juce::Button::Listener* toggleGroupListener{ nullptr };

    int vBufferIndex{ 0 };
    int vBufferIncrement{ 10 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioScopeAudioProcessorEditor)
};
