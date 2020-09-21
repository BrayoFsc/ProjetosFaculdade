#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void command_line(CLI *cl, int argc, char **argv)
{
   int option;
   if (argc > 1)
   {
      option = getopt(argc, argv, "o:");
      switch (option)
      {
      case 'o':
         cl->output = optarg;
         cl->files = argc - 3; // tira o -o, output e o 0 que é o executavel
         break;
      }
   }
}

uint32_t greater(uint32_t a, uint32_t b)
{
   switch (a > b)
   {
   case 1:
      return a;
      break;
   case 2:
      return b;
      break;
   }
   return a;
}

void read_mix(wave *audio, FILE *wav)
{ // armazeno os dados dos arquivos anteriores, se não tiver nenhum anterior, deve ser 0
   uint32_t chunkmax = audio->chunkSize;
   uint32_t datamax = audio->dataChunkSize;
   uint32_t samplePC = audio->samplesPC;
   fread(audio, 1, 44, wav);
   // leio o header e pego os maiores valores
   audio->chunkSize = greater(chunkmax, audio->chunkSize);
   audio->dataChunkSize = greater(datamax, audio->dataChunkSize);
   audio->samplesPC = greater(samplePC, audio->samplesPC);

   int16_t sample;
   if (audio->channels == 1)
   {
      audio->Right = (int16_t *)realloc(audio->Right, audio->samplesPC * sizeof(int16_t));
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         fread(&sample, audio->bitsPS / 8, 1, wav);
         audio->Right[i] += sample;
      }
   } else
   {
      audio->Right = (int16_t *)realloc(audio->Right, audio->samplesPC * sizeof(int16_t));
      audio->Left = (int16_t *)realloc(audio->Left, audio->samplesPC * sizeof(int16_t));
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         fread(&sample, audio->bitsPS / 8, 1, wav);
         audio->Right[i] += sample;
         fread(&sample, audio->bitsPS / 8, 1, wav);
         audio->Left[i] += sample;
      }
   }
}
int main(int argc, char **argv)
{
   CLI cl;
   cl.output = NULL;
   command_line(&cl, argc, argv);

   if (argc < 1)
      perror("no file selected");

   FILE **wav;
   wav = NULL;
   wav = malloc(cl.files * sizeof(FILE *));
   if (wav == NULL)
      perror("Error on memory allocation");

   printf("Opening files..\n");

   for (int i = 1; i <= cl.files; i++)
      wav[i - 1] = fopen(argv[i], "rd");

   wave audio;
   audio.Right = NULL;
   audio.Left = NULL;

   // reading file data into the struct
   audio.dataChunkSize = 0;
   audio.fmtChunkSize = 0;
   audio.samplesPC = 0;
   for (int i = 0; i < cl.files; i++)
      read_mix(&audio, wav[i]);

   wav_play(audio, cl);
   for (int i = 0; i < cl.files; i++)
      fclose(wav[i]);
}
