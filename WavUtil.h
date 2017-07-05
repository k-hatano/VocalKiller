
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_FILE_NOT_FOUND 1

typedef struct {
	long channels;
    long sampleRate;
    long bps;
    unsigned long length;
    double *data;
} MonoWAV;

typedef struct {
	long channels;
    long sampleRate;
    long bps;
    unsigned long length;
    double *dataLeft;
    double *dataRight;
} StereoWAV;

int importMonoWav(MonoWAV *pcm, char *fileName);
void exportMonoWav(MonoWAV *pcm, char *fileName);
int importStereoWav(StereoWAV *pcm, char *fileName);
void exportStereoWav(StereoWAV *pcm, char *fileName);
