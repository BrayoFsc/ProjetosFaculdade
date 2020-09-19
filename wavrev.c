#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void command_line(CLI *cl, int argc, char **argv)
{
   int option;
   if (argc > 1)
      while ((option = getopt(argc, argv, "i:o:l:")) != -1)
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
}

void cpysample(int16_t *audio, int16_t *copy, int tam)
{
   for (int i = 0; i < tam; i++)
      copy[i] = audio[i];
}

void wav_rev(wave *audio)
{
   if (audio->channels == 1)
   {
      int16_t *rightcpy = NULL;
      rightcpy = (int16_t *)malloc(sizeof(int16_t) * audio->chunkSize);

      for (int i = 0; i <= audio->samplesPC; i++)
         rightcpy[i] = audio->Right[i];

      for (int i = 0; i < audio->samplesPC; i++)
         audio->Right[i] = rightcpy[audio->samplesPC - i];

   } else
   {
      int16_t *rightcpy = NULL, *leftcpy = NULL;
      rightcpy = (int16_t *)malloc(sizeof(int16_t) * audio->chunkSize);
      leftcpy = (int16_t *)malloc(sizeof(int16_t) * audio->chunkSize);

      for (int i = 0; i <= audio->samplesPC; i++)
      {
         rightcpy[i] = audio->Right[i];
         leftcpy[i] = audio->Left[i];
      }

      for (int i = 0; i < audio->samplesPC; i++)
      {
         audio->Right[i] = rightcpy[audio->samplesPC - i];
         audio->Left[i] = leftcpy[audio->samplesPC - i];
      }
   }
}
int main(int argc, char **argv)
{
   CLI cl;
   cl.input = NULL;
   cl.output = NULL;
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
   wav_rev(&audio);
   wav_play(audio, cl);
   fclose(wav);
}
