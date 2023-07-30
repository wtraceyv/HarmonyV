#include <JuceHeader.h>

namespace gui
{
	/**
	 * Organize all inner graphical sections.
	*/
	class MainComponent : public juce::Component
	{
		private:
			// TODO: these could be their own custom components, with the right derived types
			// This is just to arrange them and such
			juce::TextButton header;
			juce::TextButton alwaysLog;
			juce::TextButton debugsLog;
			juce::TextButton outputChannelControls;
		public:
			MainComponent();
			void paint (juce::Graphics&) override;
			void resized() override;
	};
}