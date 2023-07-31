#include <memory>
#include <vector>
#include <JuceHeader.h>

namespace gui
{
	class VolPanPair : public juce::Component
	{
		private:
			juce::Slider volume;
			juce::Label volumeLabel;
			juce::Slider pan;
			juce::Label panLabel;
			juce::FlexBox container;
		public:
			VolPanPair();
			void resized() override;
	};
}