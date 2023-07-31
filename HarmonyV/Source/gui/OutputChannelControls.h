#include <memory>
#include <vector>
#include <JuceHeader.h>

namespace gui
{
	class OutputChannelControls : public juce::Component
	{
		private:
			std::vector<std::shared_ptr<juce::Slider>> sliders;
			juce::FlexBox container;
		public:
			OutputChannelControls();
			void resized() override;
	};
}