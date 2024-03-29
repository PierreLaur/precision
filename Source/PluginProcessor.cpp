#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PrecisionAudioProcessor::PrecisionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
      )
#endif
#ifndef JucePlugin_Name
#define JucePlugin_Name "precision"
#endif
{
}

PrecisionAudioProcessor::~PrecisionAudioProcessor()
{
}

//==============================================================================
const String PrecisionAudioProcessor::getName() const
{

    return JucePlugin_Name;
}

bool PrecisionAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PrecisionAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PrecisionAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PrecisionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PrecisionAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int PrecisionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PrecisionAudioProcessor::setCurrentProgram(int index)
{
    std::cout << "Received setCurrentProgram request with index " + std::to_string(index) << std::endl;
}

const String PrecisionAudioProcessor::getProgramName(int index)
{
    std::cout << "Received getProgramName request with index " + std::to_string(index) << std::endl;
    return {};
}

void PrecisionAudioProcessor::changeProgramName(int index, const String &newName)
{
    std::cout << "Received changeProgramName request with index " + std::to_string(index) +
                     " and newName " + newName.toStdString()
              << std::endl;
}

//==============================================================================
void PrecisionAudioProcessor::prepareToPlay([[maybe_unused]] double sampleRate, [[maybe_unused]] int samplesPerBlock)
{
}

void PrecisionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PrecisionAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

bool PrecisionAudioProcessor::isPreciseEnough(MidiMessage message, double messageRelativePpqPosition)
{
    if (message.isNoteOn())
    {
        auto rem = std::fmod(messageRelativePpqPosition, quantizationInBeats);
        double ppqPrecisionLimit = msToBeats(precisionLimitMs);
        if (rem <= ppqPrecisionLimit || rem >= quantizationInBeats - ppqPrecisionLimit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

void PrecisionAudioProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    double sampleRate = getSampleRate();
    int numSamples = buffer.getNumSamples();

    // SYNC WITH HOST
    updatePlayHeadInfo(getPlayHead()->getPosition());

    // AUDIO
    {
        ScopedNoDenormals noDenormals;
        auto totalNumInputChannels = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        // In case we have more outputs than inputs, this code clears any output
        // channels that didn't contain input data, (because these aren't
        // guaranteed to be empty - they may contain garbage).
        // This is here to avoid people getting screaming feedback
        // when they first compile a plugin, but obviously you don't need to keep
        // this code if your algorithm always overwrites all the output channels.
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear(i, 0, numSamples);

        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        // Make sure to reset the state if your inner loop is processing
        // the samples and the outer loop is handling the channels.
        // Alternatively, you can process the samples with the channels
        // interleaved by keeping the same state.
    }

    // MIDI
    {

        if (recording)
        {
            double maxRelativePpqPosition = numBars * timeSigNumerator;
            relativePpqPosition = ppqPosition - ppqRecordingStart;
            if (relativePpqPosition >= 0.0)
            {
                for (const auto metadata : midiMessages)
                {
                    double messageRelativePpqPosition = relativePpqPosition + samplesToBeats(metadata.samplePosition, sampleRate);
                    auto message = metadata.getMessage();
                    editor->processMidiMessage(&message, messageRelativePpqPosition, maxRelativePpqPosition);
                }

                // end of the recording zone, loop
                if (relativePpqPosition > maxRelativePpqPosition)
                {
                    editor->stopRecording();
                    editor->startRecording();
                }
            }
        }

        // In filter mode, don't let through MIDI messages that are not precise enough
        if (filtering)
        {
            relativePpqPosition = ppqPosition - ppqRecordingStart;
            if (relativePpqPosition >= 0)
            {
                for (const auto metadata : midiMessages)
                {
                    double messageRelativePpqPosition = relativePpqPosition + samplesToBeats(metadata.samplePosition, sampleRate);
                    auto message = metadata.getMessage();
                    if (isPreciseEnough(message, messageRelativePpqPosition) == false)
                    {
                        midiMessages.clear(metadata.samplePosition, metadata.numBytes);
                    }
                }
            }
        }
    }

    // If standalone, manually update the ppqPosition
    if (isStandalone)
    {
        timeInSamples += numSamples;
        timeInSeconds = timeInSamples / sampleRate;
        ppqPosition = samplesToBeats(timeInSamples, sampleRate);
        ppqPositionOfLastBarStart = std::floor(ppqPosition);
    }
}

//==============================================================================
bool PrecisionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *PrecisionAudioProcessor::createEditor()
{
    editor = new PrecisionAudioProcessorEditor(*this);
    return editor;
}

//==============================================================================
void PrecisionAudioProcessor::getStateInformation([[maybe_unused]] MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PrecisionAudioProcessor::setStateInformation([[maybe_unused]] const void *data, [[maybe_unused]] int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new PrecisionAudioProcessor();
}
