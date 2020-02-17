//
//  Vibrato.h
//  MUSI6106
//
//  Created by Yanchao Liu on 2/12/20.
//

#ifndef Vibrato_h
#define Vibrato_h

#include "Lfo.h"

class CVibrato
{
public:
    // Parameters
    float basicDelay;
    float delayWidth;
    float sampleRateInSample;
    int numOfChannel;
    
    CVibrato(): basicDelay(0), delayWidth(0), sampleRateInSample(0), numOfChannel(0)
    {

    }
    
    void init(float modulationFrequencyInHz, float sampleRateInHz)
    {
        
    }
    
    enum ParamType
    {
        basicDelay_t,
        delayWidth_t,
        sampleRate_t,
        modFrequency
    };
    
    bool isParamValid(ParamType param_t, float param)
    {
        switch (param_t)
        {
            case sampleRate_t:
                if(param > 0)
                    return true;
                else
                    return false;
            case basicDelay_t:
                if(param > 0)
                    return true;
                else
                    return false;
            case delayWidth_t:
                if(param < basicDelay && param > 0)
                    return true;
                else
                    return false;
                break;
                
            default:
                break;
        }
        return true;
    }
    
    void process(CAudioFileIf*& pInputBlock, CAudioFileIf*& pOutputBlock)
    {
        
    }

    
};


#endif /* Vibrato_h */
