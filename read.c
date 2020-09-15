#include <ctype.h>
#include <err.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct WAVE {
   //[CHUNK RIFF]
   unsigned char chunkID[4]; // RIFF string, identify file type
   uint32_t chunkSize; // size of file in bytes [No ChunkID & No ChunkSize]
   unsigned char format[4]; // WAVE string,defines the format of the rest of the file
   //[CHUNK FMT]
   unsigned char fmtChunkID[4]; // Chunk Header fmt string with trailing null char
   uint32_t fmtChunkSize; // length of the format data
   uint16_t audioFormat; // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
   uint16_t channels; // nº.of channels
   uint32_t sampleRate; // nº Samples per Sec || sampling rate (blocks per second)
   uint32_t byteRate; // nº Bytes per second [SampleRate * NumChannels * BitsPerSample/8]]
   uint16_t blockAlign; // Bytes per sample of ALL Channels [NumChannels * BitsPerSample/8]
   uint16_t bitsPS; // nº bits per sample, per channel 8- 8bits, 16- 16 bits etc
   //[CHUNK DATA]
   unsigned char dataChunkID[4]; // Chunk Header data, DATA string or FLLR string
   uint32_t dataChunkSize; // size of all samples, NumSamples * NumChannels * BitsPerSample/8
                           // -size of the next chunk
   uint32_t samplesPC; // Number of Samples per Channel
} wave;

int main(int argc, char **argv)
{
   char *fileIn = NULL, *fileOut = NULL;
   FILE *wav;
   int option;
   wave audio;
   if (argc > 1)
   {
      while ((option = getopt(argc, argv, "i:o:")) != -1)
      {
         switch (option)
         {
         case 'i':
            fileIn = optarg;
            break;

         case 'o':
            fileOut = optarg;
            break;
         }
      }
   }

   printf("Opening file..\n");
   if (fileIn == NULL)
      wav = stdin;
   else
      wav = fopen(fileIn, "rd");

   if (wav == NULL)
   {
      printf("Error opening filen");
      exit(1);
   }

   fread(&audio, 1, 44, wav);
   audio.samplesPC = (audio.dataChunkSize / audio.channels / (audio.bitsPS / 8));
}
