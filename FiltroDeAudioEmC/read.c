#include <err.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct WAVE {
   //[CHUNK RIFF]
   unsigned char chunkID[4]; // RIFF string, identify file type
   unsigned int chunkSize; // size of file in bytes [No ChunkID & No ChunkSize]
   unsigned char format[4]; // WAVE string,defines the format of the rest of the file
   //[CHUNK FMT]
   unsigned char fmtChunkID[4]; // Chunk Header fmt string with trailing null char
   unsigned int fmtChunkSize; // length of the format data
   unsigned int audioFormat; // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
   unsigned int channels; // nº.of channels
   unsigned int sampleRate; // nº Samples per Sec || sampling rate (blocks per second)
   unsigned int byteRate; // nº Bytes per second [SampleRate * NumChannels * BitsPerSample/8]]
   unsigned int blockAlign; // Bytes per sample of ALL Channels [NumChannels * BitsPerSample/8]
   unsigned int bitsPS; // nº bits per sample, per channel 8- 8bits, 16- 16 bits etc
   //[CHUNK DATA]
   unsigned char dataChunkID[4]; // Chunk Header data, DATA string or FLLR string
   unsigned int dataChunkSize; // size of all samples, NumSamples * NumChannels * BitsPerSample/8
                               // -size of the next chunk
} wave;

int main()
{

   FILE *wav;
   char *filesrc;
   wave audio;

   filesrc = "2.wav";
   printf("Opening file..\n");
   wav = fopen(filesrc, "rb");
   if (wav == NULL)
   {
      printf("Error opening filen");
      exit(1);
   }
   // read RIFF parts
   int read = 0;
   read = fread(&audio.chunkID, 1, 4, wav);
   printf("(1-4) ");
   for (int i = 0; i < 4; i++) printf("%c", audio.chunkID[i]);
   printf("\n");

   read = fread(&audio.chunkSize, 4, 1, wav);
   printf("(5-8) chunkSize: %u bytes %u Kb\n", audio.chunkSize, audio.chunkSize / 1024);

   read = fread(audio.format, 4, 1, wav);
   printf("(9-12) File format: %s\n", audio.format);
}
