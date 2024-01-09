# JX11 Synthesizer

This is the jx11 digital synthesizer The Audio Programmer's book ["Creating Synthesizer Plug-Ins with C++ and JUCE"](https://www.theaudioprogrammer.com/synth-plugin-book) by Matthijs Hollemans.

# Setup

1. Install desired programming environment. (built with Xcode version 15.1)
2. Install and configure [JUCE](https://juce.com/). (built with version 7.0.9)
3. Create a project directory and clone this repository.
4. Click on JX11.jucer
5. Make sure to select proper exporter, then launch IDE.
6. Set target to desired plugin format, and build project.

# Architecture

## JX11/

### JX11.jucer

- Projucer Executable.

### Source/

- contains all .cpp and .h files for project.  

## img/

- contains img files for the synth schematic and lifecycle of the plugin.

### Schematic

![Synth Schematic](img/synth-schematic.jpg)

Above is the general signal flow schematic of this plugin. The image is taken directly from the book.

### Plugin lifecycle

![Plugin Lifecycle](img/plugin-lifecycle.jpg)

above is the lifecycle of most audio plugins, and is true for this plugin. The image is taken directly from the book.

## .gitignore

- The JUCE Library Code is automatically built upon creation of a Juce project making it redundant to add to this repository.

- JUCE builds it's targets into a Builds/ directory, which can end up being rather large. Again, this will be created once you build the project.

- The .DS_store file is created/updated when the MacOS terminal is opened inside of a directory. This is not needed.

- I used a VS Code workspace for documenting this project, which is also not needed.

# Useful Links

### The Audio Programer

[JX11 Source Code](https://github.com/TheAudioProgrammer/synth-plugin-book)

### JUCE

[JUCE API documentation](https://docs.juce.com)

[JUCE Tutorials](https://juce.com/learn/tutorials)

[JUCE Forums](https://forum.juce.com)

[JUCE Source Code](https://github.com/juce-framework/JUCE)

### MIDI

[Official MIDI Specifications](https://www.midi.org/specifications)

[Summary of MIDI 1.0 Messages](https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message)

[MIDI 2.0 Specifications](https://www.midi.org/specifications/midi-2-0-specifications)

### Synth

[BLIT synth](https://ccrma.stanford.edu/~stilti/papers/blit.pdf)

[Osc and Filter algorithms for Analog Synthesis](https://www.researchgate.net/publication/220386519_Oscillator_and_Filter_Algorithms_for_Virtual_Analog_Synthesis)

[SVF Linear Trap Optimised](http://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf)

[One Pole Linear LowPass](http://cytomic.com/files/dsp/OnePoleLinearLowPass.pdf)

### User Presets

[Youtube Tutorial](https://www.youtube.com/watch?v=YwAtWuGA4Cg)

[Open Source Implementation](https://github.com/Chowdhury-DSP/chowdsp_utils)

### Extra

[Graphing Calculator](https://www.desmos.com/calculator)