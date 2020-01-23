
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void    iirFilter(float** prtInput, float** prtOutput, int iFileLength, int iChannels, float fs, float t0, float g, float c)
{
    int iNumOfDelay = int(t0 * fs);
    for(int i = 0; i<iChannels; i++)
    {
        for(int j = 0; j<iFileLength; j++)
        {
            if(j>=iNumOfDelay)
            {
                prtOutput[i][j] = c*prtInput[i][j] + g*prtOutput[i][j - iNumOfDelay];
            }
            else
            {
                prtOutput[i][j] = c*prtInput[i][j];
            }
        }
    }
}

bool firFilter(float** prtInput, float** prtOutput, int iFileLength, int iChannels, float fs, float t0, float g)
{
    int iNumOfDelay = int(t0 * fs);
    for(int i = 0; i<iChannels; i++)
    {
        for(int j = 0; j<iFileLength; j++)
        {
            if(j>=iNumOfDelay)
            {
                prtOutput[i][j] = prtInput[i][j] + g*prtInput[i][j - iNumOfDelay];
            }
            else
            {
                prtOutput[i][j] = prtInput[i][j];
            }
        }
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CAudioFileIf::FileSpec_t *pStFileSpec = &stFileSpec;
    
    float filterDelayTime = {0.0002268};
    std::string filterType;
    float filterGain = {0.5};
    float filterIirC = {1};
    int iNumSampleDelay = {0};
    long long iLengthInFrame = {0};
    float **ppfOutputData = 0;
    float **ppfInputData = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    showClInfo();
//  inputFilePath, outputFilePath, filterDelayTime, filterType, filterGain,
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
//    if (argc == 1)
//    {
//        cout << "Running default test cases" << endl;
//        return -1;
//    }
//    if (argc != 6)
//    {
//        cout << "Please parse argument by 'inputFilePath, outputFilePath, filterDelayTime, filterType, filterGain'" << endl;
//        return -1;
//    }

    sInputFilePath = argv[1];
    sOutputFilePath = argv[2];
    filterDelayTime = atof(argv[3]);
    filterType = argv[4];
    filterGain = atof(argv[5]);
    filterIirC = atof(argv[6]);
    
    
    sInputFilePath = "/Users/yanchaoliu/Study/Spring\ 2020\ Local/MUSI\ 6106\ Audio\ Software\ Eng/Demo_Project/Audio/sweep.wav";
    sOutputFilePath = "/Users/yanchaoliu/Study/Spring\ 2020\ Local/MUSI\ 6106\ Audio\ Software\ Eng/Demo_Project/Audio/sweep_output.wav";


    
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    
    phAudioFile->getFileSpec(stFileSpec);
    phAudioFile->getLength(iLengthInFrame);
    cout << iLengthInFrame << endl;

    //////////////////////////////////////////////////////////////////////////////
    // open the output wav file
    CAudioFileIf::create(phOutputAudioFile);
    phOutputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, pStFileSpec);
    if (!phOutputAudioFile->isOpen())
    {
        cout << "Output file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];
    
    ppfOutputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfOutputData[i] = new float[iLengthInFrame];
    
    ppfInputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfInputData[i] = new float[iLengthInFrame];

    time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    int iPosition = 0;
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        
        cout << "\r" << "reading and writing";
        phOutputAudioFile->writeData(ppfAudioData, iNumFrames);
        for (int c = 0; c < stFileSpec.iNumChannels; c++)
        {
            for (int i = 0; i < iNumFrames; i++)
            {
                ppfOutputData[c][i + iPosition] = ppfAudioData[c][i];
                ppfInputData[c][i + iPosition] = ppfAudioData[c][i];
            }
        }
        iPosition += iNumFrames;
    }
    //cout << (ppfOutputData[0][0]) << endl;
    //phOutputAudioFile->writeData(ppfOutputData, iLengthInFrame);
    

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;
    //////////////////////////////////////////////////////////////////////////////
    // process audio data
//    firFilter(ppfInputData, ppfOutputData, iLengthInFrame, stFileSpec.iNumChannels, stFileSpec.fSampleRateInHz, filterDelayTime, filterGain);
    firFilter(ppfInputData, ppfOutputData, iLengthInFrame, stFileSpec.iNumChannels, stFileSpec.fSampleRateInHz, filterDelayTime, filterGain);
//    iirFilter(ppfInputData, ppfOutputData, iLengthInFrame, stFileSpec.iNumChannels, stFileSpec.fSampleRateInHz, filterDelayTime, filterGain, filterIirC);
    //////////////////////////////////////////////////////////////////////////////
    // output audio data
    iPosition = 0;
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        
        cout << "\r" << "reading and writing";
        phOutputAudioFile->writeData(ppfAudioData, iNumFrames);
        for (int c = 0; c < stFileSpec.iNumChannels; c++)
        {
            for (int i = 0; i < iNumFrames; i++)
            {
                ppfAudioData[c][i] = ppfAudioData[c][i+iPosition];
            }
        }
        iPosition += iNumFrames;
        phOutputAudioFile->writeData(ppfAudioData, iNumFrames);
    }
    
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfInputData[i];
    delete[] ppfInputData;
    ppfInputData = 0;
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfOutputData[i];
    delete[] ppfOutputData;
    ppfOutputData = 0;

    return 0;

}



// input: pointer of buffer
// output: none
// modify the input buffer with FIR or IIR filter
//void firFilter(const float** prtInput, float** prtOutput, const int iFileLength, const int iChannels, const float fs, const float t0, const float g)
//{
//    int iNumOfDelay = int(t0 * fs);
//    for(int i = 0; i<iChannels; i++)
//    {
//        for(int j = 0; j<iFileLength; j++)
//        {
//            if(j>=iNumOfDelay)
//            {
//                prtOutput[i][j] = prtInput[i][j] + g*prtInput[i][j - iNumOfDelay];
//            }
//            else
//            {
//                prtOutput[i][j] = prtInput[i][j];
//            }
//        }
//    }
//
//}

//void IirFilter(const float** prtInput, float** prtOutput, const int iFileLength, const int iChannels, const float fs, const float t0, const float g, const float c)
//{
//    int iNumOfDelay = int(t0 * fs);
//    for(int i = 0; i<iChannels; i++)
//    {
//        for(int j = 0; j<iFileLength; j++)
//        {
//            if(j>=iNumOfDelay)
//            {
//                prtOutput[i][j] = c*prtInput[i][j] + g*prtOutput[i][j - iNumOfDelay];
//            }
//            else
//            {
//                prtOutput[i][j] = c*prtInput[i][j];
//            }
//        }
//    }
//}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch!!" << endl;
    cout  << endl;

    return;
}


