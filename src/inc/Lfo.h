//
//  Lfo.h
//  MUSI6106
//
//  Created by Yanchao Liu on 2/16/20.
//

#ifndef Lfo_h
#define Lfo_h
#include "RingBuffer.h"

class CLfo
{
public:
    CLfo(float sampleRateInHZ, float ModFrequencyInSample):
    fReadIndex(0),
    fSampleRateInHz(sampleRateInHZ),
    fModulationFreqInSample(ModFrequencyInSample)
    {
        if(ModFrequencyInSample > 0)
        {
            iBufferLength = int(1 / ModFrequencyInSample);
            pRingBuffer = new CRingBuffer<float>(iBufferLength);
        }
        if(ModFrequencyInSample == 0)
        {
            iBufferLength = int(1 / ModFrequencyInSample);
            pRingBuffer = new CRingBuffer<float>(iBufferLength);
        }
    }
    
    ~CLfo()
    {
        delete pRingBuffer;
        pRingBuffer = 0;
    }
    
    float CurrentValue()
    {
        float fCurrentValue = pRingBuffer->get(fReadIndex);
        
        
        
        return fCurrentValue;
    }
    
    
    int iBufferLength;
    float fReadIndex;
    float fSampleRateInHz;
    float fModulationFreqInSample;
    CRingBuffer<float> *pRingBuffer;
    
    
};

#endif /* Lfo_h */
