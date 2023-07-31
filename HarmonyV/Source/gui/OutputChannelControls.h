#include <memory>
#include <vector>
#include <JuceHeader.h>

#include "VolPanPair.h"

namespace gui
{
	class OutputChannelControls : public juce::Component
	{
		private:
			std::vector<std::shared_ptr<VolPanPair>> singleChannelControls;
			juce::FlexBox container;
		public:
			OutputChannelControls();
			void resized() override;
	};
}