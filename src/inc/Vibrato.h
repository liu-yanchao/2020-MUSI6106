//
//  Vibrato.h
//  MUSI6106
//
//  Created by Yanchao Liu on 2/12/20.
//

#ifndef Vibrato_h
#define Vibrato_h

#include "AudioFileIf.h"
#include "Lfo.h"

///create(ptr_vibrato)
///init(, ModulationFrequencyInHz, Basic Delay in Sec, Channel Number)
///setParam(ModulationFreqInHz, ModulationWidthInSec);
///process(pptr_audioData, pptr_AudioOutputData, numOfFrames)
///
///destroy(ptr_vibrato)
///

class CVibrato
{
public:
    // Parameters
    float basicDelay;
    float delayWidth;
    float sampleRateInSample;
    float fModulationFreq;
    int numOfChannel;
    
    CRingBuffer<float> **pptr_RingBuffer;
    CLfo *ptr_Clfo;
    
    
    CVibrato(): basicDelay(0), delayWidth(0), sampleRateInSample(0), numOfChannel(0)
    {

    }
    
    void create(CVibrato* ptr_vibrato)
    {
        ptr_vibrato = new CVibrato();
    }
    
    void destroy(CVibrato* ptr_vibrato)
    {
        delete ptr_vibrato;
        ptr_vibrato = 0;
    }
    
    void init(float modulationFrequencyInHz, float sampleRateInHz)
    {
        fModulationFreq = modulationFrequencyInHz;
//        sampleRateInSample = sampleRateInHz;
    }
    

    
    void process(float **pInputBuffer, float ** pOutputBlock, long long blockSize)
    {
        /*
         ptr_CVibrato->process(ppfAudioData, ppfOutputData, blockSize);

        */
    }

    
};


#endif /* Vibrato_h */
