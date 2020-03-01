//
//  Lfo.h
//  MUSI6106
//
//  Created by Yanchao Liu on 2/16/20.
//

#ifndef Lfo_h
#define Lfo_h
#include "RingBuffer.h"
#include "Synthesis.h"
#include <math.h>

class CLfo
{
public:
    CLfo(float sampleRateInHZ, float ModFrequencyInHz):
    fReadIndex(0),
    fSampleRateInHz(sampleRateInHZ),
    fModulationFreqInHz(ModFrequencyInHz)
    {
        fModulationFreqInSample = ModFrequencyInHz / sampleRateInHZ;
        if(fModulationFreqInSample > 0)
        {
            iBufferLength = int(1 / fModulationFreqInSample);
            pRingBuffer = new CRingBuffer<float>(iBufferLength);
        }
        if(fModulationFreqInSample == 0)
        {
            iBufferLength = int(1 / fModulationFreqInSample);
            pRingBuffer = new CRingBuffer<float>(iBufferLength);
        }
    }
    
    ~CLfo()
    {
        delete pRingBuffer;
        pRingBuffer = 0;
    }
//    unfinished
    float getSine()
    {
        float fCurrentSine = pRingBuffer->get(fReadIndex);
        fReadIndex = fmodf(fReadIndex + fModulationFreqInSample * iBufferLength, iBufferLength);
        return fCurrentSine;
    }
    
    void process()
    {
        ptr_dataBuffer = new float[iBufferLength];
//        generate sine with synthesis function
        CSynthesis::generateSine (ptr_dataBuffer, fModulationFreqInHz, fModulationFreqInSample, iBufferLength);
//        passing into pRingBuffer
        pRingBuffer->putPostInc(ptr_dataBuffer, iBufferLength);
        delete [] ptr_dataBuffer;
        ptr_dataBuffer = 0;
    }
    
    
    int iBufferLength;
    float fReadIndex;
    float fSampleRateInHz;
    float fModulationFreqInHz;
    float fModulationFreqInSample;
    CRingBuffer<float> *pRingBuffer;
    float *ptr_dataBuffer = 0;
    
    
};

#endif /* Lfo_h */
