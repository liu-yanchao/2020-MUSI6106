// Yanchao Liu
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"
#include "RingBuffer.h"

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

//    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    //CCombFilterIf   *pInstance = 0;
    //CCombFilterIf::create(pInstance);
    
    int block_num = 1;
    CFft *ptr_CFft = 0;
    float *ptr_fft_buffer = 0;
    float **pptr_spectrum = 0;
    float *pMagnitude = 0;
    float **pptr_hopsize_audiodata = 0;
    CRingBuffer<float> *ptr_RingBuffer = 0;
    
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 4)
    {
        cout << "Missing input arguments";
        return -1;
    }
    
    sInputFilePath = argv[1];
    sOutputFilePath = sInputFilePath + ".txt";
    const int kBlockSize = atoi(argv[2]);
    const int con_hopsize = atoi(argv[3]);
    long long int num_of_samples_to_read = kBlockSize;
    

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
    
//    assume the file is in mono
    assert(stFileSpec.iNumChannels == 1);

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
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    pptr_spectrum = new float*[stFileSpec.iNumChannels];
    ptr_RingBuffer = new CRingBuffer<float>(kBlockSize);
    ptr_fft_buffer = new float[kBlockSize];
    pMagnitude = new float[kBlockSize];
    pptr_hopsize_audiodata = new float*[stFileSpec.iNumChannels];
    cout << "number of channel is " << stFileSpec.iNumChannels << endl;

    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        ppfAudioData[i] = new float[kBlockSize];
        pptr_spectrum[i] = new CFft::complex_t[kBlockSize];
        pptr_hopsize_audiodata[i] = new float[con_hopsize];
    }

    time = clock();
    
    // Create and initialize FFT
    CFft::createInstance(ptr_CFft);
    ptr_CFft->initInstance(kBlockSize);
    
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    // For the first block, read blocksize number of samples
    cout << "\r" << "reading and writing block " << block_num;
    phAudioFile->readData(ppfAudioData, num_of_samples_to_read);
    ptr_RingBuffer->putPostInc(ppfAudioData[0], num_of_samples_to_read);
    for(int i = 0; i<kBlockSize; i++)
    {
        ptr_fft_buffer[i] = ptr_RingBuffer->getPostInc();
    }
    ptr_CFft->doFft(pptr_spectrum[0], ptr_fft_buffer);
    ptr_CFft->getMagnitude(pMagnitude, pptr_spectrum[0]);
    for(int i=0; i<kBlockSize; i++)
    {
        hOutputFile << pMagnitude[i] << "\t";
    }
    hOutputFile << endl;
    
    // For rest of the blocks, read hopsize number of samples
    while (!phAudioFile->isEof())
    {
        num_of_samples_to_read = con_hopsize;
        phAudioFile->readData(pptr_hopsize_audiodata, num_of_samples_to_read);
        ptr_RingBuffer->setReadIdx(ptr_RingBuffer->getWriteIdx());
        for(int i = 0; i<kBlockSize; i++)
        {
            ptr_fft_buffer[i] = ptr_RingBuffer->getPostInc();
        }
        ptr_CFft->doFft(pptr_spectrum[0], ptr_fft_buffer);
        ptr_CFft->getMagnitude(pMagnitude, pptr_spectrum[0]);
        for(int i=0; i<kBlockSize; i++)
        {
            hOutputFile << pMagnitude[i] << "\t";
        }
        hOutputFile << endl;
        
        block_num++;

        cout << "\r" << "reading and writing block " << block_num;

//        for (int i = 0; i < iNumFrames; i++)
//        {
//            for (int c = 0; c < stFileSpec.iNumChannels; c++)
//            {
//                hOutputFile << ppfAudioData[c][i] << "\t";
//            }
//            hOutputFile << endl;
//        }

    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();
    
    CFft::destroyInstance(ptr_CFft);

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        delete[] ppfAudioData[i];
        delete [] pptr_spectrum[i];
        delete [] pptr_hopsize_audiodata[i];
    }
    
    delete[] ppfAudioData;
    delete [] pptr_hopsize_audiodata;
    delete [] pptr_spectrum;
    delete ptr_RingBuffer;
    delete [] pMagnitude;
    delete [] ptr_fft_buffer;
    pptr_hopsize_audiodata = 0;
    ppfAudioData = 0;
    pptr_spectrum = 0;
    ptr_RingBuffer = 0;
    pMagnitude = 0;
    ptr_fft_buffer = 0;
    
    

    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout << "Yanchao Liu's Homework" << endl;
    cout  << endl;

    return;
}

