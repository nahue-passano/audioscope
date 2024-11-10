/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioScopeAudioProcessorEditor::AudioScopeAudioProcessorEditor(AudioScopeAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), Timer()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1280, 720);

    // scope rectangle
    rectScope = Rectangle(0+3, 3, 1280-6, 500-3);

    // int x = rectScope.getWidth() + 4;
    // int width = getWidth() - rectScope.getRight() - 2;

    // setup L/R channel toggle buttons
    // auto* b = rbChannel.add(new juce::TextButton("L"));
    // addAndMakeVisible(b);
    // b->setRadioGroupId(1);
    // b->setClickingTogglesState(true);
    // b->setConnectedEdges(juce::Button::ConnectedOnRight);
    // rectChannel.add(new Rectangle(x, 20, width / 2, 20));

    // b = rbChannel.add(new juce::TextButton("R"));
    // addAndMakeVisible(b);
    // b->setRadioGroupId(1);
    // b->setClickingTogglesState(true);
    // b->setConnectedEdges(juce::Button::ConnectedOnLeft);
    // rectChannel.add(new Rectangle(rectChannel[0]->translated(rectChannel[0]->getWidth() - 1, 0)));

    // rbChannel[1]->setToggleState(true, juce::NotificationType::dontSendNotification);

    // Vertical Scale
    rectVerticalScale = Rectangle(100, 620, 100, 100);
    addAndMakeVisible(verticalScale);
    verticalScale.setRange(0, 100, 10);
    verticalScale.setValue(10);
    verticalScale.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    verticalScale.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectVerticalScale.getWidth(), 20);
    verticalScale.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    verticalScale.setBounds(rectVerticalScale.toNearestInt());

    // Vertical Position
    rectVerticalPosition = Rectangle(200, 620, 100, 100);
    addAndMakeVisible(verticalPosition);
    verticalPosition.setRange(-200, 200, 10);
    verticalPosition.setValue(0);
    verticalPosition.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    verticalPosition.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectVerticalPosition.getWidth(), 20);
    verticalPosition.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    verticalPosition.setBounds(rectVerticalPosition.toNearestInt());

    startTimer(1000 / REFRESH_RATE);
}

AudioScopeAudioProcessorEditor::~AudioScopeAudioProcessorEditor()
{
}

//==============================================================================
void AudioScopeAudioProcessorEditor::paint(juce::Graphics& g)
{
    int channel = 0;
    float v_mid = rectScope.getHeight() / 2;
    float x0 = rectScope.getX(), y0 = v_mid, y1 = rectScope.getY();

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colour (88,121,188));
    // g.drawLine(rectScope.getX(), rectScope.getCentreY(), rectScope.getRight(), rectScope.getCentreY(), 3);
    
    // Dashed horizontal line for VerticalPosition
    juce::Line<float> dashedLine(rectScope.getX(), v_mid - verticalPosition.getValue(), rectScope.getRight(), v_mid - verticalPosition.getValue()); // From (10, 50) to (200, 50)
    float dashLengths[] = { 5.0f, 5.0f }; // 5px dash, 3px space
    g.drawDashedLine(dashedLine, dashLengths, 2, 1.0f);

    g.setColour(juce::Colours::white);
    // std::cout << "v_mid: " << v_mid << std::endl;
    // std::cout << "rectScope.getHeight(): " << rectScope.getHeight() << std::endl;

    for (int x = rectScope.getX(); x < rectScope.getRight(); x++)
    {
        y1 = juce::jlimit<float>(
            0.0f,
            rectScope.getHeight(),
            v_mid + (audioProcessor.vBuffer.getSample(channel, vBufferIndex) * verticalScale.getValue() * 100.0f) - verticalPosition.getValue()
        );
        g.drawLine(x0, y0, x, y1, 1.0f);
        x0 = x;
        y0 = y1;
        vBufferIndex = (vBufferIndex + vBufferIncrement) % audioProcessor.vBuffer.getNumSamples();
    }
    g.setColour(juce::Colour (88,121,188));
    g.drawRoundedRectangle(rectScope, 10, 3);

    g.setFont(20);
    g.drawText("Scale", 100, 600, 100, 20, juce::Justification::centred, false);
    g.drawText("Position", 200, 600, 100, 20, juce::Justification::centred, false);
}

void AudioScopeAudioProcessorEditor::resized()
{

}

void AudioScopeAudioProcessorEditor::timerCallback()
{
    repaint();
}

void sliderValueChanged(juce::Slider* slider)
{

}