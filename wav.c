#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void read_audio(wave *audio, FILE *wav)
{
   fread(audio, 1, 44, wav);
   audio->samplesPC = (audio->dataChunkSize / audio->channels / (audio->bitsPS / 8));

   if (audio->channels == 1)
   {
      audio->Right = (int16_t *)malloc(sizeof(int16_t) * audio->dataChunkSize);
      fread(audio->Right, audio->bitsPS / 8, audio->samplesPC, wav);

   } else
   {
      audio->Right = (int16_t *)malloc(audio->dataChunkSize);
      audio->Left = (int16_t *)malloc(audio->dataChunkSize);
      for (int i = 0; i < audio->samplesPC; i++)
      {
         fread(&audio->Right[i], audio->bitsPS / 8, 1, wav);
         fread(&audio->Left[i], audio->bitsPS / 8, 1, wav);
      }
   }
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

void wav_play(wave audio, CLI cl)
{
   FILE *output;
   if (cl.output == NULL)
      output = stdout;
   else
      output = fopen("teste.wav", "w");

   fwrite(&audio, 1, 44, output);
   if (audio.channels == 1)
      fwrite(audio.Right, audio.bitsPS / 8, audio.samplesPC, output);
   else
   {
      for (int i = 0; i <= audio.samplesPC; i++)
      {
         fwrite(&audio.Right[i], audio.bitsPS / 8, 1, output);
         fwrite(&audio.Left[i], audio.bitsPS / 8, 1, output);
      }
   }
   fclose(output);
}

void wav_vol(wave *audio, CLI cl)
{
   float vol;
   if (cl.vol == 0)
      vol = 1;
   else
      vol = cl.vol;

   if (audio->channels == 1)
      for (int i = 0; i <= audio->samplesPC; i++)
         audio->Right[i] = audio->Right[i] * vol;
   else
   {
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         audio->Right[i] = audio->Right[i] * vol;
         audio->Left[i] = audio->Left[i] * vol;
      }
   }
}

void wav_norm(wave *audio)
{
   int16_t max;
   max = found_max(audio);

   double norm;
   norm = (float)INT16_MAX / max;
   if (audio->channels == 1)
      for (int i = 0; i <= audio->samplesPC; i++)
         audio->Right[i] = clipping(audio->Right[i] * norm);
   else
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         audio->Right[i] = clipping(audio->Right[i] * norm);
         audio->Left[i] = clipping(audio->Left[i] * norm);
      }
}

int16_t found_max(wave *audio)
{
   int16_t max;
   if (audio->channels == 1)
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         if (abs(audio->Right[i]) > max)
            max = abs(audio->Right[i]);
      }
   else
   {
      for (int i = 0; i <= audio->samplesPC; i++)
         if (abs(audio->Right[i]) > max)
            max = abs(audio->Right[i]);

      for (int i = 0; i <= audio->samplesPC; i++)
         if (abs(audio->Left[i]) > max)
            max = abs(audio->Left[i]);
   }
   return max;
}

int16_t clipping(int16_t sample)
{
   if (sample > INT16_MAX)
      return INT16_MAX;
   else if (sample < INT16_MIN)
      return INT16_MIN;
   return sample;
}

/*
void wav_vol2(wave audio, CLI cl)
{
   FILE *output;
   if (cl.output == NULL)
      output = stdout;
   else
      output = fopen("teste.wav", "w");

   fwrite(&audio, 1, 44, output);

   float vol;
   if (cl.vol == 0)
      vol = 1;
   else
      vol = cl.vol;

   int16_t left;
   int16_t right;
   if (audio.channels == 1)
      for (int i = 0; i <= audio.samplesPC; i++)
      {
         right = audio.Right[i] * vol;
         fwrite(&right, audio.bitsPS / 8, 1, output);
      }
   else
   {
      for (int i = 0; i <= audio.samplesPC; i++)
      {
         left = audio.Left[i] * vol;
         right = audio.Right[i] * vol;
         fwrite(&left, audio.bitsPS / 8, 1, output);
         fwrite(&right, audio.bitsPS / 8, 1, output);
      }
   }
   fclose(output);
}
*/
