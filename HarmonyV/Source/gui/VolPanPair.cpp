#include "VolPanPair.h"

namespace gui
{
	VolPanPair::VolPanPair()
	{
		// settings up flex container for sliders
		container.flexDirection = juce::FlexBox::Direction::column;
		container.flexWrap = juce::FlexBox::Wrap::noWrap;
		container.alignContent = juce::FlexBox::AlignContent::spaceAround;
		container.alignItems = juce::FlexBox::AlignItems::center;
		container.justifyContent = juce::FlexBox::JustifyContent::center;

		volume.setSliderStyle(juce::Slider::RotaryVerticalDrag);
		volume.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 20);
		volumeLabel.setText("Volume", juce::dontSendNotification);
		volumeLabel.attachToComponent(&volume, true);

		pan.setSliderStyle(juce::Slider::RotaryVerticalDrag);
		pan.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 20);
		panLabel.setText("Pan", juce::dontSendNotification);
		panLabel.attachToComponent(&pan, true);

		addAndMakeVisible(volume);
		addAndMakeVisible(pan);

	}

	void VolPanPair::resized()
	{
		container.items.add(juce::FlexItem(100, 120, volume));
		container.items.add(juce::FlexItem(100, 120, pan));

		auto bounds = getLocalBounds();
		container.performLayout(bounds);
	}
}