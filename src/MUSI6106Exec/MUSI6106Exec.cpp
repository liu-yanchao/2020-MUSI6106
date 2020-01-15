
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"

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
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CAudioFileIf::FileSpec_t* ptr_stFileSpec={&stFileSpec};
    
    
//    My variables

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
//    sInputFilePath = argv[2];
    sInputFilePath = "/Users/yanchaoliu/Study/Spring 2020 Local/MUSI 6106 Audio Software Eng/Demo_Project/Audio/sweep.wav";
    sOutputFilePath = "/Users/yanchaoliu/Study/Spring 2020 Local/MUSI 6106 Audio Software Eng/Demo_Project/Text/output.txt";
    std::cout << sInputFilePath << std::endl;
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    phAudioFile->create (phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    phAudioFile->getFileSpec(stFileSpec);
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if(!hOutputFile.is_open())
    {
        std::cout << "Open text file error" << std::endl;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    long long int length = 0;

    phAudioFile->getLength(length);
    int iChannel = stFileSpec.iNumChannels;
//    int iNumOfSample = (*phAudioFile).getLength();
    ppfAudioData = new float* [iChannel];
    long long iNumSample = kBlockSize;
    for(int i=0;i<iChannel;i++)
        ppfAudioData[i] = new float[kBlockSize];
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    while(!phAudioFile->isEof())
    {
        phAudioFile->readData(ppfAudioData, iNumSample);
        for(int j = 0; j<iNumSample; j++)
        {
            for(int i = 0; i<iChannel; i++)
            {
                hOutputFile << ppfAudioData[i][j] << "\t";
            }
            hOutputFile << std::endl;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    phAudioFile->closeFile();
    phAudioFile->destroy(phAudioFile);
    hOutputFile.close();
    for(int i = 0; i<iChannel; i++)
    {
        delete[] ppfAudioData[i];
    }
    delete [] ppfAudioData;
    // all done
    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

