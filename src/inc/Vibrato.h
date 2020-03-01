// TryToFindHomework
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
    float fModulationFreqInIndex;
    int numOfChannel;
    
    CRingBuffer<float> **pptr_RingBuffer;
    CLfo *ptr_CLfo = 0;
    
    
    CVibrato(): basicDelay(0), delayWidth(0), fModulationFreqInIndex(0), numOfChannel(0)
    {

    }
    
    static void create(CVibrato* ptr_vibrato)
    {
        ptr_vibrato = new CVibrato();
    }
    
    void destroy(CVibrato* ptr_vibrato)
    {
        delete ptr_vibrato;
        delete ptr_CLfo;
        ptr_CLfo = 0;
        ptr_vibrato = 0;
    }
    
    void init(int numOfChannel, float modulationFrequencyInHz, float sampleRateInHz, float basicDelayInSec, float delayWidthInSec)
    {
        fModulationFreqInIndex = modulationFrequencyInHz / sampleRateInHz;
        basicDelay = basicDelayInSec * sampleRateInHz;
        delayWidth = delayWidthInSec * sampleRateInHz;
        this->numOfChannel = numOfChannel;
        
        pptr_RingBuffer = new CRingBuffer<float>* [numOfChannel];
        int RingBufferSize = int(2 + basicDelay + delayWidth * 2);
        
        for(int c=0; c<numOfChannel; c++)
        {
            pptr_RingBuffer[c] = new CRingBuffer<float>(RingBufferSize);
        }
        
        ptr_CLfo = new CLfo(sampleRateInHz, fModulationFreqInIndex);
        ptr_CLfo->process();
    }
    
    void process(float **ppInputBuffer, float **ppOutputBlock, long long blockSize)
    {
        /*
         ptr_CVibrato->process(ppfAudioData, ppfOutputData, blockSize);
        */

        float delay = 0;
        float sin_val = 0;
        
        for(int c = 0; c<this->numOfChannel; c++)
        {
            for(int i = 0; i<blockSize; i++)
            {
                sin_val = ptr_CLfo->getSine();
                delay = 1 + delay + delayWidth * sin_val;
                pptr_RingBuffer[c]->putPostInc(ppInputBuffer[c][i]);
                ppOutputBlock[c][i] = pptr_RingBuffer[c]->get(delay);
                pptr_RingBuffer[c]->getPostInc();
            }
        }
        
    }

    
};


#endif /* Vibrato_h */
