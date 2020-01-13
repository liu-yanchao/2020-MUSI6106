
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
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead, ptr_stFileSpec);
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    long long int length = 0;
    phAudioFile->getFileSpec(stFileSpec);
    phAudioFile->getLength(length);
    int iChannel = stFileSpec.iNumChannels;
//    int iNumOfSample = (*phAudioFile).getLength();
    *ppfAudioData = new float [iChannel];
    for(int i=0;i<iChannel;i++)
        ppfAudioData[i] = new float[length];
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)


    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    phAudioFile->closeFile();
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

