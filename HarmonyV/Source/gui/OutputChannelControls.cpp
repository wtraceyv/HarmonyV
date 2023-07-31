#include "OutputChannelControls.h"

namespace gui
{
	// Gross helper to generate sliders on the fly for right now
	std::shared_ptr<VolPanPair> genChannelControl()
	{
		return std::make_shared<VolPanPair>();
	}

	OutputChannelControls::OutputChannelControls()
	{
		// settings up flex container for sliders
		container.flexDirection = juce::FlexBox::Direction::row;
		container.flexWrap = juce::FlexBox::Wrap::wrap;
		container.alignContent = juce::FlexBox::AlignContent::spaceBetween;
		container.alignItems = juce::FlexBox::AlignItems::center;
		container.justifyContent = juce::FlexBox::JustifyContent::center;

		// dynamically adding a bunch of sliders
		for (int i = 0; i < 4; i++)
		{
			auto newChannel = genChannelControl();
			this->singleChannelControls.push_back(newChannel);
			addAndMakeVisible(*newChannel); // my understanding is the FlexBox resizes this later if I add it to the .items Array
		}

		auto bounds = getLocalBounds();
		setSize(bounds.getWidth(), bounds.getHeight());
	}

	// You have to put the adding of the components to the flexbox in resized(),
	// that is how it does its flex stuff on the fly.
	void OutputChannelControls::resized()
	{
		for (auto channelRef : singleChannelControls)
		{
			container.items.add(juce::FlexItem(200, 300, *channelRef));
		}

		auto bounds = getLocalBounds();	
		container.performLayout(getLocalBounds());
	}
}