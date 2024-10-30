#include "CustomAudioVisualiserComponent.h"

// Constructor
CustomAudioVisualiserComponent::CustomAudioVisualiserComponent(int numChannels)
    : juce::AudioVisualiserComponent(numChannels)
{
    // Additional initialization if needed
}

// Destructor definition
CustomAudioVisualiserComponent::~CustomAudioVisualiserComponent() 
{
    // Cleanup code if needed
}

void CustomAudioVisualiserComponent::paintChannel(juce::Graphics& g, 
                                                  juce::Rectangle<float> bounds, 
                                                  const juce::Range<float>* levels, 
                                                  int numLevels, 
                                                  int nextSample)
{
    juce::Path path;
    getChannelAsPath(path, levels, numLevels, nextSample);

    // Draw an outlined waveform instead of a filled waveform
    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType(1.0f), juce::AffineTransform::fromTargetPoints (
        0.0f, 1.0f, bounds.getX(), bounds.getY(),
        0.0f, -1.0f, bounds.getX(), bounds.getBottom(),
        (float) numLevels, 1.0f,  bounds.getRight(), bounds.getY()
        )
    );
    // g.setColour(juce::Colours::white);
    // g.strokePath(path, juce::PathStrokeType(4.0f), juce::AffineTransform()
    // );
}

 
