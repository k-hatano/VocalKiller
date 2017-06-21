
#include "WavUtil.h"

int main(int args, char **argv) {
    StereoWAV inputWav;
    MonoWAV outputWav;
    unsigned int i;

    char *importFileName;
    char *exportFileName;

    if (args < 2) {
        printf("Usage :\n  ./vocalkiller [wav file name]\n");
        return 0;
    }

    importFileName = argv[1];

    if (strstr(importFileName, ".wav") == NULL && strstr(importFileName, ".WAV") == NULL) {
        printf("Error: import file seems not to be a wav file \n");
        return 0;
    }

    exportFileName = calloc(strlen(importFileName) + 4, sizeof(char));
    strcpy(exportFileName, importFileName);
    strcpy(&exportFileName[strlen(importFileName) - 4], "_vk.wav");

    printf("Importing wav file...\n");
    int result = importStereoWav(&inputWav, importFileName);

    if (result == ERROR_FILE_NOT_FOUND) {
        printf("Error: input file not found.\n");
    } else if (inputWav.channels != 2) {
        printf("Error: input file is not a stereo wav file.\n");
    } else {
        outputWav.sampleRate = inputWav.sampleRate;
        outputWav.bps = inputWav.bps;
        outputWav.length = inputWav.length;

        outputWav.data = calloc(outputWav.length, sizeof(double));

        printf("Generating wave data...\n");
        for (i = 0; i < outputWav.length; i++) {
            outputWav.data[i] = inputWav.dataLeft[i] - inputWav.dataRight[i];
        }

        printf("Exporting wav file...\n");
        exportMonoWav(&outputWav, exportFileName);

        printf("Complete!\n");

        free(outputWav.data);
    }
    
    free(inputWav.dataLeft);
    free(inputWav.dataRight);
    free(exportFileName);
    
    return 0;
}
