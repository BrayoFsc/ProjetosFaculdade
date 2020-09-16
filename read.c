#include <ctype.h>
#include <err.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// command line instructions
typedef struct CLI {
   char *input; // input file defined by -i
   char *output; // output file defined by  -o
   char *option; // list of various options
} CLI;

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

   //[AUDIO DATA]
   uint32_t *Right; // Right Channel Samples
   uint32_t *Left; // Left Channel Samples
} wave;

void command_line(CLI *cl, int argc, char **argv)
{
   int option;
   if (argc > 1)
   {
      while ((option = getopt(argc, argv, "i:o:")) != -1)
      {
         switch (option)
         {
         case 'i':
            cl->input = optarg;
            break;

         case 'o':
            cl->output = optarg;
            break;
         }
      }
   } else
   {
      cl->input = NULL;
      cl->output = NULL;
      cl->option = NULL;
   }
}

void read_audio(wave *audio, FILE *wav)
{
   fread(audio, 1, 44, wav);
   audio->samplesPC = (audio->dataChunkSize / audio->channels / (audio->bitsPS / 8));
}

void wav_info(wave audio)
{
   printf("riff tag        (4 bytes): \"%.4s\"\n", audio.chunkID);
   printf("riff size       (4 bytes): %u\n", audio.chunkSize);
   printf("wave tag        (4 bytes): \"%.4s\"\n", audio.format);
   printf("form tag        (4 bytes): \"%.4s\"\n", audio.fmtChunkID);
   printf("fmt_size        (4 bytes): %u\n", audio.fmtChunkSize);
   printf("audio_format    (2 bytes): %u\n", audio.audioFormat);
   printf("num_channels    (2 bytes): %u\n", audio.channels);
   printf("sample_rate     (4 bytes): %u\n", audio.sampleRate);
   printf("byte_rate       (4 bytes): %u\n", audio.byteRate);
   printf("block_align     (2 bytes): %u\n", audio.blockAlign);
   printf("bits_per_sample (2 bytes): %u\n", audio.bitsPS);
   printf("data tag        (4 bytes): \"%.4s\"\n", audio.dataChunkID);
   printf("data size       (4 bytes): %u\n", audio.dataChunkSize);
   printf("bytes per sample         : %u\n", audio.bitsPS / 8);
   printf("samples per channel      : %u\n", audio.samplesPC);
}
int main(int argc, char **argv)
{
   CLI cl;
   command_line(&cl, argc, argv);

   FILE *wav;
   wave audio;

   printf("Opening file..\n");
   if (cl.input == NULL)
      wav = stdin;
   else
      wav = fopen(cl.input, "rd");

   if (wav == NULL)
   {
      printf("Error opening filen");
      exit(1);
   }

   // reading file data into the struct
   read_audio(&audio, wav);
   wav_info(audio);
}
