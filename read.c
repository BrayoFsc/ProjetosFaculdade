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

int main()
{

   FILE *wav;
   char *filesrc;
   wave audio;
   uint16_t buffer2[2];
   filesrc = "music.wav";
   printf("Opening file..\n");
   wav = fopen(filesrc, "rb");
   if (wav == NULL)
   {
      printf("Error opening filen");
      exit(1);
   }
   // read RIFF parts
   fread(&audio.chunkID, 1, 4, wav);
   printf("( 1- 4) ");
   for (int i = 0; i < 4; i++) printf("%c", audio.chunkID[i]);
   printf("\n");

   fread(&audio.chunkSize, 4, 1, wav);
   printf("( 5- 8)  Chunk Size: %u bytes %u Kb\n", audio.chunkSize, audio.chunkSize / 1024);

   fread(&audio.format, 4, 1, wav);
   printf("( 9-12) File format: %s\n", audio.format);
   // read FMT parts
   fread(&audio.fmtChunkID, 1, 4, wav);
   printf("(13-16) FMT Chunk Header: %s\n", audio.fmtChunkID);

   fread(&audio.fmtChunkSize, 4, 1, wav);
   printf("(17-20) FMT CHunk Size: %u\n", audio.fmtChunkSize);

   fread(&audio.audioFormat, 2, 1, wav);
   printf("(21-22) Audio Format: %u\n", audio.audioFormat);

   // fread(&buffer2, 2, 1, wav);
   // audio.channels = buffer2[0] | (buffer2[1]);
   fread(&audio.channels, 2, 1, wav);
   printf("(23-24) Number of Channels: %u\n", audio.channels);

   fread(&audio.sampleRate, 4, 1, wav);
   printf("(25-28) Sample Rate: %u\n", audio.sampleRate);

   fread(&audio.byteRate, 4, 1, wav);
   printf("(29-32) Byte Rate: %u\n", audio.byteRate);

   fread(&audio.blockAlign, 2, 1, wav);
   printf("(33-34) Block Align: %u\n", audio.blockAlign);

   fread(&audio.bitsPS, 2, 1, wav);
   printf("(35-36) Bits Per Sample: %u\n", audio.bitsPS);

   fread(&audio.dataChunkID, 1, 4, wav);
   printf("(37-40) ");
   for (int i = 0; i < 4; i++) printf("%c", audio.dataChunkID[i]);
   printf("\n");

   fread(&audio.dataChunkSize, 4, 1, wav);
   printf("(41-44) Data Size: %u\n", audio.dataChunkSize);
   audio.samplesPC = (audio.dataChunkSize / audio.channels / (audio.bitsPS / 8));
   printf("%u\n", audio.samplesPC);
}
