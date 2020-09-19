#ifndef __WAV__
#define __WAV__
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct CLI {
   char *input; // input file defined by -i
   char *output; // output file defined by  -o
   float vol; // adjustment volume
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
   int16_t *Right; // Right Channel Samples OR only Channel in Mono
   int16_t *Left; // Left Channel Samples
} wave;

void read_audio(wave *audio, FILE *wav);
void wav_info(wave audio);
void wav_play(wave audio, CLI cl);
void wav_vol(wave *audio, CLI cl);
void wav_norm(wave *audio);
int16_t found_max(wave *audio);
int16_t clipping(int16_t sample);

#endif