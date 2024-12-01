#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioScopeAudioProcessorEditor::AudioScopeAudioProcessorEditor(AudioScopeAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), Timer()
{
    setSize(1280, 720);

    // scope rectangle
    rectScope = Rectangle(0+3, 3, 1280-6, 500-3);

    // ### Vertical axis
    // Vertical Scale
    rectVerticalScale = Rectangle(350, 600, 100, 100);
    addAndMakeVisible(verticalScale);
    verticalScale.setRange(0, 100, 10);
    verticalScale.setValue(10);
    verticalScale.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    verticalScale.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectVerticalScale.getWidth(), 20);
    verticalScale.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    verticalScale.setBounds(rectVerticalScale.toNearestInt());

    // Vertical Position
    rectVerticalPosition = Rectangle(450, 600, 100, 100);
    addAndMakeVisible(verticalPosition);
    verticalPosition.setRange(-200, 200, 10);
    verticalPosition.setValue(0);
    verticalPosition.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    verticalPosition.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectVerticalPosition.getWidth(), 20);
    verticalPosition.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    verticalPosition.setBounds(rectVerticalPosition.toNearestInt());

    // ### Horizontal axis
    // Horizontal Scale
    rectHorizontalScale = Rectangle(550, 600, 100, 100);
    addAndMakeVisible(horizontalScale);
    horizontalScale.setRange(0, 100, 10);
    horizontalScale.setValue(10);
    horizontalScale.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    horizontalScale.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectHorizontalScale.getWidth(), 20);
    horizontalScale.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    horizontalScale.setBounds(rectHorizontalScale.toNearestInt());

    // Horizontal Position
    rectHorizontalPosition = Rectangle(650, 600, 100, 100);
    addAndMakeVisible(horizontalPosition);
    horizontalPosition.setRange(-200, 200, 10);
    horizontalPosition.setValue(0);
    horizontalPosition.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    horizontalPosition.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectHorizontalPosition.getWidth(), 20);
    horizontalPosition.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    horizontalPosition.setBounds(rectHorizontalPosition.toNearestInt());

    // Mode Group
    rectModeGroup = juce::Rectangle<int>(50, 550, 100, 100);
    addAndMakeVisible(acMode);
    acMode.setButtonText("AC");
    acMode.setBounds(rectModeGroup.getX() + 20, rectModeGroup.getY() + 10, 80, 30);

    addAndMakeVisible(dcMode);
    dcMode.setButtonText("DC");
    dcMode.setBounds(rectModeGroup.getX() + 20, rectModeGroup.getY() + 40, 80, 30);

    acMode.setClickingTogglesState(true);
    dcMode.setClickingTogglesState(true);

    // Range Group
    rectRangeGroup = juce::Rectangle<int>(150, 550, 200, 100);

    addAndMakeVisible(range1Toggle);
    range1Toggle.setButtonText("10 mV - 100 mV");
    range1Toggle.setBounds(rectRangeGroup.getX() + 20, rectRangeGroup.getY() + 10, 150, 30);

    addAndMakeVisible(range2Toggle);
    range2Toggle.setButtonText("0.1 V - 1 V");
    range2Toggle.setBounds(rectRangeGroup.getX() + 20, rectRangeGroup.getY() + 40, 150, 30);

    addAndMakeVisible(range3Toggle);
    range3Toggle.setButtonText("1 V - 10 V");
    range3Toggle.setBounds(rectRangeGroup.getX() + 20, rectRangeGroup.getY() + 70, 150, 30);

    addAndMakeVisible(range4Toggle);
    range4Toggle.setButtonText("10 V - 100 V");
    range4Toggle.setBounds(rectRangeGroup.getX() + 20, rectRangeGroup.getY() + 100, 150, 30);

    range1Toggle.setClickingTogglesState(true);
    range2Toggle.setClickingTogglesState(true);
    range3Toggle.setClickingTogglesState(true);
    range4Toggle.setClickingTogglesState(true);

    startTimer(1000 / REFRESH_RATE);
}

AudioScopeAudioProcessorEditor::~AudioScopeAudioProcessorEditor() {}

//==============================================================================
void AudioScopeAudioProcessorEditor::paint(juce::Graphics& g)
{
    int channel = 0;
    float v_mid = rectScope.getHeight() / 2;
    float x0 = rectScope.getX(), y0 = v_mid, y1 = rectScope.getY();

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colour (88,121,188));
    
    
    // Dashed horizontal line for VerticalPosition
    juce::Line<float> dashedLine(rectScope.getX(), v_mid - verticalPosition.getValue(), rectScope.getRight(), v_mid - verticalPosition.getValue()); // From (10, 50) to (200, 50)
    float dashLengths[] = { 5.0f, 5.0f }; // 5px dash, 3px space
    g.drawDashedLine(dashedLine, dashLengths, 2, 1.5f);

    g.setColour(juce::Colours::white);

    for (int x = rectScope.getX(); x < rectScope.getRight(); x++)
    {
        y1 = juce::jlimit<float>(
            0.0f,
            rectScope.getHeight(),
            v_mid + (audioProcessor.vBuffer.getSample(channel, vBufferIndex) * verticalScale.getValue() * 100.0f) - verticalPosition.getValue()
        );
        g.drawLine(x0, y0, x, y1, 2.0f);
        x0 = x;
        y0 = y1;
        vBufferIndex = (vBufferIndex + vBufferIncrement) % audioProcessor.vBuffer.getNumSamples();
    }
    g.setColour(juce::Colour (88,121,188));
    g.drawRoundedRectangle(rectScope, 10, 3);

    g.setFont(22);
    // Vertical labels
    g.drawText("Vertical", 400, 545, 100, 20, juce::Justification::centred, false);
    g.drawText("Scale", 350, 580, 100, 20, juce::Justification::centred, false);
    g.drawText("Position", 450, 580, 100, 20, juce::Justification::centred, false);

    // Horizontal labels
    g.drawText("Horizontal", 600, 545, 100, 20, juce::Justification::centred, false);
    g.drawText("Scale", 550, 580, 100, 20, juce::Justification::centred, false);
    g.drawText("Position", 650, 580, 100, 20, juce::Justification::centred, false);

    // Mode label
    g.drawText("Mode", rectModeGroup.getX(), rectModeGroup.getY() - 20, rectModeGroup.getWidth(), 20, juce::Justification::centred, false);

    // Range label
    g.drawText("Range", rectRangeGroup.getX(), rectRangeGroup.getY() - 20, rectRangeGroup.getWidth(), 20, juce::Justification::centred, false);
}

void AudioScopeAudioProcessorEditor::resized() {}

void AudioScopeAudioProcessorEditor::timerCallback()
{
    repaint();
}
