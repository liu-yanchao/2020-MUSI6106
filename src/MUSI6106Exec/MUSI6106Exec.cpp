
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Vibrato.h"
#include "Lfo.h"
//#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

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
    CAudioFileIf            *ptr_OutputFile = 0;
    
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    
    float **ppfOutputData = 0;
    
    float fSampleRate = 0;
    float fModFrequencyInHz = 0;
    float fBasicDelayInSec = 0;
    float fModWidthInSec = 0;
    
    CVibrato *ptr_CVibrato = 0;

    //CCombFilterIf   *pInstance = 0;
    //CCombFilterIf::create(pInstance);
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments

    
    if (argc < 6)
    {
        cout << "Missing input argument!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = argv[2];
        fModFrequencyInHz = atof(argv[4]);
        fBasicDelayInSec = atof(argv[5]);
        fModWidthInSec = atof(argv[6]);
    }

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
    fSampleRate = stFileSpec.fSampleRateInHz;
    
    // open output file
    ptr_OutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
//    CVibrato vibrato();
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];
    
    ppfOutputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfOutputData[i] = new float[kBlockSize];

    time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    long long blockSize = kBlockSize;
    
    while (!phAudioFile->isEof())
    {
        phAudioFile->readData(ppfAudioData, blockSize);
        ptr_CVibrato->process(ppfAudioData, ppfOutputData, blockSize);
        ptr_OutputFile->writeData(ppfOutputData, blockSize);
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

