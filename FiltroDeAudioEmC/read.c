#include <stdio.h>
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
  unsigned int sampleRate; // sampling rate (blocks per second)
  unsigned int byteRate; // Bytes per second [SampleRate * NumChannels * BitsPerSample/8]]
  unsigned int blockAlign; // Bytes per sample of ALL Channels [NumChannels * BitsPerSample/8]
  unsigned int bitsPS; // bits per sample, per channel 8- 8bits, 16- 16 bits etc
  unsigned char dataChunkID[4]; // Chunk Header data, DATA string or FLLR string
  unsigned int dataChunkSize; // size of all samples, NumSamples * NumChannels * BitsPerSample/8 -
                              // size of the next chunk
                              // that will be read
} wave;

int main()
{
  wave music;
}
