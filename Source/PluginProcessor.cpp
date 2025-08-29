/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include <array>
#include <memory>

auto getPhaserRateName() { return juce::String("Phaser ratehz"); }
auto getPhaserCenterFreqName() { return juce::String("phaser center freq"); }
auto getPhaserDepthName() { return juce::String("Phaser depth%"); }
auto getPhaserFeedbackName() { return juce::String("phaser feedback percent"); }
auto getPhaserMixName() { return juce::String("phaser mix"); }
auto getPhaserBypassName() {return juce::String("Phaser Bypass");}

auto getChorusRateName() { return juce::String("Chorus ratehz"); }
auto getChorusDepthName() { return juce::String("Chorus depth%"); }
auto getChorusCenterDelayName() { return juce::String("Chorus center Delay ms"); }
auto getChorusFeedbackName() { return juce::String("Chorus feedback percent"); }
auto getChorusMixName() { return juce::String("Chorus mix"); }
auto getChorusBypassName() {return juce::String("Chorus Bypass");}

auto getOverdriveSaturationName() { return juce::String("Overdrive Saturation"); }
auto getOverdriveBypassName() {return juce::String("Overdrive Bypass");}

auto getLadderFilterModeName() { return juce::String("Ladder Filter Mode"); }
auto getLadderFilterCutoffName() { return juce::String("Ladder Filter Cutoff Hz"); }
auto getLadderFilterResonanceName() { return juce::String("Ladder Filter Resonance"); }
auto getLadderFilterDriveName() { return juce::String("Ladder Filter Drive"); }
auto getLadderFilterBypassName() {return juce::String("LadderFilter Bypass");}

auto getLadderFilterChoices()
{
    return juce::StringArray
    {
        "LPF12",  // low-pass  12 dB/octave
        "HPF12",  // high-pass 12 dB/octave
        "BPF12",  // band-pass 12 dB/octave
        "LPF24",  // low-pass  24 dB/octave
        "HPF24",  // high-pass 24 dB/octave
        "BPF24"   // band-pass 24 dB/octav
    };
}

auto getGeneralFilterChoices()
{
    return juce::StringArray
    {
        "Peak",
        "bandpass",
        "notch",
        "allpass",
    };
}

auto getGeneralFilterModeName() {return juce::String("gereral filter mode"); }
auto getGeneralFilterFreqName() { return juce::String("general filter frequency hz"); }
auto getGeneralFilterQuatlityName() { return juce::String("genrel filter quality"); }
auto getGeneralFilterGainName() { return juce::String("general filter gain"); }
auto getGeneralFilterBypassName() {return juce::String("GeneralFilter Bypass");}
//==============================================================================
Project13_NewAudioProcessor::Project13_NewAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
  dspOrder = 
  {{
      DSP_Option::Phase,
      DSP_Option::Chorus,
      DSP_Option::Overdrive,
      DSP_Option::LadderFilter
    }};
    auto floatParams = std::array
    {
        &phaserRateHz,
        &phaserCenterFreqHz,
        &phaserDepthPercent,
        &phaserFeedbackPercent,
        &phaserMixPercent,

        &chorusRateHz,
        &chorusDepthPercent,
        &chorusCenterDelayMs,
        &chorusFeedbackPercent,
        &chorusMixPercent,

        &overdriveSaturation,

         &ladderFilterCutoffHz,
         &ladderFilterResonance,
         &ladderFilterDrive,

          
          &generalilterFreqHz,
          &generalilterQuality,
          &generalilterGain,

    };

    auto floatNameFuncs = std::array
    {
        &getPhaserRateName,
        &getPhaserCenterFreqName,
        &getPhaserDepthName,
        &getPhaserFeedbackName,
        &getPhaserMixName,

        &getChorusRateName,
        &getChorusDepthName,
        &getChorusCenterDelayName,
        &getChorusFeedbackName,
        &getChorusMixName,

        &getOverdriveSaturationName,

        &getLadderFilterCutoffName,
        &getLadderFilterResonanceName,
        &getLadderFilterDriveName,

        &getLadderFilterCutoffName,
        &getLadderFilterResonanceName,
        &getLadderFilterDriveName,
 

    };

    for (size_t i = 0; i < floatParams.size(); ++i)
    {
        auto ptrToParamPtr = floatParams[i];
        *ptrToParamPtr = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(floatNameFuncs[i]()));
        jassert(*ptrToParamPtr != nullptr);
    }

    auto choiceParams = std::array
    {
        &ladderFilterMode,
        &generalFilterMode,
    };

    auto choiceNameFuncs = std::array
    {
        &getLadderFilterModeName,
        &getGeneralFilterModeName,
    };

    for (size_t i = 0; i < choiceParams.size(); ++i)
    {
        auto ptrToParamPtr = choiceParams[i];
        *ptrToParamPtr = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(choiceNameFuncs[i]()));
        jassert(*ptrToParamPtr != nullptr);
    }

  auto bypassParams = std::array
  {
    &phaserBypass,
    &chorusBypass,
    &overdriveBypass,
    &ladderFilterBypass,
    &generalFilterBypass,
                       };
  auto bypassNameFuncs = std::array
                       {
                       &getPhaserBypassName,
                       &getChorusBypassName,
                       &getOverdriveBypassName,
                       &getLadderFilterBypassName,
                       &getGeneralFilterBypassName,
                       };
  for(size_t i=0; i<bypassParams.size(); ++i)
                       {
                       auto ptrToParamPtr = bypassParams[i];
                       *ptrToParamPtr = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(bypassNameFuncs[i]()));
                       jassert(*ptrToParamPtr != nullptr);

                       }
}
Project13_NewAudioProcessor::~Project13_NewAudioProcessor()
{
}

//==============================================================================
const juce::String Project13_NewAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Project13_NewAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Project13_NewAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Project13_NewAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Project13_NewAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Project13_NewAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Project13_NewAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Project13_NewAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Project13_NewAudioProcessor::getProgramName (int index)
{
    return {};
}

void Project13_NewAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Project13_NewAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate= sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

  
  std::vector<juce::dsp::ProcessorBase*> dsp
  {
    &phaser,
    &chorus,
    &overdrive,
    &ladderFilter,
    &generalFilter
  };

  for(auto p : dsp)
  {
    p->prepare(spec);
    p->reset();
  }
}

void Project13_NewAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Project13_NewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

juce::AudioProcessorValueTreeState::ParameterLayout Project13_NewAudioProcessor::createParameterLayout() {

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //    name = namefunction
    //    layout.add(std::make_unique<juce::AudioParameters<float>>(
    //        juce::ParametersId{ name,version hint })
    //        name,
    //        parameterrange,
    //        defaultrange,
    //        unitsuffix);

    const int versionHint = 1;

    /*
        Phaser:
        Rate: hz
        Depth: 0-1
        Center freq: hz
        Feedback: -1 to 1
        Mix: 0 ot 1
    */

    auto name = getPhaserRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 2.f, 0.01f, 1.f),
        0.2f,
        "Hz"));

    //phaser depth 0 to -1
    name = getPhaserDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 2.f, 0.01f, 1.f),
        0.05f,
        "%"));

    //phaser center frequency
    name = getPhaserCenterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(20.f, 2000.f, 1.f, 1.f),
        1000.f,
        "Hz"));
    //phaser feedback
    name = getPhaserFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f),
        0.0f,
        "%"));
    //phaser mix
    name = getPhaserMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f,
        "%"));
    name = getPhaserBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{name,versionHint},
    name,false));

    /*
    Chorus:
    Rate: hz
    Depth: 0-1
    Center delay: 0 to  100 ms
    Feedback: -1 to 1
    Mix: 0 ot 1
*/
    name = getChorusRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f),
        0.2f,
        "Hz"));
    //chorus depth
    name = getChorusDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f,
        "%"));
    //chorus center delay
    name = getChorusCenterDelayName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f),
        7.f,
        "%"));
    //chorus feedback
    name = getChorusFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f),
        0.0f,
        "%"));
    //chorus  mix
    name = getChorusMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f,
        "%"));
    name = getChorusBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{name,versionHint},name,false));
    /*
        overdrive
        uses the drive portion of the ladder filter class for now
        drive 1-100
    */
    //drive 1-100
    name = getOverdriveSaturationName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f),
        1.f,
        ""));

  name = getOverdriveBypassName();
  layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{name,versionHint},name,false));
    /*Ladder
        mode: ladder filter enum(int)
        cutoff: hz
        resonance: 0 to 1
        drive 1-100
    */

    name = getLadderFilterModeName();
    auto choices = getLadderFilterChoices();
    layout.add(std::make_unique<juce::AudioParameterChoice>
    (
        juce::ParameterID{name,versionHint},
        name,
        choices,
        0
    ));

    name = getLadderFilterCutoffName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(20.f, 20000.f, 0.1f, 1.f),
        20000.f,
        "Hz"));
    name = getLadderFilterResonanceName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
        0.f,
        ""));
    name = getLadderFilterDriveName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,versionHint },
        name,
        juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f),
        1.f,
        ""));
    name = getLadderFilterBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{name,versionHint},name,false));

    /*General Filter
        
        Mode: Peak, bandpass,  notch, allpass
        freq: 20hz -20000hz in 1hz steps
        Q: 0.1 - 10 in 0.05 steps
        gain: -24db to 24 db in 0.5 db increments
     */
    //mode
    name = getGeneralFilterModeName();
    choices = getGeneralFilterChoices();
    layout.add(std::make_unique<juce::AudioParameterChoice>
        (
            juce::ParameterID{ name,versionHint },
            name,
            choices,
            0
        ));
    //freq
    name = getGeneralFilterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (
            juce::ParameterID{ name,versionHint },
            name,
            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
            750.f,
            "Hz"
        ));
    //quality
    name = getGeneralFilterQuatlityName();
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (
            juce::ParameterID{ name,versionHint },
            name,
            juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
            1.f,
            ""
        ));
    //gain
    name = getGeneralFilterGainName();
    layout.add(std::make_unique < juce::AudioParameterFloat >
        (
            juce::ParameterID{ name,versionHint },
            name,
            juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f),
            0.f,
            "dB"
        ));
    name = getGeneralFilterBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{name,versionHint},name,false));
    return layout;

}

void Project13_NewAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //[DONE]: add APVTS
    //[DONE]: create audio parameteres for all audio parameter
    //[DONE]: update DSP here from audio parameters
    //[TODO]: update genral filter corrections
    //[TODO]: add smother for all param update
    //[DONE]: save/load settings
    //[DONE]: save/load DSP order
    //[TODO]: filters are mono not stereo
    //[TODO]: drag to reorder gui
    //[TODO]: GUI design for each dsp instance
    //[TODO]: metering
    //[DONE]: preparing all dsp
  phaser.dsp.setRate(phaserRateHz->get());
  phaser.dsp.setCentreFrequency(phaserCenterFreqHz->get());
  phaser.dsp.setDepth(phaserDepthPercent->get());
  phaser.dsp.setFeedback(phaserFeedbackPercent->get());
  phaser.dsp.setMix(phaserMixPercent->get());

  chorus.dsp.setRate(chorusRateHz->get());
  chorus.dsp.setDepth(chorusDepthPercent->get());
  chorus.dsp.setCentreDelay(chorusCenterDelayMs->get());
  chorus.dsp.setFeedback(chorusFeedbackPercent->get());
  chorus.dsp.setMix(chorusMixPercent->get());
  overdrive.dsp.setDrive(overdriveSaturation->get());

  ladderFilter.dsp.setMode(
    static_cast<juce::dsp::LadderFilterMode>(ladderFilterMode->getIndex())
  );
  ladderFilter.dsp.setCutoffFrequencyHz(ladderFilterCutoffHz->get());
  ladderFilter.dsp.setResonance(ladderFilterResonance->get());
  ladderFilter.dsp.setDrive(ladderFilterDrive->get());

    auto newDSPOrder = DSP_Order();

    //try to pull
    while (dspOrderFifo.pull(newDSPOrder))
    {

    }

    //if you pull replace dsp order
    if (newDSPOrder != DSP_Order())
        dspOrder = newDSPOrder;
    //now convert dspOrder into an array of pointers
    DSP_Pointers dspPointers;
    dspPointers.fill(nullptr); 

    for (size_t i = 0; i < dspPointers.size(); ++i)
    {
        switch (dspOrder[i])
        {
         case DSP_Option::Phase:
                dspPointers[i] = &phaser;
                break;
         case DSP_Option::Chorus: 
                dspPointers[i] = &chorus;
                break;
         case DSP_Option::Overdrive:
                dspPointers[i] = &overdrive;
                break;
         case DSP_Option::LadderFilter:
                dspPointers[i] = &ladderFilter;
                break;
        case DSP_Option::GeneralFilter:
                dspPointers[i] = &generalFilter; 
         case DSP_Option::END_OF_LIST:
                jassertfalse;
                break;
        }
    }
    //now processs:
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    for (size_t i = 0; i < dspPointers.size();++i)
    {
        if (dspPointers[i] != nullptr)
        {
            dspPointers[i]->process(context);
        }
    }
    
}
  
//==============================================================================
bool Project13_NewAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Project13_NewAudioProcessor::createEditor()
{
    return new Project13_NewAudioProcessorEditor (*this);
  //return new juce::GenericAudioProcessorEditor(*this);
}

template<>
struct juce::VariantConverter<Project13_NewAudioProcessor::DSP_Order> {

  static Project13_NewAudioProcessor::DSP_Order fromVar( const juce::var& v)
  {
    using T = Project13_NewAudioProcessor::DSP_Order;

    T dspOrder;

    jassert(v.isBinaryData());

    if(v.isBinaryData()==false)
    {
      dspOrder.fill(Project13_NewAudioProcessor::DSP_Option::END_OF_LIST);
    }
    else
    {
      auto mb = *v.getBinaryData();
      juce::MemoryInputStream mis(mb,false);
      std::vector<int> arr;
      while( !mis.isExhausted())
      {
        arr.push_back(mis.readInt());

      }

      jassert(arr.size()==dspOrder.size());

      for(size_t i=0; i<dspOrder.size();++i)
      {
        dspOrder[i]= static_cast<Project13_NewAudioProcessor::DSP_Option>(arr[i]);
      }

    }

    return dspOrder;
  }

  static juce::var toVar(const Project13_NewAudioProcessor::DSP_Order& t)
  {
    juce::MemoryBlock mb;
    //juce mos uses scoping to complete write memory block correctly
    {
      juce::MemoryOutputStream  mos(mb,false);

      for(const auto& v : t)
      {
        mos.writeInt(static_cast<int>(v));
      }
    }
    return mb;
  }
  

};
//==============================================================================
void Project13_NewAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
  apvts.state.setProperty("dspOrder",juce::VariantConverter<Project13_NewAudioProcessor::DSP_Order>::toVar(dspOrder),nullptr);
  juce::MemoryOutputStream mos(destData,false);
  apvts.state.writeToStream(mos);

}

void Project13_NewAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
  auto tree = juce::ValueTree::readFromData(data,sizeInBytes);

  if(tree.isValid())
  {
    apvts.replaceState(tree);

    if(apvts.state.hasProperty("dspOrder"))
    {
      auto order = juce::VariantConverter<Project13_NewAudioProcessor::DSP_Order>::fromVar(apvts.state.getProperty("dspOrder"));
      dspOrderFifo.push(order);
    }
    DBG(apvts.state.toXmlString()); 
  }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Project13_NewAudioProcessor();
}
