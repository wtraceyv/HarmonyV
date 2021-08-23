# HarmonyV
A WIP monophonic source harmonizer in the Juce Framework

### State of the Art
I am still researching and playing with algorithms to use in this plugin. This implmentation uses frequency domain pitch shifting via block-based PSOLA and lots of FFTs with basic zero-crossing for pitch detection (there is also a commented-out frequency domain pitch detection option). I'm looking at implementing something like a Lent algorithm (MIT 1989) or "Synchronous Granular Synthesis" for pitch shifting, as either should be able to better maintain formants and possibly perform better.

### Building/Running
You will need an [install of the Juce framework and their tool the Projucer](https://juce.com/get-juce/download), which will come with most of the library code you need, but you will also need to add [Foley's Gui Magic](https://github.com/ffAudio/foleys_gui_magic) as an extra module in the Projucer so that you can use the GUI system the way I am. 

After that, settings in the projucer and/or your IDE of choice (which you set up Projucer to export to) will allow you to build various plugin types for all major platforms. Right now I develop on macOS with AUs for Logic X as my target.
