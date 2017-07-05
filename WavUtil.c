
#include "WavUtil.h"

int importMonoWav(MonoWAV *pcm, char *fileName) {
    FILE *fp;
    unsigned long i;

    char riffHead[4];
    long riffLength;
    char riffType[4];
    char fmtHead[4];
    long fmtLength;
    short fmtWavFormat, fmtChannels;
    long fmtSamplesPerSec, fmtBytesPerSec;
    short fmtBlockSize, fmtBitsPerSample, fmtJunk;
    char dataHead[4];
    unsigned long dataLength;
    short data;
    
    fp = fopen(fileName, "rb");

    if (fp == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    fread(riffHead, 1, 4, fp);
    fread(&riffLength, 4, 1, fp);
    fread(riffType, 1, 4, fp);
    fread(fmtHead, 1, 4, fp);
    fread(&fmtLength, 4, 1, fp);
    fread(&fmtWavFormat, 2, 1, fp);
    fread(&fmtChannels, 2, 1, fp);
    fread(&fmtSamplesPerSec, 4, 1, fp);
    fread(&fmtBytesPerSec, 4, 1, fp);
    fread(&fmtBlockSize, 2, 1, fp);
    fread(&fmtBitsPerSample, 2, 1, fp);
    printf("fmtLength : %lu\n", fmtLength);
    for (i = 16; i < (int)fmtLength; i+=2) {
        fread(&fmtJunk, 2, 1, fp);
    }
    fread(dataHead, 1, 4, fp);
    fread(&dataLength, 4, 1, fp);
    
    pcm->channels = fmtChannels;
    pcm->sampleRate = fmtSamplesPerSec;
    pcm->bps = fmtBitsPerSample;
    pcm->length = dataLength / 2;

    printf("fmtSamplesPerSec : %lu\n", fmtSamplesPerSec);
    printf("fmtBitsPerSample : %hu\n", fmtBitsPerSample);
    printf("dataLength : %lu\n", dataLength);
    printf("length : %u\n", pcm->length);

    pcm->data = calloc(pcm->length, sizeof(double));
    
    for (i = 0; i < pcm->length; i++) {
        fread(&data, 2, 1, fp);
        pcm->data[i] = (double)data / 32768.0;
    }
    
    fclose(fp);

    return 0;
}

void exportMonoWav(MonoWAV *pcm, char *fileName) {
    FILE *fp;
    unsigned int i;
    char riffHead[4];
    long riffLength;
    char riffType[4];
    char fmtHead[4];
    long fmtLength;
    short fmtWavFormat, fmtChannels;
    long fmtSamplesPerSec, fmtBytesPerSec;
    short fmtBlockSize, fmtBitsPerSample;
    char dataHead[4];
    unsigned long dataLength;
    short data;
    double s;
    
    riffHead[0] = 'R';
    riffHead[1] = 'I';
    riffHead[2] = 'F';
    riffHead[3] = 'F';
    riffLength = 36 + pcm->length * 2;
    riffType[0] = 'W';
    riffType[1] = 'A';
    riffType[2] = 'V';
    riffType[3] = 'E';
    
    fmtHead[0] = 'f';
    fmtHead[1] = 'm';
    fmtHead[2] = 't';
    fmtHead[3] = ' ';
    fmtLength = 16;
    fmtWavFormat = 1;
    fmtChannels = 1;
    fmtSamplesPerSec = pcm->sampleRate;
    fmtBytesPerSec = pcm->sampleRate * pcm->bps / 8;
    fmtBlockSize = pcm->bps / 8;
    fmtBitsPerSample = pcm->bps;
    
    printf("length : %u\n", pcm->length);
    printf("fmtSamplesPerSec : %lu\n", fmtSamplesPerSec);
    printf("fmtBytesPerSec : %lu\n", fmtBytesPerSec);
    printf("fmtBlockSize : %hu\n", fmtBlockSize);
    printf("fmtBitsPerSample : %u\n", fmtBitsPerSample);
    
    dataHead[0] = 'd';
    dataHead[1] = 'a';
    dataHead[2] = 't';
    dataHead[3] = 'a';
    dataLength = pcm->length * 2;
    
    fp = fopen(fileName, "wb");
    
    fwrite(riffHead, 1, 4, fp);
    fwrite(&riffLength, 4, 1, fp);
    fwrite(riffType, 1, 4, fp);
    fwrite(fmtHead, 1, 4, fp);
    fwrite(&fmtLength, 4, 1, fp);
    fwrite(&fmtWavFormat, 2, 1, fp);
    fwrite(&fmtChannels, 2, 1, fp);
    fwrite(&fmtSamplesPerSec, 4, 1, fp);
    fwrite(&fmtBytesPerSec, 4, 1, fp);
    fwrite(&fmtBlockSize, 2, 1, fp);
    fwrite(&fmtBitsPerSample, 2, 1, fp);
    fwrite(dataHead, 1, 4, fp);
    fwrite(&dataLength, 4, 1, fp);
    
    for (i = 0; i < pcm->length; i++) {
        s = (pcm->data[i] + 1.0) / 2.0 * 65536.0;
        
        if (s > 65535.0) {
            s = 65535.0;
        } else if (s < 0.0) {
            s = 0.0;
        }
        
        data = (short)(s + 0.5) - 32768;
        fwrite(&data, 2, 1, fp);
    }
    
    fclose(fp);
}

int importStereoWav(StereoWAV *pcm, char *fileName) {
    FILE *fp;
    unsigned long i;
    char riffHead[4];
    long riffLength;
    char riffType[4];
    char fmtHead[4];
    long fmtLength;
    short fmtWavFormat, fmtChannels;
    long fmtSamplesPerSec, fmtBytesPerSec;
    short fmtBlockSize, fmtBitsPerSample, fmtJunk;
    char dataHead[4];
    unsigned long dataLength;
    short data;
    
    fp = fopen(fileName, "rb");

    if (fp == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    fread(riffHead, 1, 4, fp);
    fread(&riffLength, 4, 1, fp);
    fread(riffType, 1, 4, fp);
    fread(fmtHead, 1, 4, fp);
    fread(&fmtLength, 4, 1, fp);
    fread(&fmtWavFormat, 2, 1, fp);
    fread(&fmtChannels, 2, 1, fp);
    fread(&fmtSamplesPerSec, 4, 1, fp);
    fread(&fmtBytesPerSec, 4, 1, fp);
    fread(&fmtBlockSize, 2, 1, fp);
    fread(&fmtBitsPerSample, 2, 1, fp);
    printf("fmtLength : %lu\n", fmtLength);
    for (i = 16; i < (int)fmtLength; i+=2) {
        fread(&fmtJunk, 2, 1, fp);
    }
    fread(dataHead, 1, 4, fp);
    fread(&dataLength, 4, 1, fp);
    
    pcm->channels = fmtChannels;
    pcm->sampleRate = fmtSamplesPerSec;
    pcm->bps = fmtBitsPerSample;
    pcm->length = dataLength / 4;
    
    printf("fmtSamplesPerSec : %lu\n", fmtSamplesPerSec);
    printf("fmtBitsPerSample : %hu\n", fmtBitsPerSample);
    printf("dataLength : %lu\n", dataLength);
    printf("length : %u\n", pcm->length);
    
    pcm->dataLeft = calloc(pcm->length, sizeof(double));
    pcm->dataRight = calloc(pcm->length, sizeof(double));
    
    for (i = 0; i < pcm->length; i++) {
        fread(&data, 2, 1, fp);
        pcm->dataLeft[i] = (double)data / 32768.0;
        
        fread(&data, 2, 1, fp);
        pcm->dataRight[i] = (double)data / 32768.0;
    }
    
    fclose(fp);

    return 0;
}

void exportStereoWav(StereoWAV *pcm, char *fileName) {
    FILE *fp;
    unsigned int i;
    char riffHead[4];
    long riffLength;
    char riffType[4];
    char fmtHead[4];
    long fmtLength;
    short fmtWavFormat, fmtChannels;
    long fmtSamplesPerSec, fmtBytesPerSec;
    short fmtBlockSize, fmtBitsPerSample;
    char dataHead[4];
    unsigned long dataLength;
    short data;
    double s;
    
    riffHead[0] = 'R';
    riffHead[1] = 'I';
    riffHead[2] = 'F';
    riffHead[3] = 'F';
    riffLength = 36 + pcm->length * 4;
    riffType[0] = 'W';
    riffType[1] = 'A';
    riffType[2] = 'V';
    riffType[3] = 'E';
    
    fmtHead[0] = 'f';
    fmtHead[1] = 'm';
    fmtHead[2] = 't';
    fmtHead[3] = ' ';
    fmtLength = 16;
    fmtWavFormat = 1;
    fmtChannels = 2;
    fmtSamplesPerSec = pcm->sampleRate;
    fmtBytesPerSec = pcm->sampleRate * pcm->bps / 8 * 2;
    fmtBlockSize = pcm->bps / 8 * 2;
    fmtBitsPerSample = pcm->bps;
    
    printf("length : %u\n", pcm->length);
    printf("fmtSamplesPerSec : %lu\n", fmtSamplesPerSec);
    printf("fmtBytesPerSec : %lu\n", fmtBytesPerSec);
    printf("fmtBlockSize : %hu\n", fmtBlockSize);
    printf("fmtBitsPerSample : %u\n", fmtBitsPerSample);
    
    dataHead[0] = 'd';
    dataHead[1] = 'a';
    dataHead[2] = 't';
    dataHead[3] = 'a';
    dataLength = pcm->length * 4;
    
    fp = fopen(fileName, "wb");
    
    fwrite(riffHead, 1, 4, fp);
    fwrite(&riffLength, 4, 1, fp);
    fwrite(riffType, 1, 4, fp);
    fwrite(fmtHead, 1, 4, fp);
    fwrite(&fmtLength, 4, 1, fp);
    fwrite(&fmtWavFormat, 2, 1, fp);
    fwrite(&fmtChannels, 2, 1, fp);
    fwrite(&fmtSamplesPerSec, 4, 1, fp);
    fwrite(&fmtBytesPerSec, 4, 1, fp);
    fwrite(&fmtBlockSize, 2, 1, fp);
    fwrite(&fmtBitsPerSample, 2, 1, fp);
    fwrite(dataHead, 1, 4, fp);
    fwrite(&dataLength, 4, 1, fp);
    
    for (i = 0; i < pcm->length; i++) {
        s = (pcm->dataLeft[i] + 1.0) / 2.0 * 65536.0;
        
        if (s > 65535.0) {
            s = 65535.0;
        } else if (s < 0.0) {
            s = 0.0; 
        }
        
        data = (short)(s + 0.5) - 32768;
        fwrite(&data, 2, 1, fp);
        
        s = (pcm->dataRight[i] + 1.0) / 2.0 * 65536.0;
        
        if (s > 65535.0) {
            s = 65535.0;
        } else if (s < 0.0) {
            s = 0.0;
        }
        
        data = (short)(s + 0.5) - 32768;
        fwrite(&data, 2, 1, fp);
    }
    
    fclose(fp);
}
